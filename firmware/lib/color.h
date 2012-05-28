/*
 * PentaFnord Firmware
 *
 * by Alexander Lorz <bigalex@gmx.de>
 *
 *
 * Parts of this software are derived work, originating from the fine
 * fnordlicht firmware (see http://lochraster.org/fnordlichtmini)
 * by Alexander Neumann <alexander@bumpern.de> and Lars Noschinski
 * <lars@public.noschinski.de>, originally published and of course
 * republished under the terms of the GNU General Public License version 3
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>
#include "../globals.h"



#if PWM_CHANNELS != 5
#error "PWM_CHANNELS is not 5, this is unsupported!"
#endif

struct rgb_color_t
{
    union {
        struct {
            uint8_t red;
            uint8_t green;
            uint8_t blue;
            uint8_t white;
            uint8_t uv;

        };
        uint8_t rgbwu[5];
    };
};

struct rgb_color_offset_t {
    union {
        struct {
            int8_t red;
            int8_t green;
            int8_t blue;
            uint8_t white;
            uint8_t uv;        };
        int8_t rgbwu[5];
    };
};

struct hsv_color_t
{
    union {
        struct {
            uint16_t hue;
            uint8_t saturation;
            uint8_t value;
        };
        uint8_t hsv[4];
    };
};

struct hsv_color_offset_t {
    int16_t hue;
    int8_t saturation;
    int8_t value;
};

struct dual_color_t
{
    struct rgb_color_t rgbwu;
    struct hsv_color_t hsv;
};

union color_t
{
    /* rgb */
    struct {
        union {
            struct {
                uint8_t red;
                uint8_t green;
                uint8_t blue;
                uint8_t white;
                uint8_t uv;
            };
            uint8_t rgb[5];
        };
        /* marker, 0xff if rgb */
        uint8_t rgb_marker;
    };

    /* hsv */
    struct {
        uint8_t saturation;
        uint8_t value;
        /* 0 <= hue <= 349, otherwise rgb might be assumed */
        uint16_t hue;
    };
};

#endif /* COLOR_H*/
