/* Copyright 2022 @ Keychron (https://www.keychron.com)
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
#include "keychron_ft_common.h"

// clang-format off

enum layers{
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN
};

enum custom_keycodes {
    KC_LOPTN = QK_KB_2,
    KC_ROPTN,
    KC_LCMMD,
    KC_RCMMD,
    KC_SIRI,
    KC_TASK_VIEW,
    KC_FILE_EXPLORER,
    KC_SCREEN_SHOT,
    KC_CORTANA
};

#define KC_TASK KC_TASK_VIEW
#define KC_FLXP KC_FILE_EXPLORER
#define KC_SNAP KC_SCREEN_SHOT
#define KC_CRTA KC_CORTANA

typedef struct PACKED {
    uint8_t len;
    uint8_t keycode[3];
} key_combination_t;

bool     is_siri_active = false;
uint32_t siri_timer     = 0;
// clang-format off
key_combination_t key_comb_list[4] = {
    {2, {KC_LWIN, KC_TAB}},
    {2, {KC_LWIN, KC_E}},
    {3, {KC_LSFT, KC_LCMD, KC_4}},
    {2, {KC_LWIN, KC_C}}
};

static uint8_t mac_keycode[4] = {KC_LOPT, KC_ROPT, KC_LCMD, KC_RCMD};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_ansi_87(
        KC_ESC,             KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  BL_DOWN,  BL_UP,    KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  KC_SNAP,  KC_SIRI,  BL_STEP,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,     KC_BSPC,  KC_INS,   KC_HOME,  KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,    KC_BSLS,  KC_DEL,   KC_END,   KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,            KC_UP,
        KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, KC_ROPTN, MO(MAC_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [MAC_FN] = LAYOUT_ansi_87(
        _______,            KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   _______,  _______,  BL_TOGG,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,  _______,
        BL_TOGG,  BL_STEP,  BL_UP,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,  _______,
        _______,  _______,  BL_DOWN,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,
        _______,            _______,  _______,  _______,  _______,  _______,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,            _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,    _______,  _______,  _______,  _______),

    [WIN_BASE] = LAYOUT_ansi_87(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   KC_PSCR,  KC_CRTA,  BL_STEP,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,     KC_BSPC,  KC_INS,   KC_HOME,  KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,    KC_BSLS,  KC_DEL,   KC_END,   KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,            KC_UP,
        KC_LCTL,  KC_LCMD,  KC_LALT,                                KC_SPC,                                 KC_RALT,  KC_RWIN,  MO(WIN_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [WIN_FN] = LAYOUT_ansi_87(
        _______,            KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  BL_DOWN,   BL_UP,   KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  _______,  _______,  BL_TOGG,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,  _______,
        BL_TOGG,  BL_STEP,  BL_UP,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,  _______,
        _______,  _______,  BL_DOWN,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,
        _______,            _______,  _______,  _______,  _______,  _______,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,            _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,    _______,  _______,  _______,  _______),
};

// clang-format on

#if defined(LED_MATRIX_ENABLE) && defined(CAPS_LOCK_LED_INDEX) && defined(MAC_LOCK_LED_INDEX) && defined(WIN_LOCK_LED_INDEX)
extern void led_matrix_update_pwm_buffers(void);
#endif // LED_MATRIX_ENABLE

void housekeeping_task_user(void) {
    if (is_siri_active) {
        if (sync_timer_elapsed32(siri_timer) >= 500) {
            unregister_code(KC_LCMD);
            unregister_code(KC_SPACE);
            is_siri_active = false;
        }
    }

#if defined(LED_MATRIX_ENABLE) && defined(CAPS_LOCK_LED_INDEX) && defined(MAC_LOCK_LED_INDEX) && defined(WIN_LOCK_LED_INDEX)
    if (!led_matrix_is_enabled()) {
        if ((host_keyboard_led_state().caps_lock) && (default_layer_state == (1 << 0))) {
            led_matrix_set_value(CAPS_LOCK_LED_INDEX, 255);
            led_matrix_set_value(MAC_LOCK_LED_INDEX, 255);
            led_matrix_set_value(WIN_LOCK_LED_INDEX, 0);
        } else if ((!host_keyboard_led_state().caps_lock) && (default_layer_state == (1 << 0))) {
            led_matrix_set_value(CAPS_LOCK_LED_INDEX, 0);
            led_matrix_set_value(MAC_LOCK_LED_INDEX, 255);
            led_matrix_set_value(WIN_LOCK_LED_INDEX, 0);
        } else if ((host_keyboard_led_state().caps_lock) && (default_layer_state == (1 << 2))) {
            led_matrix_set_value(CAPS_LOCK_LED_INDEX, 255);
            led_matrix_set_value(MAC_LOCK_LED_INDEX, 0);
            led_matrix_set_value(WIN_LOCK_LED_INDEX, 255);
        } else if ((!host_keyboard_led_state().caps_lock) && (default_layer_state == (1 << 2))) {
            led_matrix_set_value(CAPS_LOCK_LED_INDEX, 0);
            led_matrix_set_value(MAC_LOCK_LED_INDEX, 0);
            led_matrix_set_value(WIN_LOCK_LED_INDEX, 255);
        }
        led_matrix_update_pwm_buffers();
    }
#endif // LED_MATRIX_ENABLE
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_keychron_ft(keycode, record)) {
        return false;
    }

    return true;
}

bool process_record_keychron(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_LOPTN:
        case KC_ROPTN:
        case KC_LCMMD:
        case KC_RCMMD:
            if (record->event.pressed) {
                register_code(mac_keycode[keycode - KC_LOPTN]);
            } else {
                unregister_code(mac_keycode[keycode - KC_LOPTN]);
            }
            return false; // Skip all further processing of this key
        case KC_SIRI:
            if (record->event.pressed) {
                if (!is_siri_active) {
                    is_siri_active = true;
                    register_code(KC_LCMD);
                    register_code(KC_SPACE);
                }
                siri_timer = sync_timer_read32();
            } else {
                // Do something else when release
            }
            return false; // Skip all further processing of this key
        case KC_TASK:
        case KC_FLXP:
        case KC_SNAP:
        case KC_CRTA:
            if (record->event.pressed) {
                for (uint8_t i = 0; i < key_comb_list[keycode - KC_TASK].len; i++) {
                    register_code(key_comb_list[keycode - KC_TASK].keycode[i]);
                }
            } else {
                for (uint8_t i = 0; i < key_comb_list[keycode - KC_TASK].len; i++) {
                    unregister_code(key_comb_list[keycode - KC_TASK].keycode[i]);
                }
            }
            return false; // Skip all further processing of this key
        default:
            return true; // Process all other keycodes normally
    }
}
