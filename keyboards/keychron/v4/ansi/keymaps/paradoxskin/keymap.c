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


/* static declare */
#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)
#define QK_TAP_DANCE_GET_INDEX(kc) ((kc)&0xFF)
#define ACT_TD_TAP_HOLD(tap, hold) { \
    .fn = {NULL, fn_td_tap_hold_finished, fn_td_tap_hold_reset, fn_td_tap_hold_release}, \
    .user_data = (void *)&((td_tap_hold_t){(tap), (hold), 0}) \
}
#define ACT_TD_PAIR(idx, first, second, second_name, p_flag) { \
    .fn = {fn_td_pair_tap, fn_td_pair_finished, fn_td_pair_reset, NULL}, \
    .user_data = (void*)&((td_pair_t){(idx), (first), (second), (second_name), (p_flag)}) \
}

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

enum td_names {
    TD_TH_LC_F1, /* tap hold lctrl F1 */
    TD_PA_F_LSFT, /* pair first l_shift */
    TD_PA_S_SPC, /* pair second space */
};

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} td_tap_hold_t;

typedef struct {
    char idx;
    uint16_t first;
    uint16_t second;
    uint16_t second_name;
    char *flag;
} td_pair_t;

static char g_flag_0;


/* func declare */
void fn_td_tap_hold_finished(tap_dance_state_t *state, void *user_data);
void fn_td_tap_hold_reset(tap_dance_state_t *state, void *user_data);
void fn_td_tap_hold_release(tap_dance_state_t *state, void *user_data);
void fn_td_pair_tap(tap_dance_state_t *state, void *user_data);
void fn_td_pair_finished(tap_dance_state_t *state, void *user_data);
void fn_td_pair_reset(tap_dance_state_t *state, void *user_data);


/* before tap_dance */
bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    switch (keycode) {
        case TD(TD_TH_LC_F1):
            break;
    }
    return true;
}


/* tap_dance map */
tap_dance_action_t tap_dance_actions[] = {
    [TD_TH_LC_F1] = ACT_TD_TAP_HOLD(C(KC_F1), KC_LCTL),
    [TD_PA_F_LSFT] = ACT_TD_PAIR(0, KC_LSFT, MT(MOD_LALT, KC_SPC), TD_PA_S_SPC, &g_flag_0),
    [TD_PA_S_SPC] = ACT_TD_PAIR(1, KC_LSFT, MT(MOD_LALT, KC_SPC), TD_PA_S_SPC, &g_flag_0),
};


/* keymaps */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* BASE layers */
    [MAC_BASE] = LAYOUT_ansi_61(
        KC_ESC,  KC_1,    KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,  KC_Q,    KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_CAPS, KC_A,    KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
        KC_LSFT,          KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,
        KC_LCTL, KC_LOPT, KC_LCMD,                KC_SPC,                    KC_RCMD, XXXXXXX, XXXXXXX, KC_RCTL
    ),
    [WIN_BASE] = LAYOUT_ansi_61(
        KC_GRAVE,            KC_1,     KC_2, KC_3, KC_4, KC_5, KC_6,    KC_7, KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        LT(_FN1,KC_TAB),     KC_Q,     KC_W, KC_E, KC_R, KC_T, KC_Y,    KC_U, KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        MT(MOD_LCTL,KC_ESC), KC_A,     KC_S, KC_D, KC_F, KC_G, KC_H,    KC_J, KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
        TD(TD_PA_F_LSFT),              KC_Z, KC_X, KC_C, KC_V, KC_B,    KC_N, KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          RSFT_T(KC_UP),
        TD(TD_TH_LC_F1),     MO(_FN2), KC_LGUI,          TD(TD_PA_S_SPC),     KC_RALT, LT(_FN1,KC_LEFT), LT(_FN2,KC_DOWN), LT(_FNX,KC_RIGHT)
    ),

    /* FN layers */
    [_FN1] = LAYOUT_ansi_61(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_HUI, RGB_SAI, RGB_VAI,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_SPI,          RGB_MOD,
        KC_LSFT,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          RGB_TOG,
        XXXXXXX, XXXXXXX, KC_PSCR,                            KC_LSFT,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
    ),
    [_FN2] = LAYOUT_ansi_61(
        KC_WH_U, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,
        KC_WH_D, KC_WH_L, KC_WH_R, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_HOME, KC_END,  XXXXXXX,
        KC_VOLD, KC_VOLU, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,XXXXXXX, KC_PGUP,          KC_PSCR,
        KC_MPRV,          KC_MNXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PGDN,          XXXXXXX,
        KC_MUTE, XXXXXXX, KC_MPLY,                            XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
    ),
    [_FNX] = LAYOUT_ansi_61(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, TO(_TN1),
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, TO(_TN2),
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          TO(_TN3),
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, _______
    ),
    [_FNY] = LAYOUT_ansi_61(
        KC_ESC,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,      _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,               _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,               _______,
        _______, _______, _______,                            _______,                            _______, _______, TO(WIN_BASE), _______
    ),

    // TO layer
    [_TN1] = LAYOUT_ansi_61(
        KC_BSPC, KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,
        KC_7,    KC_8,        KC_9,           KC_KP_PLUS,  _______,  _______, _______, _______, KC_MS_U, _______, _______, _______, _______, _______,
        KC_4,    KC_5,        KC_6,           KC_KP_PLUS,  _______,  _______, _______, KC_MS_L, KC_MS_R, _______, _______, _______,          _______,
        KC_1,                 KC_2,           KC_3,        KC_ENTER, _______, _______, KC_BTN1, KC_MS_D, KC_BTN2, _______, _______,          _______,
        KC_0,    KC_0,        KC_DOT,                                         KC_ENTER,                           _______, _______, _______, LT(_FNY, KC_RIGHT)
    ),
    [_TN2] = LAYOUT_ansi_61(
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, LT(_FNY, KC_RIGHT)
    ),
    [_TN3] = LAYOUT_ansi_61(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        KC_P,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        KC_LCTL, _______, _______,                            KC_SPACE,                           _______, _______, _______, LT(_FNY, KC_RIGHT)
    ),
};
/* TEMPLATE:
    #0
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
        XXXXXXX,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
    #1
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, _______
*/

/* func define */
void fn_td_tap_hold_finished(tap_dance_state_t *state, void *user_data)
{
    td_tap_hold_t *tap_hold = (td_tap_hold_t *)user_data;
    /* only hold in */
    tap_hold->held = 1;
    register_code16(tap_hold->hold);
}

void fn_td_tap_hold_reset(tap_dance_state_t *state, void *user_data)
{
    td_tap_hold_t *tap_hold = (td_tap_hold_t *)user_data;
    if (!tap_hold->held) return;
    /* hold */
    unregister_code16(tap_hold->hold);
    tap_hold->held = 0;
}

void fn_td_tap_hold_release(tap_dance_state_t *state, void *user_data)
{
    td_tap_hold_t *tap_hold = (td_tap_hold_t *)user_data;
    /* speed up tap */
    if (!state->finished) {
        tap_code16(tap_hold->tap);
        state->finished = 1;
    }
}

void fn_td_pair_tap(tap_dance_state_t *state, void *user_data)
{
    td_pair_t *data = user_data;
    if (!data->idx) return; /* fisrt skip tap */
    /* second */
    if (*(data->flag)) {
        register_code16(data->first);
    } else {
        register_code16(data->second);
    }
    state->finished = 1; /* second skip finished */
}

void fn_td_pair_finished(tap_dance_state_t *state, void *user_data)
{
    /* only first will in */
    td_pair_t *data = user_data;
    if (state->interrupted
            && (state->interrupting_keycode == TD(data->second_name))) {
        *data->flag = 1;
    } else {
        register_code16(data->first);
    }
}

void fn_td_pair_reset(tap_dance_state_t *state, void *user_data)
{
    td_pair_t *data = user_data;
    if (data->idx) { /* second */
        if (*data->flag) {
            unregister_code16(data->first);
            *data->flag = 0;
        } else {
            unregister_code16(data->second);
        }
    } else { /* first */
        if (!*data->flag) {
            unregister_code16(data->first);
        }
    }
}
