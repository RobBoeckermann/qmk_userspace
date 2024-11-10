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
#include "custom_keycodes.h"
#include "tap_dance.h"

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

// TODO:
// DPI configs and buttons https://docs.bastardkb.com/fw/charybdis-features.html
// Dynamic macros https://docs.qmk.fm/keycodes#dynamic-macros


enum charybdis_keymap_layers {
    L_BASE = 0,
    L_SHIFT,
    L_FUNCTION,
    L_ARROWS,
    L_SYMBOLS,
    L_NUMPAD,
    L_VSCODE,
    L_WINDOWS,
    L_MEDIA,
    L_POINTER,
};

// Automatically enable sniping-mode on the pointer layer.
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER L_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#define _L_PTR(KC) LT(L_POINTER, KC)

// #define KC_Y_MEDIA LT(L_MEDIA, KC_Y)
// #define KC_J_VSCODE LT(L_VSCODE, KC_J)
// #define KC_ENT_FUNCTION LT(L_FUNCTION, KC_ENT)
// #define KC_BSPC_SHIFT LT(L_SHIFT, KC_BSPC)
// #define KC_TAB_ARROWS LT(L_ARROWS, KC_TAB)
// #define KC_DEL_NUMPAD LT(L_NUMPAD, KC_DEL)
// #define KC_SPC_SYMBOLS LT(L_SYMBOLS, KC_SPC)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

/** Convenience row shorthands. */
#define _______________DEAD_HALF_ROW_______________ XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
#define ______________HOME_ROW_GACS_L______________ XXXXXXX, KC_LALT, KC_LCTL, KC_LSFT, KC_LGUI
#define ______________HOME_ROW_GACS_R______________ KC_LGUI, KC_LSFT, KC_LCTL, KC_LALT, XXXXXXX

// clang-format off

// -----------------------------------------------------------------------------------------
// |   q   |  h  |  o  |  u  |  x  |                                        |  g  |  c  |  r  |  f  |  z  |
// |  y/-  |  i  |  e  |  a  |  .  |                                        |  d  |  s  |  t  |  n  | b/; |
// |   j   |  /  |  ,  |  k  |  '  |                                        |  w  |  m  |  l  |  p  |  v  |
//       | ENTER | BACKSPACE | TAB |                                        | DELETE | SPACE |
#define LAYER_BASE \
KC_Q,         KC_H,     KC_O,             KC_U,           KC_X,           /**/  KC_G,           KC_C,           KC_R,   KC_F,   KC_Z,           \
/* KC_Y_MEDIA */KC_Y,   KC_I,     KC_E,             KC_A,           KC_DOT,         /**/  KC_D,           KC_S,           KC_T,   KC_N,   KC_B, \
/* KC_J_VSCODE */KC_J,  KC_SLSH,  KC_COMM,          KC_K,           KC_QUOT,        /**/  KC_W,           KC_M,           KC_L,   KC_P,   KC_V, \
KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
//                          KC_ENT_FUNCTION,  KC_BSPC_SHIFT,  KC_TAB_ARROWS,  /**/  KC_DEL_NUMPAD,  KC_SPC_SYMBOLS

// -----------------------------------------------------------------------------------------
// |  Q  |  H  |  O  |  U  |  X  |                                        |  G  |  C  |  R  |  F  |  Z  |
// |  Y  |  I  |  E  |  A  |  @  |                                        |  D  |  S  |  T  |  N  |  B  |
// |  J  |  ?  |  !  |  K  |  `  |                                        |  W  |  M  |  L  |  P  |  V  |
//             |     |  *  |     |                                        |     | LOCK |
#define LAYER_SHIFT \
LSFT(KC_Q), LSFT(KC_H), LSFT(KC_O), LSFT(KC_U), LSFT(KC_X),       LSFT(KC_G), LSFT(KC_C), LSFT(KC_R), LSFT(KC_F), LSFT(KC_Z), \
LSFT(KC_Y), LSFT(KC_I), LSFT(KC_E), LSFT(KC_A), KC_AT,            LSFT(KC_D), LSFT(KC_S), LSFT(KC_T), LSFT(KC_N), LSFT(KC_B), \
LSFT(KC_J), KC_QUES,    KC_EXLM,    LSFT(KC_K), KC_GRV,           LSFT(KC_W), LSFT(KC_M), LSFT(KC_L), LSFT(KC_P), LSFT(KC_V), \
                        _______,    _______,    _______,          _______,    QK_LOCK

// -----------------------------------------------------------------------------------------
// |ESC            |       |CLOSE TAB              |NEW TAB        |SAVE        |               |F10|F7 |F8 |F9 |   |
// |LINE COMMENT   |UNDO   |COPY                   |PASTE          |REDO        |               |F11|   |F1 |F2 |F3 |
// |BLOCK COMMENT  |FIND   |FIND IN SOLUTION       |               |            |               |F12|F4 |F5 |F6 |   |
//                         |           *           |               |            |               |   |   |
#define LAYER_FUNCTION \
KC_ESC,            _______,     TD_TAB_CLOSE_REOPEN,    LCTL(KC_T), LCTL(KC_S),       /**/ KC_F10,     KC_F7,    KC_F8,    KC_F9,  _______,  \
LCTL(KC_SLSH),     LCTL(KC_Z),  TD_COPY,                TD_PASTE,   LCTL(LSFT(KC_Z)), /**/ KC_F11,     _______,  KC_F1,    KC_F2,  KC_F3,    \
LALT(LSFT(KC_A)),  LCTL(KC_F),  LCTL(LSFT(KC_F)),       _______,    _______,          /**/ KC_F12,     KC_F4,    KC_F5,    KC_F6,  _______,  \
                                _______,                _______,    _______,          /**/ _______,    _______

// -----------------------------------------------------------------------------------------
// | RESET      |                | SELECT WORD |         |          |                        |    |HOME    |UP     |END       |       |
// | SCREENSHOT | SHIFT          | CTRL        | ALT     |          |                        |    |LEFT    |DOWN   |RIGHT     |INSERT |
// | RESTART    | DEBUG/CONTINUE | STEP        | STEP IN | STEP OUT |                        |    |PAGE UP |       |PAGE DOWN |       |
//                               |             | BOOT    |     *    |                        |    |        |
#define LAYER_ARROWS \
QK_RBT,             _______,    SELECT_WORD,    _______,    _______,        /**/ _______,  KC_HOME,    KC_UP,      KC_END,         _______, \
LGUI(LSFT(KC_S)),   KC_LSFT,    KC_LCTL,        KC_LALT,    _______,        /**/ _______,  KC_LEFT,    KC_DOWN,    KC_RIGHT,       KC_INSERT, \
LSFT(LCTL(KC_F5)),  KC_F5,      KC_F10,         KC_F11,     LSFT(KC_F11),   /**/ _______,  KC_PAGE_UP, _______,    KC_PAGE_DOWN,   _______, \
                                _______,        QK_BOOT,    _______,        /**/ _______,  _______

// -----------------------------------------------------------------------------------------
// |     |  <  |  $  |  >  |     |                        |     |  [  |  _  |  ]  |     |
// |  \  |  (  |  "  |  )  |  #  |                        |  %  |  {  |  =  |  }  |  |  |
// |     |  :  |  *  |  +  |     |                        |     |  &  |  ^  |  ~  |     |
//             |     |     |     |                        |     |  *  |
#define LAYER_SYMBOLS \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
                        _______,    _______,    _______, /**/ _______,  _______

// -----------------------------------------------------------------------------------------
// |    | (  | +  | )  |    |                        | PERIOD      |  7  |  8  |  9  | RESET |
// | =  | /  | -  | *  | .  |                        | BACKSPACE   |  0  |  1  |  2  |  3    |
// |    | :  |    |    |    |                        | COLON       |  4  |  5  |  6  |       |
//           |    |    |    |                        |      *      | BOOT|
#define LAYER_NUMPAD \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
                        _______,    _______,    _______, /**/ _______,  _______

// -----------------------------------------------------------------------------------------
// |     |peef def          |references     |implementations        |    |                        |    |    |    |    |    |
// |     |back              |def            |forward                |    |                        |    |    |    |    |    |
// |  *  |prev bookmark     |bookmark       |next bookmark          |    |                        |    |    |    |    |    |
//                          |               |                       |    |                        |    |    |
#define LAYER_VSCODE \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
                        _______,    _______,    _______, /**/ _______,  _______

// -----------------------------------------------------------------------------------------
// |     |VD left           |window up      |VD right       |new VD         |                        |    |    | BRIGHTNESS UP    |    |    |
// |  *  |window left       |window down    |window right   |exit VD        |                        |    |    | BRIGHTNESS DOWN  |    |    |
// |     |                  |               |               |               |                        |    |    |    |    |    |
//                          |monitor left   |task view      |monitor right  |                        |    |    |
#define LAYER_WINDOWS \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
                        _______,    _______,    _______, /**/ _______,  _______

// -----------------------------------------------------------------------------------------
// |   *   |VOL DOWN / PREV  |VOL UP / NEXT  |PLAY PAUSE / MUTE  |    |                        |    |    |    |    |    |
// |       |                 |               |                   |    |                        |    |    |    |    |    |
// |       |                 |               |                   |    |                        |    |    |    |    |    |
//                           |               |                   |    |                        |    |    |
#define LAYER_MEDIA \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
                        _______,    _______,    _______, /**/ _______,  _______

// -----------------------------------------------------------------------------------------
// |    |    |    |    |    | /**/ |    |    |    |    |    |
// |    |    |    |    |    | /**/ |    |    |    |    |    |
// |    |    |    |    |    | /**/ |    |    |    |    |    |
//           |    |    |    | /**/ |    |    |
#define LAYER_TEMPLATE \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
                        _______,    _______,    _______, /**/ _______,  _______


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
     RGUI_T(R15), RSFT_T(R16), RCTL_T(R17), LALT_T(R18),         R19,  \
      __VA_ARGS__
#define HOME_ROW_MOD_GACS(...) _HOME_ROW_MOD_GACS(__VA_ARGS__)

/**
 * \brief Add pointer layer keys to a layout.
 *
 * Expects a 10-key per row layout.  The layout passed in parameter must contain
 * at least 30 keycodes.
 *
 * This is meant to be used with `LAYER_ALPHAS_QWERTY` defined above, eg.:
 *
 *     POINTER_MOD(LAYER_ALPHAS_QWERTY)
 */
#define _POINTER_MOD(                                                  \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    L20, L21, L22, L23, L24, R25, R26, R27, R28, R29,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
             L10,         L11,         L12,         L13,         L14,  \
             R15,         R16,         R17,         R18,         R19,  \
      _L_PTR(L20),        L21,         L22,         L23,         L24,  \
             R25,         R26,         R27,         R28,  _L_PTR(R29), \
      __VA_ARGS__
#define POINTER_MOD(...) _POINTER_MOD(__VA_ARGS__)




#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_BASE] = LAYOUT_wrapper(POINTER_MOD(HOME_ROW_MOD_GACS(LAYER_BASE))),
  [L_SHIFT] = LAYOUT_wrapper(LAYER_SHIFT),
  [L_FUNCTION] = LAYOUT_wrapper(LAYER_FUNCTION),
  [L_ARROWS] = LAYOUT_wrapper(LAYER_ARROWS),
  [L_SYMBOLS] = LAYOUT_wrapper(LAYER_SYMBOLS),
  [L_NUMPAD] = LAYOUT_wrapper(LAYER_NUMPAD),
  [L_VSCODE] = LAYOUT_wrapper(LAYER_VSCODE),
  [L_WINDOWS] = LAYOUT_wrapper(LAYER_WINDOWS),
  [L_MEDIA] = LAYOUT_wrapper(LAYER_MEDIA),
};

// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(L_POINTER);
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(L_POINTER);
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE
