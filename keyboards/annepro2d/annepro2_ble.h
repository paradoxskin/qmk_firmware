/*
    Copyright (C) 2020 Yaotian Feng, Codetector<codetector@codetector.cn>
    SPDX-License-Identifier: Apache-2.0
*/

#pragma once

#include "annepro2d.h"
#include <stdbool.h>
#include <stdint.h>

void annepro2_ble_startup(void);
void annepro2_ble_broadcast(uint8_t port);
void annepro2_ble_connect(uint8_t port);
void annepro2_ble_disconnect(void);
void annepro2_ble_unpair(void);

/* Feed one byte received from the BLE UART into the frame parser. */
void annepro2_ble_consume(uint8_t byte);

/* Call periodically (from matrix_scan_kb) to time out half-received frames. */
void annepro2_ble_poll(void);

bool annepro2_ble_is_active(void);
