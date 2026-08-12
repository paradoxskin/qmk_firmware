/*
    Copyright (C) 2020 Yaotian Feng, Codetector<codetector@codetector.cn>
    SPDX-License-Identifier: Apache-2.0

    Anne Pro 2D BLE host driver and UART protocol implementation.

    Wire format is documented in the AP2D / C18 BLE reverse engineering notes:
    frames are 8-byte header + payload, length in header byte 4, total length
    is 8 + byte[4]. BLE -> MCU responses swap the routing nibble 0x53 -> 0x35.
    The 0x20/0x0c post-connect HID handshake and 0x20/0x07 Caps Lock status
    sync are request/reply pairs (replies use routing 0x43).
*/

#include "annepro2_ble.h"
#include "ch.h"
#include "hal.h"
#include "host.h"
#include "host_driver.h"
#include "report.h"
#include "timer.h"

/* -------------------- Static Function Prototypes -------------------------- */
static uint8_t ap2_ble_leds(void);
static void    ap2_ble_mouse(report_mouse_t *report);
static void    ap2_ble_extra(report_extra_t *report);
static void    ap2_ble_keyboard(report_keyboard_t *report);

static void ap2_ble_route_to_ble(void);
static void ap2_ble_route_to_usb(void);
static void ap2_ble_reply_handshake(void);
static void ap2_ble_reply_capslock(uint8_t value);

/* -------------------- Static Local Variables ------------------------------ */
static host_driver_t ap2_ble_driver = {
    ap2_ble_leds, ap2_ble_keyboard, NULL, ap2_ble_mouse, ap2_ble_extra
};

/*
 * MCU -> BLE command templates. Frames are 8 + LL bytes total.
 * The trailing 0x00 after the slot byte on broadcast/connect is a
 * compatibility-required byte confirmed in the RE notes.
 */
static const uint8_t ble_mcu_wakeup[11]      = {0x7b, 0x12, 0x53, 0x00, 0x03, 0x00, 0x01, 0x7d, 0x02, 0x01, 0x02};
static const uint8_t ble_mcu_broadcast[10]   = {0x7b, 0x12, 0x53, 0x00, 0x03, 0x00, 0x00, 0x7d, 0x40, 0x01};
static const uint8_t ble_mcu_connect[10]     = {0x7b, 0x12, 0x53, 0x00, 0x03, 0x00, 0x00, 0x7d, 0x40, 0x04};
static const uint8_t ble_mcu_unpair[10]      = {0x7b, 0x12, 0x53, 0x00, 0x02, 0x00, 0x00, 0x7d, 0x40, 0x05};
static const uint8_t ble_mcu_keyboard[10]    = {0x7b, 0x12, 0x53, 0x00, 0x0A, 0x00, 0x00, 0x7d, 0x10, 0x04};

/*
 * Consumer report. BLE 2.13 (AP2D) uses a four little-endian uint16_t Usage
 * array (8 payload bytes), so LL is 0x0A here, unlike BLE 2.05's 0x06/4-byte
 * bitmask used by the C18 driver.
 */
static const uint8_t ble_mcu_consumer[10]    = {0x7b, 0x12, 0x53, 0x00, 0x0A, 0x00, 0x00, 0x7d, 0x10, 0x08};

/* MCU -> BLE replies (routing 0x43). */
static const uint8_t ble_mcu_handshake_reply[12] = {0x7b, 0x12, 0x43, 0x00, 0x04, 0x00, 0x00, 0x7d, 0x20, 0x0c, 0x00, 0x00};
static const uint8_t ble_mcu_caps_reply[10]      = {0x7b, 0x12, 0x43, 0x00, 0x03, 0x00, 0x00, 0x7d, 0x20, 0x07};

static host_driver_t *last_host_driver = NULL;

/* Current LED (lock light) state as reported over the BLE route. */
static uint8_t ble_host_leds = 0;

/* USB HID LED Output: Caps Lock is bit 1. */
#define BLE_LED_CAPS_LOCK (1 << 1)

/* Whether the keyboard should switch to the BLE host driver once the
 * BLE module confirms the HID connection via the 0x20/0x0c handshake. */
static bool ble_pending_route = false;

/* -------------------- UART RX frame parser -------------------------------- */

enum ble_rx_state { BLE_RX_SYNC, BLE_RX_HEADER, BLE_RX_PAYLOAD };

/* Maximum accepted payload length (real BLE->KEY frames are <= 3 bytes). */
#define BLE_RX_PAYLOAD_MAX 32
/* A frame that stops mid-way for longer than this is discarded. */
#define BLE_RX_HALF_FRAME_TIMEOUT 20

static enum ble_rx_state rx_state    = BLE_RX_SYNC;
static uint8_t           rx_header[8];
static uint8_t           rx_header_pos;
static uint8_t           rx_payload[BLE_RX_PAYLOAD_MAX];
static uint8_t           rx_payload_len;
static uint8_t           rx_payload_pos;
static uint16_t          rx_last_byte_time;

static void ble_rx_reset(void) {
    rx_state         = BLE_RX_SYNC;
    rx_header_pos    = 0;
    rx_payload_pos   = 0;
    rx_payload_len   = 0;
    rx_last_byte_time = timer_read();
}

static void ble_rx_dispatch(void) {
    /* BLE -> MCU responses use routing nibble 0x35 (byte 2). */
    if (rx_header[2] != 0x35) {
        return;
    }
    uint8_t group = rx_payload[0];
    uint8_t op    = rx_payload[1];

    if (group == 0x20) {
        if (op == 0x07 && rx_payload_len == 3 && rx_payload[2] <= 1) {
            /* BLE-initiated Caps Lock status sync. Map to standard Caps Lock
             * LED bit and echo the value back with routing 0x43. */
            if (rx_payload[2]) {
                ble_host_leds |= BLE_LED_CAPS_LOCK;
            } else {
                ble_host_leds &= ~BLE_LED_CAPS_LOCK;
            }
            ap2_ble_reply_capslock(rx_payload[2]);
        } else if (op == 0x0c && rx_payload_len == 3) {
            /* Post-connection HID handshake request. Reply and (if the user
             * requested a BLE slot) switch the host route. */
            ap2_ble_reply_handshake();
            if (ble_pending_route) {
                ap2_ble_route_to_ble();
            }
        }
        /* 0x0b / 0x24 slot state notifications are informational only. */
    }
}

static void ble_rx_consume(uint8_t byte) {
    /* Discard a frame that has been left half-received too long so a later
     * complete frame can resynchronize cleanly. */
    if (rx_state != BLE_RX_SYNC && timer_elapsed(rx_last_byte_time) > BLE_RX_HALF_FRAME_TIMEOUT) {
        ble_rx_reset();
    }
    rx_last_byte_time = timer_read();

    switch (rx_state) {
        case BLE_RX_SYNC:
            if (byte == 0x7b) {
                rx_header[0] = byte;
                rx_header_pos = 1;
                rx_state     = BLE_RX_HEADER;
            }
            break;

        case BLE_RX_HEADER:
            rx_header[rx_header_pos] = byte;
            rx_header_pos++;
            if (rx_header_pos == 8) {
                /* Validate header terminator and that the length high bytes
                 * (header[5], header[6]) are zero. */
                if (rx_header[7] == 0x7d && rx_header[5] == 0x00 && rx_header[6] == 0x00) {
                    rx_payload_len = rx_header[4];
                    if (rx_payload_len > BLE_RX_PAYLOAD_MAX) {
                        rx_payload_len = BLE_RX_PAYLOAD_MAX;
                    }
                    rx_payload_pos = 0;
                    rx_state       = BLE_RX_PAYLOAD;
                    if (rx_payload_len == 0) {
                        ble_rx_reset();
                    }
                } else {
                    /* Bad header - resynchronize. */
                    ble_rx_reset();
                }
            }
            break;

        case BLE_RX_PAYLOAD:
            rx_payload[rx_payload_pos] = byte;
            rx_payload_pos++;
            if (rx_payload_pos == rx_payload_len) {
                ble_rx_dispatch();
                ble_rx_reset();
            }
            break;
    }
}

/* -------------------- Public Function Implementation ---------------------- */

void annepro2_ble_startup(void) {
    ble_host_leds     = 0;
    ble_pending_route = false;
    sdWrite(&SD1, ble_mcu_wakeup, sizeof(ble_mcu_wakeup));
}

void annepro2_ble_broadcast(uint8_t port) {
    if (port > 3) {
        port = 3;
    }
    ble_pending_route = true;
    sdWrite(&SD1, ble_mcu_broadcast, sizeof(ble_mcu_broadcast));
    sdPut(&SD1, port);
    sdPut(&SD1, 0x00);
}

void annepro2_ble_connect(uint8_t port) {
    if (port > 3) {
        port = 3;
    }
    ble_pending_route = true;
    sdWrite(&SD1, ble_mcu_connect, sizeof(ble_mcu_connect));
    sdPut(&SD1, port);
    sdPut(&SD1, 0x00);
}

void annepro2_ble_disconnect(void) {
    if (host_get_driver() != &ap2_ble_driver) {
        return;
    }
    ap2_ble_route_to_usb();
}

void annepro2_ble_unpair(void) { sdWrite(&SD1, ble_mcu_unpair, sizeof(ble_mcu_unpair)); }

void annepro2_ble_consume(uint8_t byte) { ble_rx_consume(byte); }

void annepro2_ble_poll(void) {
    if (rx_state != BLE_RX_SYNC && timer_elapsed(rx_last_byte_time) > BLE_RX_HALF_FRAME_TIMEOUT) {
        ble_rx_reset();
    }
}

bool annepro2_ble_is_active(void) { return host_get_driver() == &ap2_ble_driver; }

/* ------------------- Static Function Implementation ----------------------- */

static void ap2_ble_route_to_usb(void) {
    if (host_get_driver() != &ap2_ble_driver) {
        return;
    }
    clear_keyboard();
    ble_pending_route = false;
    ble_host_leds     = 0;
    if (last_host_driver) {
        host_set_driver(last_host_driver);
    }
}

static void ap2_ble_route_to_ble(void) {
    if (host_get_driver() == &ap2_ble_driver) {
        ble_pending_route = false;
        return;
    }
    clear_keyboard();
    last_host_driver  = host_get_driver();
    ble_pending_route = false;
    ble_host_leds     = 0;
    host_set_driver(&ap2_ble_driver);
}

static void ap2_ble_reply_handshake(void) { sdWrite(&SD1, ble_mcu_handshake_reply, sizeof(ble_mcu_handshake_reply)); }

static void ap2_ble_reply_capslock(uint8_t value) {
    sdWrite(&SD1, ble_mcu_caps_reply, sizeof(ble_mcu_caps_reply));
    sdPut(&SD1, value);
}

static uint8_t ap2_ble_leds(void) { return ble_host_leds; }

static void ap2_ble_mouse(report_mouse_t *report) {}

static inline uint16_t CONSUMER2AP2(uint16_t usage) {
    /* BLE 2.13 consumer Report ID 3 is a 4 x uint16_t Usage array whose usage
     * range is 0x0101..0x0202 (AC controls), so the standard Consumer-page
     * media usages are mapped into the AC range. */
    switch (usage) {
        case AUDIO_VOL_DOWN:
            return 0x01B8; /* AC Volume Decrement */
        case AUDIO_VOL_UP:
            return 0x01B7; /* AC Volume Increment */
        case AUDIO_MUTE:
            return 0x01B6; /* AC Mute */
        case TRANSPORT_PLAY_PAUSE:
            return 0x0199; /* AC Play/Pause */
        case TRANSPORT_NEXT_TRACK:
            return 0x018E; /* AC Scan Next Track */
        case TRANSPORT_PREV_TRACK:
            return 0x018F; /* AC Scan Previous Track */
        default:
            return 0x0000;
    }
}

static void ap2_ble_extra(report_extra_t *report) {
    if (report->report_id == REPORT_ID_CONSUMER) {
        uint8_t payload[8] = {0};
        uint16_t usage     = CONSUMER2AP2(report->usage);
        payload[0]         = usage & 0xff;
        payload[1]         = (usage >> 8) & 0xff;

        sdPut(&SD1, 0x00);
        sdWrite(&SD1, ble_mcu_consumer, sizeof(ble_mcu_consumer));
        sdWrite(&SD1, payload, sizeof(payload));
    }
}

static void ap2_ble_keyboard(report_keyboard_t *report) {
    sdPut(&SD1, 0x00);
    sdWrite(&SD1, ble_mcu_keyboard, sizeof(ble_mcu_keyboard));
    sdWrite(&SD1, (uint8_t *)report, KEYBOARD_REPORT_SIZE);
}
