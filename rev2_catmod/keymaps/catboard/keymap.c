/* Copyright 2020 tominabox1
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
#include <color.h>
//#include "print.h"

enum layers {
    _QUIRKY,
    _LOWER,
    _RAISE,
    _ADJUST,
    _SPECIAL,
    _LAYERS_END
};

enum keycodes {
    QUIRKY = SAFE_RANGE,
    LOWER,
    RAISE,
    ADJUST,
    SPECIAL,
    BSP_DEL,
    INJ,
    CLR_RST,
};

struct Timer_Ramp_s {
    uint16_t timer_data;
    const uint8_t ramp_start;
    const uint8_t ramp_end;
    uint8_t ramp;
    bool ramp_flip;
} const Timer_Ramp_default = {0, 100, RGBLIGHT_LIMIT_VAL, 100, true};
typedef struct Timer_Ramp_s Timer_Ramp;

Timer_Ramp timer_danger = Timer_Ramp_default;
Timer_Ramp timer_caps   = Timer_Ramp_default;
Timer_Ramp timer_layer_v  = Timer_Ramp_default;//{0, 100, 255, 100, true};
Timer_Ramp timer_layer_s  = {0, 140, 210, 140, true};

bool inj_active = false;
uint16_t timer_inj = 0;
uint16_t inj_keycode = KC_NO;
bool inj_first = true;
bool inj_holding = false;
const uint16_t INJ_INITIAL_DELAY = 400;
const uint16_t INJ_HOLDING_DELAY = 100;

uint8_t boot_effect = 1;
const uint8_t boot_effect_stop = 2;
const Timer_Ramp timer_boot_effect_default = {0, 0, 127, 0, true};
Timer_Ramp timer_boot_effect = timer_boot_effect_default;

/*
#define LOWER  FN_MO13
#define RAISE  FN_MO23
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Quirky (Modified qwerty)
 * ,-----------------------------------------------------------------------------------.
 * | Esc  |  Q   |  W   |  E   |  R   |  T   |  Y   |  U   |  I   |  O   |  P   | Del  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Tab  |  A   |  S   |  D   |  F   |  G   |  H   |  J   |  K   |  L   |  ;:  |  "'  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|  Z   |  X   |  C   |  V   |  B   |  N   |  M   |  ,<  |  .>  |  Up  |  /?  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl | Super| Alt  | Enter| Lower|    Space    | Raise| Inj  | Left | Down | Right|
 * `-----------------------------------------------------------------------------------'
 */
[_QUIRKY] = LAYOUT_ortho_4x12_1x2uC(
    KC_ESC , KC_Q   , KC_W   , KC_E        , KC_R   , KC_T   , KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_DEL ,
    KC_TAB , KC_A   , KC_S   , KC_D        , KC_F   , KC_G   , KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z   , KC_X   , KC_C        , KC_V   , KC_B   , KC_N   , KC_M   , KC_COMM, KC_DOT , KC_UP  , KC_SLSH,
    KC_LCTL, KC_LGUI, KC_LALT, MO(_SPECIAL), LOWER  , KC_SPC          , RAISE  , INJ    , KC_LEFT, KC_DOWN, KC_RGHT
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * | `~   |  1   |  2   |  3   |  4   |  5   |  6   |  7   |  8   |  9   |  0   | Ins  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | CAPS |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  -   |  =   |  {   |  }   |  \   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |      |      | WBak | PgUp | WFfw |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      | Home | PgDn | End  |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_ortho_4x12_1x2uC(
    KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   , KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_INS ,
    KC_CAPS, KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  , KC_MINS, KC_EQL , KC_LCBR, KC_RCBR, KC_BSLS,
    _______, KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 , _______, _______, KC_WBAK, KC_PGUP, KC_WFWD,
    _______, _______, _______, KC_MPLY, _______, KC_ENT          , _______, _______, KC_HOME, KC_PGDN, KC_END
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |  ~   |  !   |  @   |  #   |  $   |  %   |  ^   |  &   |  *   |  (   |  )   | Ins  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | CAPS |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  _   |  +   |  [   |  ]   |  |   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |ISO ~ |ISO | | Rwnd | Vol+ | Ffwd |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | Play | Prev | Vol- | Next |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_ortho_4x12_1x2uC(
    KC_TILD, KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_INS ,
    KC_CAPS, KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  , KC_UNDS, KC_PLUS, KC_LBRC, KC_RBRC, KC_PIPE,
    _______, KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 , _______, _______, KC_MRWD, KC_VOLU, KC_MFFD,
    _______, _______, _______, XXXXXXX, _______, BSP_DEL         , _______, _______, KC_MPRV, KC_VOLD, KC_MNXT
),

/* Adjust (Lower + Raise)
 * ,-----------------------------------------------------------------------------------.
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_ortho_4x12_1x2uC(
    QK_BOOT, DB_TOGG, _______, _______, _______, _______, RGB_TOG, RGB_MOD , RGB_SPI, RGB_HUI, RGB_SAI, RGB_VAI,
    CLR_RST, _______, _______, _______, _______, _______, RGB_M_P, RGB_RMOD, RGB_SPD, RGB_HUD, RGB_SAD, RGB_VAD,
    EE_CLR , _______, _______, _______, _______, _______, _______, _______ , _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______         , _______ , _______, _______, _______, _______
),

/* Special (Special key)
 * ,-----------------------------------------------------------------------------------.
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      | MUP  | MBT2 |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_SPECIAL] = LAYOUT_ortho_4x12_1x2uC(
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_ACL2, KC_BTN4, KC_WH_U, KC_BTN5,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DM_REC1, DM_REC2, XXXXXXX, KC_ACL1, KC_WH_L, KC_WH_D, KC_WH_R,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DM_PLY1, DM_PLY2, XXXXXXX, KC_ACL0, KC_BTN1, KC_MS_U, KC_BTN2,
    XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, DM_RSTP         , XXXXXXX, KC_BTN3, KC_MS_L, KC_MS_D, KC_MS_R
),

};


bool is_caps_on(void) {
    return host_keyboard_led_state().caps_lock;
}

uint16_t keycode_at_layer(const uint8_t layer, const uint8_t col, const uint8_t row)
{
    return keymap_key_to_keycode(layer, (keypos_t){col,row});
}

uint16_t keycode_at_layer_below(const uint8_t col, const uint8_t row)
{
    return keymap_key_to_keycode(
        layer_switch_get_layer((keypos_t){col,row}),
        (keypos_t){col,row}
    );
}

/*
HSV brighten(HSV hsv, uint8_t amount, bool limit_to_board_val) {
    // Brighten
    hsv.v += amount;

    // Enforce current default limit
    if (limit_to_board_val && hsv.v > rgb_matrix_get_val()) {
        hsv.v = rgb_matrix_get_val();
    }

    return hsv;
}
*/

void handle_timer(uint16_t *timer_data,
                  uint8_t timer_duration,
                  uint8_t *ramp,
                  uint8_t ramp_start,
                  uint8_t ramp_end,
                  bool *ramp_flip,
                  uint8_t ramp_step)
{
    if (timer_elapsed(*timer_data) > timer_duration) {
        // do something if 100ms or more have passed
        *ramp += *ramp_flip ? ramp_step : -ramp_step;
        if (*ramp <= ramp_start ||
            *ramp >= ramp_end) {
            *ramp_flip = !*ramp_flip;
        }
        *timer_data = timer_read();
    }
}

void handle_timers(void) {
    handle_timer(
        &timer_danger.timer_data, 10,
        &timer_danger.ramp,
        timer_danger.ramp_start, timer_danger.ramp_end,
        &timer_danger.ramp_flip, 1
    );
    handle_timer(
        &timer_caps.timer_data, 1,
        &timer_caps.ramp,
        timer_caps.ramp_start, timer_caps.ramp_end,
        &timer_caps.ramp_flip, 1
    );
    handle_timer(
        &timer_layer_v.timer_data, 10,
        &timer_layer_v.ramp,
        timer_layer_v.ramp_start, timer_layer_v.ramp_end,
        &timer_layer_v.ramp_flip, 1
    );
    handle_timer(
        &timer_layer_s.timer_data, 5,
        &timer_layer_s.ramp,
        timer_layer_s.ramp_start, timer_layer_s.ramp_end,
        &timer_layer_s.ramp_flip, 1
    );
}

void light_keycode(const uint8_t led_index,
                   const uint8_t col,
                   const uint8_t row,
                   const uint8_t layer)
{
    // Color definitions
    const HSV hsv_default = rgb_matrix_get_hsv();
    const HSV hsv_fkey = {12, 250, 120};//const HSV hsv_fkey = {31, 211, 120};
    const HSV hsv_num = {122, 205, 120};
    const HSV hsv_danger = {0, 255, timer_danger.ramp};
    const HSV hsv_caps_inactive = {248, 255, 91};
    const HSV hsv_caps_word_active = {248, 255, timer_caps.ramp}; //231, 163};
    const HSV hsv_caps_active = {248, 255, 180};
    const HSV hsv_mouse_btn = {12, 250, 120};


    // Read keycode from position
    const uint16_t keycode = keycode_at_layer(layer, col, row);

    HSV hsv = {0, 0, 0};
    if ((keycode >= KC_F1 && keycode <= KC_F12) || (keycode >= KC_F13 && keycode <= KC_F24)) {
        hsv = hsv_fkey;
    }
    else if (keycode == QK_BOOT || keycode == EE_CLR
        || keycode == CLR_RST || keycode == DB_TOGG) {
        hsv = hsv_danger;
    }
    else if (keycode == KC_TAB || keycode == KC_CAPS || keycode == CW_TOGG) {
        if (is_caps_on()) { 
            hsv = hsv_caps_active;
        }
        else if (is_caps_word_on()) {
            hsv = hsv_caps_word_active;
        } else {
            if (keycode != KC_TAB) {
                hsv = hsv_caps_inactive;
            } else {
                hsv = hsv_default;
            }
        }
    }
    else if ((keycode >= KC_1 && keycode <= KC_0) || (keycode >= KC_KP_SLASH && keycode <= KC_KP_DOT) || keycode == KC_KP_EQUAL || keycode == KC_KP_COMMA || keycode == KC_KP_EQUAL_AS400) {
        hsv = hsv_num;
    }
    else if (keycode >= KC_BTN1 && keycode <= KC_BTN5) {
        hsv = hsv_mouse_btn;
    }
    /*
    else if (keycode >= MACRO00 && keycode <= MACRO15) {
        continue;
    }
    else if (keycode >= USER00 && keycode <= USER15) {
        continue;
    }
    */
    /*
    else if (keycode >= LOWER && keycode <= ADJUST) {
        hsv = (HSV){rgb_matrix_get_hue(), rgb_matrix_get_sat(), 255};
        hsv = brighten(hsv, 255, false);
    }
    */
    else if (keycode == KC_TRNS) {
        // Resolve key below
        uint16_t keycode_below = keycode_at_layer_below(col, row);

        // Check if key below is a layer shift key
        if ((keycode_below >= LOWER && keycode_below <= ADJUST)
            || keycode_below == MO(_SPECIAL)) {
            // Brighten default board color
            hsv = (HSV){rgb_matrix_get_hue(), rgb_matrix_get_sat(), 255};
            //hsv = brighten(hsv, 255, false);
            hsv.v = timer_layer_v.ramp;
            hsv.s = timer_layer_s.ramp;
        }
        else {
            // Blank KC_TRNS
            rgb_matrix_set_color(led_index, 0, 0, 0);
            return;
        }
    }
    else if (keycode == KC_NO) {
        // Blank KC_NO
        rgb_matrix_set_color(led_index, 0, 0, 0);
        return;
    }
    else {
        if (layer != 0) hsv = hsv_default;
        else return;  // Skip unknown keycodes
    }

    // Enforce config limit
    if (hsv.v > RGBLIGHT_LIMIT_VAL) hsv.v = RGBLIGHT_LIMIT_VAL;

    // Apply color
    //rgb_matrix_sethsv(hsv.h, hsv.s, hsv.v);
    RGB rgb = hsv_to_rgb(hsv);
    rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (boot_effect > 0) {
        if (boot_effect > boot_effect_stop) {
            boot_effect = 0;
            rgb_matrix_reload_from_eeprom();
        } else {
            bool old_flip = timer_boot_effect.ramp_flip;
            handle_timer(
                &timer_boot_effect.timer_data, 2,
                &timer_boot_effect.ramp,
                timer_boot_effect.ramp_start, timer_boot_effect.ramp_end,
                &timer_boot_effect.ramp_flip, 1
            );
            if (old_flip != timer_boot_effect.ramp_flip) boot_effect++;

            rgb_matrix_sethsv_noeeprom(
                rgb_matrix_get_hue(),
                rgb_matrix_get_sat(),
                timer_boot_effect.ramp
            );
        }
        return true;
    }

    const uint8_t layer = get_highest_layer(layer_state);
    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            uint8_t led_index = g_led_config.matrix_co[row][col];

            // Skip
            if (led_index < led_min || led_index > led_max
                || led_index == NO_LED) continue;

            // Light up the keycode
            light_keycode(led_index, col, row, layer);
        }
    }

    handle_timers();
    return true;
}

void matrix_scan_user(void) {
    if (inj_active && inj_keycode != KC_NO) {
        uint16_t inj_delay = inj_holding ? INJ_HOLDING_DELAY : INJ_INITIAL_DELAY;
        if (inj_first || timer_elapsed(timer_inj) > inj_delay) {
            tap_code(inj_keycode);
            tap_code(KC_LEFT);

            if (inj_first) inj_first = false;
            else inj_holding = true;
            timer_inj = timer_read();
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint8_t saved_mods  = 0;
    uint16_t       tmp_keycode = keycode;

    // Filter out the actual keycode from MT and LT keys.
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) || (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) {
        tmp_keycode &= 0xFF;
    }

    switch (tmp_keycode) {
        case INJ:
            inj_active = record->event.pressed;
            // no return here
            break;
        case KC_TAB:
            if (!record->event.pressed) {
                if (is_caps_on()) tap_code(KC_CAPS);
                else if (is_caps_word_on()) caps_word_off();
                else return true;
            } else {
                if (is_caps_on() || is_caps_word_on()) return false;
                else return true;
            }
            return false;
            break;
        case KC_CAPS:
            if (record->event.pressed) {
                saved_mods = get_mods() & MOD_MASK_SHIFT;

                if (saved_mods) {  // One shift pressed
                    del_mods(saved_mods);
                    if (is_caps_word_on()) caps_word_off();
                    register_code(KC_CAPS);
                    add_mods(saved_mods);
                } else {
                    if (is_caps_on()) tap_code(KC_CAPS);
                    else caps_word_toggle();
                }
            } else {
                unregister_code(KC_CAPS);
            }
            return false;
            break;
        case BSP_DEL:
            if (record->event.pressed) {
                saved_mods = get_mods() & MOD_MASK_SHIFT;

                if (saved_mods == MOD_MASK_SHIFT) {  // Both shifts pressed
                    register_code(KC_DEL);
                } else if (saved_mods) {             // One shift pressed
                    del_mods(saved_mods);            // Remove any Shifts present
                    register_code(KC_DEL);
                    add_mods(saved_mods);            // Add shifts again
                } else {
                    register_code(KC_BSPC);
                }
            } else {
                unregister_code(KC_DEL);
                unregister_code(KC_BSPC);
            }
            return false;
            break;
        case CLR_RST:
            eeconfig_init();
            reset_keyboard();
            break;
        case QUIRKY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QUIRKY);
            }
            return false;
            break;
        case LOWER:
            if (record->event.pressed) {
                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
        case RAISE:
            if (record->event.pressed) {
                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
    }

    inj_holding = false;
    inj_first = true;
    if (inj_active && tmp_keycode != INJ && record->event.pressed) {
        inj_keycode = tmp_keycode;
        return false;
    } else {
        inj_keycode = KC_NO;
    }

    return true;
}

void keyboard_post_init_user(void) {
    rgb_matrix_enable_noeeprom();
    rgb_matrix_set_speed_noeeprom(127);
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR); // RGB_MATRIX_BREATHING

    boot_effect = 1;
    timer_boot_effect.ramp = timer_boot_effect_default.ramp;
}
