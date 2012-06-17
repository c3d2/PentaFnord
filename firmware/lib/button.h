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

#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>

#define BTN_UP     		0
#define BTN_DOWN		1
#define BTN_RIGHT		2
#define BTN_LEFT		3
#define BTN_CENTER		4

#define BTN_BUTTONS     5

#define BTN_T_DEBOUNCE  5  // 50ms debounce time = minimum short press time
#define BTN_T_LONGFACT  10 // after 10 * T_DEBOUNCE = 500ms button reads as long pressed


#define BTNST_NTRL	0	// neutral - initial state nothing interesting, please go along

#define BTNST_DBNC	1	// debounce - pin went up, but we wait for things to stabilize and stop oscillating

#define BTNST_SDN 	2   // affirmative, button is pressed,
						// and it's pressed no longer than
						// BTN_T_LONGFACT * BTN_T_DEBOUNCE * 10ms

#define BTNST_SUP	3   // and button went up after beeing pressed for a _short_ time

#define BTNST_LDN  	4   // button is still down for more than
						//BTN_T_LONGFACT * BTN_T_DEBOUNCE * 10ms

#define BTNST_LUP   5   // button came up after being pressed for a long time


void button_init(void);

void button_poll(void);

// reset button to neutral state
void button_clear(uint8_t button);

//test if buttonstate von btn eqals btnstate, returns true if yes
bool btn_state(uint8_t btnstate, uint8_t btn);


#endif /* BUTTON_H */
