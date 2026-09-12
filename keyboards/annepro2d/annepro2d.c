/* Copyright 2021 OpenAnnePro community
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "hal.h"
#include "annepro2d.h"
#include "annepro2_ble.h"
#include "spi_master.h"
#include "aw20216s.h"

#define RAM_MAGIC_LOCATION 0x20001ffc
#define IAP_MAGIC_VALUE 0x0000fab2

static const SerialConfig ble_uart_config = {
    .speed = 115200,
};

void mcu_reset(void) {
    __disable_irq();
    NVIC_SystemReset();
}

void bootloader_jump(void) {
    // Magic key to set keyboard to IAP. The value was found by reversing the
    // original boot loader - if this value is present the MCU stays in IAP.
    *((uint32_t *)RAM_MAGIC_LOCATION) = IAP_MAGIC_VALUE;

    // Load the main MCU into IAP
    __disable_irq();
    NVIC_SystemReset();
}

void keyboard_pre_init_kb(void) {
    // Enable the LED driver supply (PB13). Required before the AW20216S
    // output stage can light anything; matches official 3.08 (0xF09A).
    gpio_set_pin_output(LED_POWER_PIN);
    gpio_write_pin_high(LED_POWER_PIN);
    wait_ms(5);

    // AW20216S RGB init is handled by the rgb_matrix driver (aw20216s).
    keyboard_pre_init_user();
}

void keyboard_post_init_kb(void) {
    // Start BLE UART (USART1, PA4/PA5, 115200 8N1)
    sdStart(&SD1, &ble_uart_config);
    annepro2_ble_startup();

    // Give the send uart thread some time to send out the wakeup before we
    // read back any response.
    wait_ms(100);

    // Clear out the receive buffer from the BLE wakeup response.
    while (!sdGetWouldBlock(&SD1)) {
        sdGet(&SD1);
    }

    keyboard_post_init_user();
}

void matrix_scan_kb(void) {
    // Feed any BLE UART bytes into the frame parser.
    while (!sdGetWouldBlock(&SD1)) {
        annepro2_ble_consume(sdGet(&SD1));
    }
    annepro2_ble_poll();

    matrix_scan_user();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case KC_AP2_BT1:
                annepro2_ble_broadcast(0);
                return false;

            case KC_AP2_BT2:
                annepro2_ble_broadcast(1);
                return false;

            case KC_AP2_BT3:
                annepro2_ble_broadcast(2);
                return false;

            case KC_AP2_BT4:
                annepro2_ble_broadcast(3);
                return false;

            case KC_AP2_USB:
                annepro2_ble_disconnect();
                return false;

            case KC_AP2_BT_UNPAIR:
                annepro2_ble_unpair();
                return false;

            default:
                break;
        }
    }

#if defined(RGB_MATRIX_ENABLE)
    // Consume both press and release: the generic handler acts on key release,
    // so returning here for the release too is required or it will re-toggle.
    if (keycode == QK_RGB_MATRIX_TOGGLE) {
        // Toggle between the full animation and indicator-only mode by
        // clearing the LED flags instead of disabling the engine, so
        // indicators (e.g. Caps Lock) keep working with the matrix "off".
        if (record->event.pressed) {
            switch (rgb_matrix_get_flags()) {
                case LED_FLAG_ALL:
                    rgb_matrix_set_flags(LED_FLAG_NONE);
                    rgb_matrix_set_color_all(0, 0, 0);
                    break;
                default:
                    rgb_matrix_set_flags(LED_FLAG_ALL);
                    break;
            }
        }
        if (!rgb_matrix_is_enabled()) {
            rgb_matrix_set_flags(LED_FLAG_ALL);
            rgb_matrix_enable();
        }
        return false;
    }
#endif

    return process_record_user(keycode, record);
}

#if defined(RGB_MATRIX_ENABLE)
bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) {
        return false;
    }

    // Light the Caps Lock key while Caps Lock is active.
    if (host_keyboard_led_state().caps_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(CAPS_LOCK_LED_INDEX, 0xFF, 0xFF, 0xFF);
    } else if (!rgb_matrix_get_flags()) {
        // In indicator-only mode nothing else clears the LED, so do it here.
        RGB_MATRIX_INDICATOR_SET_COLOR(CAPS_LOCK_LED_INDEX, 0, 0, 0);
    }
    return true;
}
#endif
