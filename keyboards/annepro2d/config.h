/*
 * Copyright (c) 2018 Charlie Waters
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

#pragma once

#include "pin_defs.h"

/*
 * BLE module UART (USART1 = SD1).
 * Per AP2D reverse engineering: BLE channel is PA4/PA5, AF6, 115200 8N1,
 * and the BLE UART peripheral is USART1 (base 0x40040000).
 * AP2D has no independent LED MCU, so there is no LED UART.
 */
#define LINE_BT_UART_TX A4  // Master TX, BLE RX
#define LINE_BT_UART_RX A5  // Master RX, BLE TX

// Obins stock firmware has something similar to this already enabled, but disabled by default in QMK
#define PERMISSIVE_HOLD

// SPI configuration for the direct-drive AW20216S RGB controller
#define SPI_DRIVER SPID0
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B4
#define SPI_MOSI_PAL_MODE 5
#define SPI_MISO_PIN B5
#define SPI_MISO_PAL_MODE 5

#define AW20216S_CS_PIN_1 B2
#define AW20216S_EN_PIN D3
#define AW20216S_SPI_MODE 0
#define AW20216S_SPI_DIVISOR 4
/*
 * LED supply enable. Official AP2D KEY 3.08 RGB init (0xF09A) configures
 * PB13 as a GPIO output and drives it HIGH to power the LED driver stage.
 */
#define LED_POWER_PIN B13
/*
 * Match the official AP2D KEY 3.08 RGB init (0x12302): global current and
 * scaling are set to 0xFF, and MIX_FUNCTION is left at its reset value
 * (official firmware never writes it, the stock QMK driver writes LPEN=1).
 */
#define AW20216S_GLOBAL_CURRENT_MAX 0xFF
#define AW20216S_SCALING_MAX 0xFF
#define AW20216S_MIX_FUNCTION 0x00
