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

#ifndef PWM_H
#define PWM_H

#include "timer.h"
#include "color.h"

#if PWM_CHANNELS != 5
#error "PWM_CHANNELS is not 5, this is unsupported!"
#endif

#define PWM_HSV_SIZE sizeof(struct hsv_color_t)

struct global_pwm_t
{
    /* current color */
    struct rgb_color_t current;

    /* target for fading engine */
    struct rgb_color_t target;

    /* delay and step for fading engine */
    uint8_t fade_delay[PWM_CHANNELS];
    uint8_t fade_step[PWM_CHANNELS];
};

extern struct global_pwm_t global_pwm;

/* prototypes */
void pwm_init(void);
void pwm_poll(void);
void pwm_poll_fading(void);

void pwm_fade_rgb(const struct rgb_color_t *color, uint8_t step, uint8_t delay);

/* return true if fading process is complete */
bool pwm_target_reached(void);

/* stop fading, hold current color */
void pwm_stop_fading(void);

/* modify color */
void pwm_modify_rgb(struct rgb_color_offset_t *color, uint8_t step, uint8_t delay);


#endif
