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

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

// TODO:
// DPI configs and buttons https://docs.bastardkb.com/fw/charybdis-features.html
// Dynamic macros https://docs.qmk.fm/keycodes#dynamic-macros

typedef struct {
  bool is_press_action;
  int state;
} tap;

enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3,
  DOUBLE_HOLD = 4,
  DOUBLE_SINGLE_TAP = 5, //send two single taps
  TRIPLE_TAP = 6,
  TRIPLE_HOLD = 7
};

//Tap dance enums
enum {
  X_CTL = 0,
  TD_COPY,
  TD_PASTE,
  TD_TAB_CLOSE_REOPEN
};

int cur_dance (tap_dance_state_t *state);

//for the x tap dance. Put it here so it can be used in any keymap
void x_finished (tap_dance_state_t *state, void *user_data);
void x_reset (tap_dance_state_t *state, void *user_data);
void copy_finished(tap_dance_state_t *state, void *user_data);

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

#define KC_Y_MEDIA LT(L_MEDIA, KC_Y)
#define KC_J_VSCODE LT(L_VSCODE, KC_J)
#define KC_ENT_FUNCTION LT(L_FUNCTION, KC_ENT)
#define KC_BSPC_SHIFT LT(L_SHIFT, KC_BSPC)
#define KC_TAB_ARROWS LT(L_ARROWS, KC_TAB)
#define KC_DEL_NUMPAD LT(L_NUMPAD, KC_DEL)
#define KC_SPC_SYMBOLS LT(L_SYMBOLS, KC_SPC)
// hi i love you 

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE


// clang-format off

// -----------------------------------------------------------------------------------------
// |   q   |  h  |  o  |  u  |  x  | /**/ |  g  |  c  |  r  |  f  |  z  |
// |  y/-  |  i  |  e  |  a  |  .  | /**/ |  d  |  s  |  t  |  n  | b/; |
// |   j   |  /  |  ,  |  k  |  '  | /**/ |  w  |  m  |  l  |  p  |  v  |
//       | ENTER | BACKSPACE | TAB | /**/ | DELETE | SPACE |
#define LAYER_BASE \
KC_Q,         KC_H,     KC_O,               KC_U,           KC_X,           /**/  KC_G,           KC_C,           KC_R,   KC_F,   KC_Z,           \
KC_Y_MEDIA,   KC_I,     KC_E,               KC_A,           KC_DOT,         /**/  KC_D,           KC_S,           KC_T,   KC_N,   KC_B, \
KC_J_VSCODE,  KC_SLSH,  KC_COMM,            KC_K,           KC_QUOT,        /**/  KC_W,           KC_M,           KC_L,   KC_P,   KC_V, \
                        KC_ENT_FUNCTION,    KC_BSPC_SHIFT,  KC_TAB_ARROWS,  /**/  KC_DEL_NUMPAD,  KC_SPC_SYMBOLS

// -----------------------------------------------------------------------------------------
// |  Q  |  H  |  O  |  U  |  X  | /**/ |  G  |  C  |  R  |  F  |  Z  |
// |  Y  |  I  |  E  |  A  |  @  | /**/ |  D  |  S  |  T  |  N  |  B  |
// |  J  |  ?  |  !  |  K  |  `  | /**/ |  W  |  M  |  L  |  P  |  V  |
//             |     |  *  |     | /**/ |     | LOCK |
#define LAYER_SHIFT \
LSFT(KC_Q), LSFT(KC_H), LSFT(KC_O), LSFT(KC_U), LSFT(KC_X),       LSFT(KC_G), LSFT(KC_C), LSFT(KC_R), LSFT(KC_F), LSFT(KC_Z), \
LSFT(KC_Y), LSFT(KC_I), LSFT(KC_E), LSFT(KC_A), KC_AT,            LSFT(KC_D), LSFT(KC_S), LSFT(KC_T), LSFT(KC_N), LSFT(KC_B), \
LSFT(KC_J), KC_QUES,    KC_EXLM,    LSFT(KC_K), KC_GRV,           LSFT(KC_W), LSFT(KC_M), LSFT(KC_L), LSFT(KC_P), LSFT(KC_V), \
                        _______,    _______,    _______,          _______,    QK_LOCK

// -----------------------------------------------------------------------------------------
// | ESC           |      | CLOSE TAB        | NEW TAB | SAVE | /**/ | F10 | F7 | F8 | F9 |    |
// | LINE COMMENT  | UNDO | COPY             | PASTE   | REDO | /**/ | F11 |    | F1 | F2 | F3 |
// | BLOCK COMMENT | FIND | FIND IN SOLUTION |         |      | /**/ | F12 | F4 | F5 | F6 |    |
//                        |        *         |         |      | /**/ |     |    |
#define KC_COPYCUT MT(MOD_LCTL, KC_C)
#define LAYER_FUNCTION \
KC_ESC,            _______,     TD(TD_TAB_CLOSE_REOPEN),  LCTL(KC_T),   LCTL(KC_S),       /**/ KC_F10,     KC_F7,    KC_F8,    KC_F9,  _______,  \
LCTL(KC_SLSH),     LCTL(KC_Z),  TD(TD_COPY),              TD(TD_PASTE), LCTL(LSFT(KC_Z)), /**/ KC_F11,     _______,  KC_F1,    KC_F2,  KC_F3,    \
LALT(LSFT(KC_A)),  LCTL(KC_F),  LCTL(LSFT(KC_F)),         _______,      _______,          /**/ KC_F12,     KC_F4,    KC_F5,    KC_F6,  _______,  \
                                _______,                  _______,      _______,          /**/ _______,    _______

// -----------------------------------------------------------------------------------------
// | RESET      |                | SELECT WORD |         |          | /**/ |    | HOME    | UP   | END       |        |
// | SCREENSHOT | SHIFT          | CTRL        | ALT     |          | /**/ |    | LEFT    | DOWN | RIGHT     | INSERT |
// | RESTART    | DEBUG/CONTINUE | STEP        | STEP IN | STEP OUT | /**/ |    | PAGE UP |      | PAGE DOWN |        |
//                               |             | BOOT    |     *    | /**/ |    |         |
#define LAYER_ARROWS \
QK_RBT,             _______,    SELECT_WORD,    _______,    _______,        /**/ _______,  KC_HOME,    KC_UP,      KC_END,         _______, \
LGUI(LSFT(KC_S)),   KC_LSFT,    KC_LCTL,        KC_LALT,    _______,        /**/ _______,  KC_LEFT,    KC_DOWN,    KC_RIGHT,       KC_INSERT, \
LSFT(LCTL(KC_F5)),  KC_F5,      KC_F10,         KC_F11,     LSFT(KC_F11),   /**/ _______,  KC_PAGE_UP, _______,    KC_PAGE_DOWN,   _______, \
                                _______,        QK_BOOT,    _______,        /**/ _______,  _______

// -----------------------------------------------------------------------------------------
// |     |  <  |  $  |  >  |     | /**/ |     |  [  |  _  |  ]  |     |
// |  \  |  (  |  "  |  )  |  #  | /**/ |  %  |  {  |  =  |  }  |  |  |
// |     |  :  |  *  |  +  |     | /**/ |     |  &  |  ^  |  ~  |     |
//             |     |     |     | /**/ |     |  *  |
#define LAYER_SYMBOLS \
_______,    KC_LT,      KC_DLR,     KC_GT,      _______, /**/ _______,  KC_LBRC,    KC_UNDS,    KC_RBRC,    _______, \
KC_SLSH,    KC_LPRN,    KC_DQT,     KC_RPRN,    KC_HASH, /**/ KC_PERC,  KC_LCBR,    KC_EQUAL,   KC_RCBR,    KC_PIPE, \
_______,    KC_COLN,    KC_ASTR,    KC_PLUS,    _______, /**/ _______,  KC_AMPR,    KC_CIRC,    KC_TILDE,   _______, \
                        _______,    _______,    _______, /**/ _______,  _______

// -----------------------------------------------------------------------------------------
// |    | (  | +  | )  |    | /**/ | PERIOD    |  7  |  8  |  9  | RESET |
// | =  | /  | -  | *  | .  | /**/ | BACKSPACE |  0  |  1  |  2  |  3    |
// |    | :  |    |    |    | /**/ | COLON     |  4  |  5  |  6  |       |
//           |    |    |    | /**/ |      *    | BOOT|
#define LAYER_NUMPAD \
_______,    KC_LPRN,    KC_PLUS,    KC_RPRN,    _______, /**/ KC_DOT,   KC_7,    KC_8,    KC_9,    QK_RBT,  \
KC_EQL,     KC_SLSH,    KC_MINS,    KC_ASTR,    KC_DOT,  /**/ KC_BSPC,  KC_0,    KC_1,    KC_2,    KC_3,    \
_______,    KC_COLN,    _______,    _______,    _______, /**/ KC_COLN,  KC_4,    KC_5,    KC_6,    _______, \
                        _______,    _______,    _______, /**/ _______,  QK_BOOT

// -----------------------------------------------------------------------------------------
// |     |               |            |                 |    | /**/ |    |    |    |    |    |
// |     | back          | def        | forward         |    | /**/ |    |    |    |    |    |
// |  *  | prev bookmark | bookmark   | next bookmark   |    | /**/ |    |    |    |    |    |
//                       |            |                 |    | /**/ |    |    |
#define LAYER_VSCODE \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
                        _______,    _______,    _______, /**/ _______,  _______

// -----------------------------------------------------------------------------------------
// |     |VD left           |window up      |VD right       |new VD         | /**/ |    |    | BRIGHTNESS UP    |    |    |
// |  *  |window left       |window down    |window right   |exit VD        | /**/ |    |    | BRIGHTNESS DOWN  |    |    |
// |     |                  |               |               |               | /**/ |    |    |                  |    |    |
//                          |monitor left   |task view      |monitor right  | /**/ |    |    |
#define LAYER_WINDOWS \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
_______,    _______,    _______,    _______,    _______, /**/ _______,  _______,    _______,    _______,    _______, \
                        _______,    _______,    _______, /**/ _______,  _______

// -----------------------------------------------------------------------------------------
// |   *   | VOL DOWN / PREV | VOL UP / NEXT | PLAY PAUSE / MUTE |    | /**/ |    |    |    |    |    |
// |       |                 |               |                   |    | /**/ |    |    |    |    |    |
// |       |                 |               |                   |    | /**/ |    |    |    |    |    |
//                           |               |                   |    | /**/ |    |    |
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
             L10, LSFT_T(L11), LCTL_T(L12), LALT_T(L13), LGUI_T(L14),  \
     RGUI_T(R15), RALT_T(R16), RCTL_T(R17), RSFT_T(R18),         R19,  \
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



/* Return an integer that corresponds to what kind of tap dance should be executed.
 *
 * How to figure out tap dance state: interrupted and pressed.
 *
 * Interrupted: If the state of a dance dance is "interrupted", that means that another key has been hit
 *  under the tapping term. This is typically indicitive that you are trying to "tap" the key.
 *
 * Pressed: Whether or not the key is still being pressed. If this value is true, that means the tapping term
 *  has ended, but the key is still being pressed down. This generally means the key is being "held".
 *
 * One thing that is currenlty not possible with qmk software in regards to tap dance is to mimic the "permissive hold"
 *  feature. In general, advanced tap dances do not work well if they are used with commonly typed letters.
 *  For example "A". Tap dances are best used on non-letter keys that are not hit while typing letters.
 *
 * Good places to put an advanced tap dance:
 *  z,q,x,j,k,v,b, any function key, home/end, comma, semi-colon
 *
 * Criteria for "good placement" of a tap dance key:
 *  Not a key that is hit frequently in a sentence
 *  Not a key that is used frequently to double tap, for example 'tab' is often double tapped in a terminal, or
 *    in a web form. So 'tab' would be a poor choice for a tap dance.
 *  Letters used in common words as a double. For example 'p' in 'pepper'. If a tap dance function existed on the
 *    letter 'p', the word 'pepper' would be quite frustating to type.
 *
 * For the third point, there does exist the 'DOUBLE_SINGLE_TAP', however this is not fully tested
 *
 */
int cur_dance (tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed)  return SINGLE_TAP;
    //key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
    else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    /*
     * DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
     * action when hitting 'pp'. Suggested use case for this return value is when you want to send two
     * keystrokes of the key, and not the 'double tap' action/macro.
    */
    if (state->interrupted) return DOUBLE_SINGLE_TAP;
    else if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  //Assumes no one is trying to type the same letter three times (at least not quickly).
  //If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
  //an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
  if (state->count == 3) {
    if (state->interrupted || !state->pressed)  return TRIPLE_TAP;
    else return TRIPLE_HOLD;
  }
  else return 8; //magic number. At some point this method will expand to work for more presses
}

//instanalize an instance of 'tap' for the 'x' tap dance.
static tap xtap_state = {
  .is_press_action = true,
  .state = 0
};

void copy_finished(tap_dance_state_t *state, void *user_data) {
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case SINGLE_TAP: tap_code16(LCTL(KC_C)); break;
        case SINGLE_HOLD: tap_code16(LCTL(KC_X)); break;
    }
}
// void copy_reset(tap_dance_state_t *state, void *user_data) {
//     switch (xtap_state.state) {
//         case SINGLE_TAP: unregister_code16(LCTL(KC_C)); break;
//         case SINGLE_HOLD: unregister_code16(LCTL(KC_X)); break;
//     }
//   xtap_state.state = 0;
// }

void paste_finished(tap_dance_state_t *state, void *user_data) {
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case SINGLE_TAP: tap_code16(LCTL(KC_V)); break;
        case SINGLE_HOLD: tap_code16(LGUI(KC_V)); break;
        case DOUBLE_HOLD: tap_code16(LCTL(LSFT(KC_V))); break;
        default: break;
    }
}

void tab_close_reopen_finished(tap_dance_state_t *state, void *user_data) {
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case SINGLE_TAP: tap_code16(LCTL(KC_W)); break;
        case SINGLE_HOLD: tap_code16(LCTL(LSFT(KC_T))); break;
        default: break;
    }
}

tap_dance_action_t tap_dance_actions[] = {
  [X_CTL]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL,x_finished, x_reset),
  [TD_COPY] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, copy_finished, NULL),
  [TD_PASTE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, paste_finished, NULL),
  [TD_TAB_CLOSE_REOPEN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, tab_close_reopen_finished, NULL)
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
