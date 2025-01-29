#pragma once
#include QMK_KEYBOARD_H

enum custom_keycodes {
    SELECT_WORD = SAFE_RANGE,
    KC_DPI_RESET,
    KC_SNIPING_RESET
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case SELECT_WORD:
        if (record->event.pressed) 
        {
            SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_LEFT)SS_DOWN(X_LSFT)SS_TAP(X_RIGHT));
        }
        break;
    case KC_Y:
        if (get_mods() == MOD_BIT(KC_RCTL)) {
            if (record->event.pressed) {
                unregister_code(KC_RCTL);
                tap_code(KC_MINUS);
                register_code(KC_RCTL);
            } 
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
            return false;
        }
        break;
    case KC_DPI_RESET:
        if (record->event.pressed) {
            charybdis_get_pointer_default_dpi();
        }
        return false;
    case KC_SNIPING_RESET:
        if (record->event.pressed) {
            charybdis_get_pointer_sniping_dpi();
        }
        return false;
    }

    return true;
};
