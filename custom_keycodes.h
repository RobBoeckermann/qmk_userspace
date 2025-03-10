#pragma once
#include QMK_KEYBOARD_H

enum custom_keycodes {
    SELECT_WORD = SAFE_RANGE,
    KC_DPI_RESET,
    KC_SNIPING_RESET,
    DRAG_SCROLL,
    CPI_RESET,
    CPI_UP,
    CPI_DOWN
};

bool set_scrolling = false;

// Modify these values to adjust the scrolling speed
#define SCROLL_DIVISOR_H 8.0
#define SCROLL_DIVISOR_V 8.0

// Variables to store accumulated scroll values
float scroll_accumulated_h = 0;
float scroll_accumulated_v = 0;

// Function to handle mouse reports and perform drag scrolling
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    // Check if drag scrolling is active
    if (set_scrolling) {
        // Calculate and accumulate scroll values based on mouse movement and divisors
        scroll_accumulated_h += (float)mouse_report.x / SCROLL_DIVISOR_H;
        scroll_accumulated_v += (float)mouse_report.y / SCROLL_DIVISOR_V;

        // Assign integer parts of accumulated scroll values to the mouse report
        mouse_report.h = (int8_t)scroll_accumulated_h;
        mouse_report.v = (int8_t)scroll_accumulated_v;

        // Update accumulated scroll values by subtracting the integer parts
        scroll_accumulated_h -= (int8_t)scroll_accumulated_h;
        scroll_accumulated_v -= (int8_t)scroll_accumulated_v;

        // Clear the X and Y values of the mouse report
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}

void reset_cpi_to_default(void) {
    pointing_device_set_cpi(PMW33XX_CPI);
}

void increment_cpi(void) {
    pointing_device_set_cpi(pointing_device_get_cpi() + 200);
}


void decrement_cpi(void) {
    pointing_device_set_cpi(pointing_device_get_cpi() - 200);
}

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
    case DRAG_SCROLL:
        set_scrolling = record->event.pressed;
        break;
    case CPI_RESET:
        if (record->event.pressed) {
            reset_cpi_to_default();
        }
        break;
    case CPI_UP:
        if (record->event.pressed) {
            increment_cpi();
        }
        break;
    case CPI_DOWN:
        if (record->event.pressed) {
            decrement_cpi();
        }
        break;
    }

    return true;
};

// Function to handle layer changes and disable drag scrolling when not in AUTO_MOUSE_DEFAULT_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    // Disable set_scrolling if the current layer is not the AUTO_MOUSE_DEFAULT_LAYER
    if (get_highest_layer(state) != AUTO_MOUSE_DEFAULT_LAYER) {
        set_scrolling = false;
    }
    return state;
}
