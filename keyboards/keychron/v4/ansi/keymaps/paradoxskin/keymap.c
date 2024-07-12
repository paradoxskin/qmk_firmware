/* Copyright 2021 @ Keychron (https://www.keychron.com)
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

#include QMK_KEYBOARD_H

// clang-format off

enum layers {
    MAC_BASE,
    WIN_BASE,
    _FN1,
    _FN2,
    _FNX,
    _FNY,
    _TN1,
    _TN2,
    _TN3,
};

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)
#define QK_TAP_DANCE_GET_INDEX(kc) ((kc)&0xFF)

enum {
    TD_TH_CE_C,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;

    switch (keycode) {
        case TD(TD_TH_CE_C):  // list all tap dance keycodes with tap-hold configurations
            action = &tap_dance_actions[QK_TAP_DANCE_GET_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
    }
    return true;
}

//#include "print.h"

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        //uprintf("F%d %d %d %d\n", state->count, state->interrupted, state->pressed, state->finished);
        if (state->count > 0) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        }
        //else {
        //    register_code16(tap_hold->tap);
        //    tap_hold->held = tap_hold->tap;
        //}
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;
    //uprintf("R%d %d %d %d\n", state->count, state->interrupted, state->pressed, state->finished);

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) \
    { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }

tap_dance_action_t tap_dance_actions[] = {
    [TD_TH_CE_C] = ACTION_TAP_DANCE_TAP_HOLD(C(KC_E), KC_LCTL)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_ansi_61(
        KC_ESC,   KC_1,     KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,
        KC_TAB,   KC_Q,     KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,
        KC_CAPS,  KC_A,     KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,  KC_QUOT,            KC_ENT,
        KC_LSFT,            KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,   KC_SLSH,            KC_RSFT,
        KC_LCTL,  KC_LOPT,  KC_LCMD,                            KC_SPC,                             KC_RCMD,  XXXXXXX,  XXXXXXX,  KC_RCTL
    ),

    [WIN_BASE] = LAYOUT_ansi_61(
        KC_GRAVE,         KC_1,     KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,
        KC_TAB,           KC_Q,     KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,
        LT(_FN1,KC_ESC),  KC_A,     KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,  KC_QUOT,            KC_ENT,
        KC_LSFT,                    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,   KC_SLSH,            RSFT_T(KC_UP),
        TD(TD_TH_CE_C),  MO(_FN2), KC_LGUI,                   MT(MOD_LALT,KC_SPC),       KC_RALT, LT(_FN1,KC_LEFT),  LT(_FN2,KC_DOWN),   LT(_FNX,KC_RIGHT)
    ),

    [_FN1] = LAYOUT_ansi_61(
        XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
        XXXXXXX, KC_BTN1,  KC_MS_U, KC_BTN2, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  RGB_HUI,  RGB_SAI,  RGB_VAI,
        XXXXXXX, KC_MS_L,  KC_MS_D, KC_MS_R, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  RGB_SPI,            RGB_MOD,
        KC_LSFT,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,            RGB_TOG,
        XXXXXXX, XXXXXXX,  KC_PSCR,                            XXXXXXX,                            XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX
    ),

    [_FN2] = LAYOUT_ansi_61(
        KC_WH_U, KC_F1,    KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,   KC_DEL,
        KC_WH_D, KC_WH_L,  KC_WH_R, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_HOME,  KC_END,   XXXXXXX,
        KC_VOLD, KC_VOLU,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,XXXXXXX,  KC_PGUP,            KC_PSCR,
        KC_MPRV,           KC_MNXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_PGDN,            XXXXXXX,
        KC_MUTE, XXXXXXX,  KC_MPLY,                            XXXXXXX,                            XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX
    ),

    [_FNX] = LAYOUT_ansi_61(
        KC_CAPS,  _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  TO(_TN1),
        _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  TO(_TN2),
        _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            TO(_TN3),
        _______,            _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        _______,  _______,  _______,                            _______,                            _______,  _______,  _______,  _______
    ),

    [_FNY] = LAYOUT_ansi_61(
        _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,       _______,
        _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,       _______,
        KC_ESC,   _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,                 _______,
        _______,            _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,                 _______,
        _______,  _______,  _______,                            _______,                            _______,  _______,  TO(WIN_BASE),  _______
    ),

    // TO layer
    [_TN1] = LAYOUT_ansi_61(
        KC_TAB,  KC_ENT,   KC_ENT,  _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,
        KC_7,    KC_8,     KC_9,    _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,
        KC_4,    KC_5,     KC_6,    _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        KC_1,              KC_2,    KC_3,    _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        KC_0,    KC_0,     KC_0,                               _______,                            _______,  _______,  _______,  LT(_FNY, KC_RIGHT)
    ),

    [_TN2] = LAYOUT_ansi_61(
        _______, KC_F1,    KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,   _______,
        _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,
        _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        _______,           _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        _______, _______,  _______,                            _______,                            _______,  _______,  _______,  LT(_FNY, KC_RIGHT)
    ),
    [_TN3] = LAYOUT_ansi_61(
        _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,
        _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______,  _______,  _______,
        KC_P,    _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        _______,           _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        KC_LCTL, _______,  _______,                            KC_SPACE,                           _______,  _______,  _______,  LT(_FNY, KC_RIGHT)
    ),
};
/* TEMPLATE:
    #0
        XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
        XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
        XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,            XXXXXXX,
        XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,            XXXXXXX,
        XXXXXXX, XXXXXXX,  XXXXXXX,                            XXXXXXX,                            XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX
    #1
        _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,
        _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,
        _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        _______,           _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        _______, _______,  _______,                            _______,                            _______,  _______,  _______,  _______
*/
