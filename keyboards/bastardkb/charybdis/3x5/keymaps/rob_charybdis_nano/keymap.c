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
    LAYER_BASE = 0,
};

#define KC_Y_MEDIA LT(L_MEDIA, KC_Y)

// clang-format off
#define LAYOUT_LAYER_BASE                                                                         \
    KC_Q,               KC_H,               KC_O,                   KC_U,                   KC_X,                 /**/ KC_G,                   KC_C,                KC_R,               KC_F,               KC_Z,                   \
    KC_Y_MEDIA,         KC_I,               KC_E,                   KC_A,                   KC_DOT,               /**/ KC_D,                   KC_S,                KC_T,               KC_N,               KC_B,                   \
    LT(L_VSCODE, KC_J), KC_SLSH,            KC_COMM,                KC_K,                   KC_QUOT,              /**/ KC_W,                   KC_M,                KC_L,               KC_P,               LT(L_BLUETOOTH, KC_V),  \
                                            LT(L_FUNCTION, KC_ENT), LT(L_SHIFT, KC_BSPC),   LT(L_ARROWS, KC_TAB), /**/ LT(L_NUMPAD, KC_DEL),   LT(L_SYMBOLS, KC_SPC)

/** Convenience row shorthands. */
#define _______________DEAD_HALF_ROW_______________ XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
#define ______________HOME_ROW_GACS_L______________ XXXXXXX, KC_LALT, KC_LCTL, KC_LSFT, KC_LGUI
#define ______________HOME_ROW_GACS_R______________ KC_LGUI, KC_LSFT, KC_LCTL, KC_LALT, XXXXXXX


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
  [LAYER_BASE] = LAYOUT_wrapper(HOME_ROW_MOD_GACS(LAYOUT_LAYER_BASE))
};
// clang-format on
