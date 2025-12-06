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

#define LINE_UART_TX B0
#define LINE_UART_RX B1

#define LINE_BT_UART_TX A2  // Master TX, BLE RX
#define LINE_BT_UART_RX A3  // Master RX, BLE TX

// Obins stock firmware has something similar to this already enabled, but disabled by default in QMK
#define PERMISSIVE_HOLD

// SPI configuration
#define SPI_DRIVER SPID0
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B4
#define SPI_MOSI_PAL_MODE 5
#define SPI_MISO_PIN B5
#define SPI_MISO_PAL_MODE 5

#define AW20216S_CS_PIN_1 B2
#define AW20216S_EN_PIN D3
#define AW20216S_LED_COUNT 1
#define AW20216S_SPI_MODE 0
#define AW20216S_SPI_DIVISOR 16
#define AW20216S_SCALING_MAX 150
#define AW20216S_GLOBAL_CURRENT_MAX 150
