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



/* some colors*/
const struct rgb_color_t PROGMEM palettecolors  [] = {
	{.rgbwu={ 0x00, 0x00, 0x00, 0x00, 0x00}},
	{.rgbwu={ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}},
	{.rgbwu={ 0xFF, 0xFF, 0xFF, 0xFF, 0x00}},
	{.rgbwu={ 0xFF, 0x00, 0x00, 0x00, 0x00}},
	{.rgbwu={ 0x00, 0xFF, 0x00, 0x00, 0x00}},
	{.rgbwu={ 0x00, 0x00, 0xFF, 0x00, 0x00}},
	{.rgbwu={ 0x01, 0x01, 0x01, 0x01, 0x00}},
	{.rgbwu={ 0x00, 0x00, 0xFF, 0x00, 0x00}},
	{.rgbwu={ 0x08, 0x00, 0xFF, 0x00, 0x00}},
	{.rgbwu={ 0x07, 0x00, 0xFF, 0x00, 0x00}},
	{.rgbwu={ 0x00, 0x00, 0x00, 0x00, 0xFF}}
};

#define pal_color(X) ( palettecolors + X)

#define P_BLACK    pal_color(0)
#define P_MAXALL   pal_color(1)
#define P_MAXWHITE pal_color(2)
#define P_MAXRED   pal_color(3)
#define P_MAXGREEN pal_color(4)
#define P_MAXBLUE  pal_color(5)
#define P_STRANGE  pal_color(6)
#define P_BEIGE    pal_color(7)
#define P_AQUA     pal_color(8)
#define P_ULTRAV   pal_color(9)



int main(void)
{
	// some people think this makes a difference
	// and who am I to argue
	//power_all_disable();

	/** main function
	 */

	pwm_init();
	timer_init();
	button_init();
	USART0_Init();

	/* enable interrupts globally */
	sei();

	pwm_fade_rgb(pal_color(6), 1, 0);
	while (1) {

		pwm_poll();

		if (btn_state(BTNST_SUP, BTN_UP)) {
			pwm_fade_rgb(P_BLACK, 2, 1);
			button_clear(BTN_UP);
		}
		if (btn_state(BTNST_SUP, BTN_RIGHT)) {
			pwm_fade_rgb(P_MAXRED, 2, 1);
			button_clear(BTN_RIGHT);
		}
		if (btn_state(BTNST_SUP, BTN_DOWN)) {
			pwm_fade_rgb(P_MAXGREEN, 2, 1);
			button_clear(BTN_DOWN);
		}
		if (btn_state(BTNST_SUP, BTN_LEFT)) {
			pwm_fade_rgb(P_MAXBLUE, 2, 1);
			button_clear(BTN_LEFT);
		}
		if (btn_state(BTNST_LUP, BTN_UP)) {
			pwm_fade_rgb(P_ULTRAV, 2, 1);
			button_clear(BTN_UP);
		}
		if (btn_state(BTNST_LUP, BTN_CENTER)) {
			pwm_fade_rgb(P_BLACK, 2, 1);
			button_clear(BTN_CENTER);
		}
		if (btn_state(BTNST_SUP, BTN_CENTER)) {
			pwm_fade_rgb(P_MAXALL, 2, 1);
			button_clear(BTN_CENTER);
		}
		pwm_poll();
		pwm_poll_fading();

		uint8_t data = 0;
		if (USART0_Getc_nb(&data)) {
			switch(data){
				case 0x30: 
					pwm_fade_rgb(pal_color(0), 0xFF, 1);
					USART0_putc('w');
					break;
				case  0x31 :
					pwm_fade_rgb(pal_color(1), 0xFF, 1);
					USART0_putc('r');

					break;
				case  0x32 :
					pwm_fade_rgb(pal_color(2), 0xFF, 1);
					USART0_putc('g');
					break;
				case  0x33 :
					pwm_fade_rgb(pal_color(3), 0xFF, 1);
					USART0_putc('b');
					break;
				case  0x20 :
					pwm_fade_rgb(pal_color(4), 0xFF, 1);
					USART0_putc('0');
					break;
			}
		}

		pwm_poll();
		button_poll();
		pwm_poll();

	}

	return 0;
}// END main(void)

