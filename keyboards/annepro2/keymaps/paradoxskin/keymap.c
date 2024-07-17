#include QMK_KEYBOARD_H

enum anne_pro_layers {
  _BASE_LAYER,
  _FN1,
  _FN2,
  _FNX,
  _FNY,
  _TN1,
  _TN2,
  _TN3,
};

// Key symbols are based on QMK. Use them to remap your keyboard
/*
* Layer _BASE_LAYER
* ,-----------------------------------------------------------------------------------------.
* | esc |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |    Bksp   |
* |-----------------------------------------------------------------------------------------+
* | Tab    |  q  |  w  |  e  |  r  |  t  |  y  |  u  |  i  |  o  |  p  |  [  |  ]  |   \    |
* |-----------------------------------------------------------------------------------------+
* | Caps    |  a  |  s  |  d  |  f  |  g  |  h  |  j  |  k  |  l  |  ;  |  '  |    Enter    |
* |-----------------------------------------------------------------------------------------+
* | Shift      |  z  |  x  |  c  |  v  |  b  |  n  |  m  |  ,  |  .  |  /  |    Shift       |
* |-----------------------------------------------------------------------------------------+
* | Ctrl  |  L1   |  Alt  |               space             |  Alt  |  FN1  |  FN2  | Ctrl  |
* \-----------------------------------------------------------------------------------------/
* Layer TAP in _BASE_LAYER
* ,-----------------------------------------------------------------------------------------.
* |     |     |     |     |     |     |     |     |     |     |     |     |     |           |
* |-----------------------------------------------------------------------------------------+
* |        |     |     |     |     |     |     |     |     |     |     |     |     |        |
* |-----------------------------------------------------------------------------------------+
* |         |     |     |     |     |     |     |     |     |     |     |     |             |
* |-----------------------------------------------------------------------------------------+
* |            |     |     |     |     |     |     |     |     |     |     |       UP       |
* |-----------------------------------------------------------------------------------------+
* |       |       |       |                                 |       |  LEFT | DOWN  | RIGHT |
* \-----------------------------------------------------------------------------------------/
*/
enum {
    TD_TH_CE_F1
};

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

#define QK_TAP_DANCE_GET_INDEX(kc) ((kc)&0xFF)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;

    switch (keycode) {
        case TD(TD_TH_CE_F1):
            action = &tap_dance_actions[QK_TAP_DANCE_GET_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
    }
    return true;
}

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count > 0) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        }
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;
    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) \
    { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }

tap_dance_action_t tap_dance_actions[] = {
    [TD_TH_CE_F1] = ACTION_TAP_DANCE_TAP_HOLD(C(KC_F1), KC_LCTL)
};

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE_LAYER] = LAYOUT_60_ansi( /* Base */
        KC_GRAVE,         KC_1,     KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,
        KC_TAB,           KC_Q,     KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,
        LT(_FN1,KC_ESC),  KC_A,     KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,  KC_QUOT,            KC_ENT,
        KC_LSFT,                    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,   KC_SLSH,            RSFT_T(KC_UP),
        TD(TD_TH_CE_F1),      MO(_FN2),          KC_LGUI, MT(MOD_LALT,KC_SPC),                KC_RALT, LT(_FN1,KC_LEFT),  LT(_FN2,KC_DOWN),   LT(_FNX,KC_RIGHT)
    ),

    [_FN1] = LAYOUT_60_ansi( /* Base */
        KC_AP2_BT_UNPAIR, KC_AP2_BT1,  KC_AP2_BT2, KC_AP2_BT3, KC_AP2_BT4, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
        XXXXXXX,          KC_BTN1,     KC_MS_U,    KC_BTN2,    XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  RGB_HUI,  RGB_SAI,  RGB_VAI,
        XXXXXXX,          KC_MS_L,     KC_MS_D,    KC_MS_R,    XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  RGB_SPI,            RGB_MOD,
        KC_LSFT,                       XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,            RGB_TOG,
        XXXXXXX,          XXXXXXX,     KC_PSCR,                                     XXXXXXX,                            XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX
    ),

    [_FN2] = LAYOUT_60_ansi( /* Base */
        KC_WH_U, KC_F1,    KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,   KC_DEL,
        KC_WH_D, KC_WH_L,  KC_WH_R, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_HOME,  KC_END,   XXXXXXX,
        KC_VOLD, KC_VOLU,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,XXXXXXX,  KC_PGUP,            KC_PSCR,
        KC_MPRV,           KC_MNXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_PGDN,            XXXXXXX,
        KC_MUTE, XXXXXXX,  KC_MPLY,                            XXXXXXX,                            XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX
    ),

    [_FNX] = LAYOUT_60_ansi(
        KC_CAPS,  _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  TO(_TN1),
        _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  TO(_TN2),
        _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            TO(_TN3),
        _______,            _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        _______,  _______,  _______,                            _______,                            _______,  _______,  _______,  _______
    ),

    [_FNY] = LAYOUT_60_ansi(
        KC_ESC,   _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,
        _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,
        _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        _______,            _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        _______,  _______,  _______,                            _______,                            _______,  _______,  TO(_BASE_LAYER),  _______
    ),

    [_TN1] = LAYOUT_60_ansi(
        KC_TAB,  KC_ENT,   KC_ENT,  _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,
        KC_7,    KC_8,     KC_9,    _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,
        KC_4,    KC_5,     KC_6,    _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        KC_1,              KC_2,    KC_3,    _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        KC_0,    KC_0,     KC_0,                               _______,                            _______,  _______,  _______,  LT(_FNY, KC_RIGHT)
    ),

    [_TN2] = LAYOUT_60_ansi(
        _______, KC_F1,    KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,   _______,
        _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,
        _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        _______,           _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        _______, _______,  _______,                            _______,                            _______,  _______,  _______,  LT(_FNY, KC_RIGHT)
    ),

    [_TN3] = LAYOUT_60_ansi(
        _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,  _______,
        _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______,  _______,  _______,
        KC_P,    _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        _______,           _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,            _______,
        KC_LCTL, _______,  _______,                            KC_SPACE,                           _______,  _______,  _______,  LT(_FNY, KC_RIGHT)
    ),
};
const uint16_t keymaps_size = sizeof(keymaps);


void matrix_init_user(void) {

}

void matrix_scan_user(void) {
}

// Code to run after initializing the keyboard
void keyboard_post_init_user(void) {
    // Here are two common functions that you can use. For more LED functions, refer to the file "qmk_ap2_led.h"

    // annepro2-shine disables LEDs by default. Uncomment this function to enable them at startup.
    // annepro2LedEnable();

    // Additionally, it also chooses the first LED profile by default. Refer to the "profiles" array in main.c in
    // annepro2-shine to see the order. Replace "i" with the index of your preferred profile. (i.e the RED profile is index 0)
    // annepro2LedSetProfile(i);
}

layer_state_t layer_state_set_user(layer_state_t layer) {
    return layer;
}
