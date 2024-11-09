/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
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


enum charybdis_keymap_layers {
    L_BASE = 0,
};

#define KC_Y_MEDIA LT(L_MEDIA, KC_Y)
#define KC_J_VSCODE LT(L_VSCODE, KC_J)
#define KC_ENT_FUNCTION LT(L_FUNCTION, KC_ENT)
#define KC_BSPC_SHIFT LT(L_SHIFT, KC_BSPC)
#define KC_TAB_ARROWS LT(L_ARROWS, KC_TAB)
#define KC_DEL_NUMPAD LT(L_NUMPAD, KC_DEL)
#define KC_SPC_SYMBOLS LT(L_SYMBOLS, KC_SPC)
#define KC_V_BLUETOOTH LT(L_BLUETOOTH, KC_SPC)

// clang-format off
#define L_BASE \
  KC_Q,         KC_H,     KC_O,             KC_U,           KC_X,           /**/  KC_G,           KC_C,           KC_R,   KC_F,   KC_Z,           \
  KC_Y_MEDIA,   KC_I,     KC_E,             KC_A,           KC_DOT,         /**/  KC_D,           KC_S,           KC_T,   KC_N,   KC_B,           \
  KC_J_VSCODE,  KC_SLSH,  KC_COMM,          KC_K,           KC_QUOT,        /**/  KC_W,           KC_M,           KC_L,   KC_P,   KC_V_BLUETOOTH, \
                          KC_ENT_FUNCTION,  KC_BSPC_SHIFT,  KC_TAB_ARROWS,  /**/  KC_DEL_NUMPAD,  KC_SPC_SYMBOLS

/** Convenience row shorthands. */
#define _______________DEAD_HALF_ROW_______________ XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
#define ______________HOME_ROW_GACS_L______________ XXXXXXX, KC_LALT, KC_LCTL, KC_LSFT, KC_LGUI
#define ______________HOME_ROW_GACS_R______________ KC_LGUI, KC_LSFT, KC_LCTL, KC_LALT, XXXXXXX

#define L_SHIFT                                                                          \
  LSFT(KC_Q), LSFT(KC_H), LSFT(KC_O), LSFT(KC_U), LSFT(KC_X),       LSFT(KC_G), LSFT(KC_C), LSFT(KC_R), LSFT(KC_F), LSFT(KC_Z), \
  LSFT(KC_Y), LSFT(KC_I), LSFT(KC_E), LSFT(KC_A), KC_AT,            LSFT(KC_D), LSFT(KC_S), LSFT(KC_T), LSFT(KC_N), LSFT(KC_B), \
  LSFT(KC_J), KC_QUES,    KC_EXLM,    LSFT(KC_K), KC_GRV,           LSFT(KC_W), LSFT(KC_M), LSFT(KC_L), LSFT(KC_P), LSFT(KC_V), \
                          _______,    _______,    _______,          _______,    _______

#define L_FUNCTION \
  KC_ESC,            _______,       /* LCTL(LSFT(KC_T)), LCTL(KC_W), */    LCTL(KC_T),  LCTL(KC_S),       KC_F10, KC_F7,    KC_F8, KC_F9,   _______,  _______,  \
  LCTL(KC_SLSH),     LCTL(KC_Z),    /* LCTL(KC_X),       LCTL(KC_C), */    /* LCTL(KC_V), */  LCTL(LSFT(KC_Z)),       KC_F11, _______,  KC_F1, KC_F2,   KC_F3,    _______,  \
LALT(LSFT(KC_A)),  LCTL(KC_F),    LCTL(LSFT(KC_F)), _______,       _______,                               KC_F12, KC_F4,    KC_F5,    KC_F6, _______, _______,            \
                                      _______,        _______,       _______,     _______,        _______




/**
 * \brief Add Home Row mod to a layout.
 *
 * Expects a 10-key per row layout.  Adds support for GACS (Gui, Alt, Ctl, Shift)
 * home row.  The layout passed in parameter must contain at least 20 keycodes.
 *
 * This is meant to be used with `LAYER_ALPHAS_QWERTY` defined above, eg.:
 *
 *     HOME_ROW_MOD_GACS(LAYER_ALPHAS_QWERTY)
 */
#define _HOME_ROW_MOD_GACS(                                            \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
             L10, LALT_T(L11), LCTL_T(L12), LSFT_T(L13), LGUI_T(L14),  \
    wRGUI_T(R15), RSFT_T(R16), RCTL_T(R17), LALT_T(R18),         R19,  \
      __VA_ARGS__
#define HOME_ROW_MOD_GACS(...) _HOME_ROW_MOD_GACS(__VA_ARGS__)


#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_wrapper(HOME_ROW_MOD_GACS(L_BASE))
};
// clang-format on
