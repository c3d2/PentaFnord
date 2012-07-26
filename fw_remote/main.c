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
 * The USART control code is derived by code from the 4CHLED project
 * by sebseb7: https://github.com/sebseb7/eagle/tree/master/4CHLED/firmware
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

//#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define EXTERN

#include "globals.h"

#include "lib/button.h"
#include "lib/pwm.h"
#include "lib/color.h"
#include "lib/timer.h"
#include "lib/common.h"
#include "lib/usart.h"

//void mode_poll();

volatile uint8_t count;

uint8_t red = 0;
uint8_t green = 0;

int main(void)
{
	// some people think this makes a difference
	// and who am I to argue
	//power_all_disable();

	/** main function
	 */

//      G_mode = STARTMODE;
//      G_trigger = INITIAL_TRIGGER;

	pwm_init();
	timer_init();
	button_init();
	USART0_Init();

	/* enable interrupts globally */
	sei();

	USART0_putc('0'); //command to switch on all LEDs
	while (1) {
	//	USART0_putc('1');
		//USART0_putc('1');
		if (btn_state(BTNST_SUP, BTN_UP)) {
			button_clear(BTN_UP);
			USART0_putc('1'); //switch on red LEDs
		}

		if (btn_state(BTNST_SUP, BTN_RIGHT)) {

			button_clear(BTN_RIGHT);
		}
		if (btn_state(BTNST_SUP, BTN_DOWN)) {
			button_clear(BTN_DOWN);
			USART0_putc('2'); //switch on green LEDs
		}
		if (btn_state(BTNST_SUP, BTN_LEFT)) {

			button_clear(BTN_LEFT);
		}
		if (btn_state(BTNST_LUP, BTN_UP)) {
			button_clear(BTN_UP);
		}
		if (btn_state(BTNST_LUP, BTN_CENTER)) {
			button_clear(BTN_CENTER);
		}
		if (btn_state(BTNST_SUP, BTN_CENTER)) {
			button_clear(BTN_CENTER);
		}
		uint8_t data = 0;
		button_poll();

	}

	return 0;
}				// END main(void)
