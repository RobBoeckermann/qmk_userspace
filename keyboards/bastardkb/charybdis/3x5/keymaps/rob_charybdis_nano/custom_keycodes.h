#pragma once
#include QMK_KEYBOARD_H

enum custom_keycodes {
    SELECT_WORD = SAFE_RANGE,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case SELECT_WORD:
        if (record->event.pressed) {
            SEND_STRING(SS_DOWN(X_LCTL)SS_TAP(X_LEFT)SS_DOWN(X_LSFT)SS_TAP(X_RIGHT));
        } else {
        }
        break;
    }
    return true;
};
