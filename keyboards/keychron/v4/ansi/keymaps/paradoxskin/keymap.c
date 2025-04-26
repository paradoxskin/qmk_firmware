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


/* func declare */
bool fn_process_super_space(uint16_t keycode, keyrecord_t *record);
void fn_td_tap_hold_finished(tap_dance_state_t *state, void *user_data);
void fn_td_tap_hold_reset(tap_dance_state_t *state, void *user_data);
void fn_td_tap_hold_release(tap_dance_state_t *state, void *user_data);
bool pre_odd_space(uint16_t keycode, keyrecord_t *record);


/* static */
#define QK_TAP_DANCE_GET_INDEX(kc) ((kc)&0xFF)
#define ACT_TD_TAP_HOLD(once, irq_tap, tap, hold) { \
    .fn = {NULL, fn_td_tap_hold_finished, fn_td_tap_hold_reset, fn_td_tap_hold_release}, \
    .user_data = (void *)&((td_tap_hold_t){(once), (irq_tap), (tap), (hold), 0}) \
}
#define ODD_IDX(KC) ((KC) - ODD_BG - 1)

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
};

enum odd_keycodes {
    ODD_BG = SAFE_RANGE,
    KC_ODD_SPACE,
    ODD_ED
};

typedef bool (*odd_fn_t)(uint16_t keycode, keyrecord_t *record);

typedef struct {
    char once; /* hold process only tap once, 0/1 */
    char irq_tap; /* use tap for interrupted, 0/1 */
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} td_tap_hold_t;

typedef struct {
    odd_fn_t pre;
    odd_fn_t process;
    odd_fn_t post;
} odd_action_t;

tap_dance_action_t tap_dance_actions[] = {
    [TD_TH_LC_F1] = ACT_TD_TAP_HOLD(1, 0, C(KC_F1), KC_LCTL),
};

odd_action_t odd_actions[] = {
    [ODD_IDX(KC_ODD_SPACE)] = {pre_odd_space, NULL, NULL},
};

const uint16_t PROGMEM mkcbhp[] = {COMBO_END};
combo_t key_combos[] = {
    COMBO(mkcbhp, 0),
};

uint16_t g_odd_space_time;
uint16_t g_odd_space_type;


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
        KC_GRAVE,            KC_1,     KC_2, KC_3, KC_4, KC_5, KC_6,    KC_7, KC_8, KC_9,    KC_0,    KC_MINS,          KC_EQL,           KC_BSPC,
        LT(_FN1,KC_TAB),     KC_Q,     KC_W, KC_E, KC_R, KC_T, KC_Y,    KC_U, KC_I, KC_O,    KC_P,    KC_LBRC,          KC_RBRC,          KC_BSLS,
        MT(MOD_LCTL,KC_ESC), KC_A,     KC_S, KC_D, KC_F, KC_G, KC_H,    KC_J, KC_K, KC_L,    KC_SCLN, KC_QUOT,                            KC_ENT,
        KC_LSFT,                       KC_Z, KC_X, KC_C, KC_V, KC_B,    KC_N, KC_M, KC_COMM, KC_DOT,  KC_SLSH,                            RSFT_T(KC_UP),
        TD(TD_TH_LC_F1),     MO(_FN2), KC_LGUI,                KC_ODD_SPACE,                 KC_RALT, LT(_FN1,KC_LEFT), LT(_FN2,KC_DOWN), LT(_FNX,KC_RIGHT)
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
        KC_CAPS, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TO(_TN1),
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TO(_TN2),
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          TO(_TN3),
        XXXXXXX,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
    ),
    [_FNY] = LAYOUT_ansi_61(
        KC_ESC,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,               XXXXXXX,
        XXXXXXX,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,               XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX,                            XXXXXXX,                            XXXXXXX, XXXXXXX, TO(WIN_BASE), XXXXXXX
    ),

    // TO layer
    [_TN1] = LAYOUT_ansi_61(
        KC_BSPC, KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,
        KC_7,    KC_8,        KC_9,           KC_KP_PLUS,  _______,  _______, _______, _______, _______, _______, _______, KC_MS_U, _______, _______,
        KC_4,    KC_5,        KC_6,           KC_KP_PLUS,  _______,  _______, _______, _______, _______, _______, KC_MS_L, KC_MS_R,          KC_BTN2,
        KC_1,                 KC_2,           KC_3,        KC_ENTER, _______, _______, _______, _______, _______, KC_MS_D, _______,          _______,
        KC_DOT,  KC_0,        KC_0,                                           KC_ENTER,                           KC_BTN1, _______, _______, LT(_FNY, KC_RIGHT)
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
};


/* user custom */
bool pre_process_record_user(uint16_t keycode, keyrecord_t *record)
{
    switch (keycode) {
        case ODD_BG ... ODD_ED:
            if (odd_actions[ODD_IDX(keycode)].pre) {
                return odd_actions[ODD_IDX(keycode)].pre(keycode, record);
            }
            break;
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    switch (keycode) {
        case ODD_BG ... ODD_ED:
            if (odd_actions[ODD_IDX(keycode)].process) {
                return odd_actions[ODD_IDX(keycode)].process(keycode, record);
            }
            break;
        case KC_LSFT:
            if (!record->event.pressed) {
                g_odd_space_time = record->event.time;
                g_odd_space_type = keycode;
            }
    }
    return true;
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record)
{
    switch (keycode) {
        case ODD_BG ... ODD_ED:
            if (odd_actions[ODD_IDX(keycode)].post) {
                odd_actions[ODD_IDX(keycode)].post(keycode, record);
            }
            break;
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    rgb_matrix_set_color_all(0, 0, 0);
    switch (get_highest_layer(state)) {
        case _TN1:
            rgb_matrix_set_color(13, 0, 0, 5);
            break;
        case _TN2:
            rgb_matrix_set_color(27, 0, 5, 5);
            break;
        case _TN3:
            rgb_matrix_set_color(40, 0, 5, 5);
            break;
    }
    return state;
}


/* func define */
/**
 * MT: 有按键irq时，等待release，如果超过TAPPING_TERM还不松，判断为hold。如果提前松了，判断为tap
 * TD: 有按键irq时，立即处理finished，无法等待完整的窗口，想要达到MT的效果，需要其他处理逻辑的帮助
 */
void fn_td_tap_hold_finished(tap_dance_state_t *state, void *user_data)
{
    td_tap_hold_t *tap_hold = (td_tap_hold_t *)user_data;
    if (state->count > 1
            || (state->interrupted && tap_hold->irq_tap)) {
        tap_hold->held = tap_hold->tap;
    } else {
        tap_hold->held = tap_hold->hold;
    }
    register_code16(tap_hold->held);
}

void fn_td_tap_hold_reset(tap_dance_state_t *state, void *user_data)
{
    td_tap_hold_t *tap_hold = (td_tap_hold_t *)user_data;
    if (!tap_hold->held) return;
    /* hold */
    unregister_code16(tap_hold->held);
    tap_hold->held = 0;
}

void fn_td_tap_hold_release(tap_dance_state_t *state, void *user_data)
{
    td_tap_hold_t *tap_hold = (td_tap_hold_t *)user_data;
    /* speed up tap */
    if (!state->finished) {
        tap_code16(tap_hold->tap);
        if (tap_hold->once) state->finished = 1;
    }
}

bool pre_odd_space(uint16_t keycode, keyrecord_t *record)
{
    static uint16_t now_kc = MT(MOD_LALT, KC_SPC);
    if (record->event.pressed) {
        if (timer_elapsed(g_odd_space_time) < TAPPING_TERM) {
            switch (g_odd_space_type) {
                case KC_LSFT:
                    now_kc = KC_LSFT;
                    rgb_matrix_set_color(41, 0, 5, 5);
                    break;
            }
        } else {
            now_kc = MT(MOD_LALT, KC_SPC);
        }
    } else {
        rgb_matrix_set_color(41, 0, 0, 0);
    }
    record->keycode = now_kc;
    return true;
}


/* freezone */
#if 0
#define ACT_TD_PAIR(idx, first, second, second_name, p_flag) { \
    .fn = {fn_td_pair_tap, fn_td_pair_finished, fn_td_pair_reset, NULL}, \
    .user_data = (void*)&((td_pair_t){(idx), (first), (second), (second_name), (p_flag)}) \
}
typedef struct {
    char idx;
    uint16_t first;
    uint16_t second;
    uint16_t second_name;
    char *flag;
} td_pair_t;
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
#endif
