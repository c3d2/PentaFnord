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

/* small timer library, uses timer2 */

#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>
#include <stdbool.h>

/* structures */
typedef struct {
    uint8_t current;
    uint8_t timeout;
} timer_t;

typedef struct {
    uint8_t current;
    uint16_t timeout;
} timerL_t;


/* functions */
void timer_init(void);
void timer_set(timer_t *t, uint8_t timeout);
bool timer_expired(timer_t *t);
void timerL_set(timerL_t *t, uint16_t timeout);
bool timerL_expired(timerL_t *t);

#endif
