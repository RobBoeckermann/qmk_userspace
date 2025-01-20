#pragma once
#include QMK_KEYBOARD_H

enum custom_keycodes {
    SELECT_WORD = SAFE_RANGE,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case SELECT_WORD:
        if (record->event.pressed) 
        {
            SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_LEFT)SS_DOWN(X_LSFT)SS_TAP(X_RIGHT));
        } 
        else {}
        break;
    case KC_Y:
        if (get_mods() == MOD_BIT(KC_RCTL)) {
            if (record->event.pressed) {
                unregister_code(KC_RCTL);
                tap_code(KC_MINUS);
                register_code(KC_RCTL);
            } 
            else {}
            return false;
        }
        break;
    case KC_B:
        if (get_mods() == MOD_BIT(KC_LCTL)) {
            if (record->event.pressed) {
                unregister_code(KC_LCTL);
                tap_code(KC_SEMICOLON);
                register_code(KC_LCTL);
            } 
            else {}
            return false;
        }
        break;
    }
    return true;
};
