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
#pragma once

#ifdef VIA_ENABLE
/* VIA configuration. */
#    define DYNAMIC_KEYMAP_LAYER_COUNT 10
#endif // VIA_ENABLE

#ifndef __arm__
/* Disable unused features. */
#    define NO_ACTION_ONESHOT
#endif // __arm__

/* Charybdis-specific features. */

#ifdef POINTING_DEVICE_ENABLE
#define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1500
#define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8 //4
//#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#define CHARYBDIS_MINIMUM_DEFAULT_DPI 2400
#define CHARYBDIS_DEFAULT_DPI_CONFIG_STEP 200

#define CHARYBDIS_MINIMUM_SNIPING_DPI 200
#define CHARYBDIS_SNIPING_DPI_CONFIG_STEP 100


#define CHARYBDIS_DRAGSCROLL_REVERSE_X // inverts horizontal scrolling 
//#define CHARYBDIS_DRAGSCROLL_REVERSE_Y // inverts vertical scrolling
#endif // POINTING_DEVICE_ENABLE


#define TAPPING_TERM 150
#define PERMISSIVE_HOLD
#define QUICK_TAP_TERM 80
