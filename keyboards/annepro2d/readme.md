# Anne Pro 2D QMK firmware

QMK firmware for the Anne Pro 2D (Hexcore). The AP2D is a 60% ANSI keyboard
with a Holtek HT32F52352 KEY MCU, a CC254x BLE module, and per-key RGB driven
directly by the KEY MCU through a single AW20216S LED driver (no separate LED
MCU, unlike the Anne Pro 2 C15/C18).

* Keyboard Maintainer: [paradoxskin](https://github.com/paradoxskin)
* Hardware Supported: Anne Pro 2D (HT32F52352)
* Hardware Availability: [Hexcore](https://www.hexcore.xyz/)

## Hardware summary (from reverse engineering)

* Matrix: 14 columns x 5 rows (COL2ROW).
  * Columns: PC4 PC5 PC8 PC0 PA10 PB1 PA8 PC13 PC12 PA15 PA14 PA11 PD1 PD2
  * Rows: PC3 PC14 PC15 PC1 PC2
* BLE module: USART1 (SD1), PA4/PA5, AF6, 115200 8N1.
* RGB: AW20216S on SPI0 (PB3/PB4/PB5, AF5), CS PB2, EN PD3, LED supply
  enable PB13 (driven high at boot, from official 3.08). 61 LEDs.
* No external LED MCU and no LED UART.

## How to compile

After setting up your build environment, compile the default keymap:

    make annepro2d:default

The build needs the ChibiOS submodules:

    git submodule update --init --recursive

## Installing

1. Put the keyboard into IAP mode by unplugging it, then holding ESC while
   plugging it back in.
2. Flash the built firmware with `annepro2_tools`:

   ```sh
   annepro2_tools --boot annepro2d_default.bin
   ```

   (adjust the path if you picked a different keymap profile)

## BLE keys

* `KC_AP2_BT1`..`KC_AP2_BT4` (default: FN2+1..4): select Bluetooth slot
  0..3 and start advertising. The firmware switches to the BLE HID route only
  after the BLE module confirms the connection with the `0x20/0x0c` handshake.
* `KC_AP2_USB`: switch back to the USB host route.
* `KC_AP2_BT_UNPAIR`: clear all pairings on the BLE module.

Caps Lock state is synchronized over the shared `0x20/0x07` UART ABI and shown
on the Caps Lock LED (index 28). Media keys are encoded for BLE 2.13's Consumer
Report ID 3 (four `uint16_t` usages in the 0x0101..0x0202 AC-control range).

## Notes / open items

* The LED channel map in `led.c` is reversed from the official AP2D KEY 3.08
  per-key channel table (0x123C4), so it is authoritative for this PCB.
* The LED driver supply is switched by PB13; it is driven high in
  `keyboard_pre_init_kb` before the AW20216S is initialized.
* The IAP `RAM_MAGIC_LOCATION` (0x20001ffc) follows the C18 convention; the
  AP2D bootloader magic location is not confirmed in the RE notes.
* BLE slot command `0x21`/`0x22` vendor semantics are still TODO in the RE
  notes and are not implemented. Auto-reconnect to the last slot on power-up
  is not implemented; press `FN2+1..4` to select a slot after boot.
