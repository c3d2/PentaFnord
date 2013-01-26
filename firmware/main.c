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
#include "lib/addr.h"
#include "lib/buscmd.h"



int main(void)
{
	// some people think this makes a difference
	// and who am I to argue
	//power_all_disable();

	/** main function
	 */

//      G_mode = STARTMODE;
//      G_trigger = INITIAL_TRIGGER;

	ADDR_Init();
	pwm_init();
	timer_init();
	button_init();
	USART0_Init();


	/* enable interrupts globally */
	sei();

	//static  struct rgb_color_t col_antiquewhite PROGMEM = {0xFA,0xEB,0xD7};
	//static  struct rgb_color_t col_antiquewhite PROGMEM ={0xFA,0xEB,0xD7};
	//static  struct rgb_color_t col_purple PROGMEM = {0x80,0x00,0x80};
	//static  struct rgb_color_t col_darksalmon = {0xE9,0x96,0x7A};

	//pwm_fade_rgb(&((static struct rgb_color_t){0xFF,0xFF,0xFF}), 1,1);
	//pwm_fade_rgb(&((struct rgb_color_t){0xFF,0xFF,0xFF}), 1,4);
	//pwm_fade_rgb(&col_darksalmon, 1,10);
	//  pwm_fade(0x80,0x80,0x80,1,5);

//    pwm_fade_rgb(&COL_LIME, 1,1);
//    pwm_fade_rgb(&COL_CHARTREUSE, 1,1);
	//pwm_set_color(&((struct rgb_color_t){0xFF,0xF0,0x80,0x00,0x00}));
	//   pwm_fade_rgb(&((struct rgb_color_t){0xFF,0x20,0x20,0x80,0x00}),10,100);
	//pwm_set_color(&((struct rgb_color_t){0x00,0x00,0x20,0x00,0xFF}));

	pwm_fade_rgb(&((struct rgb_color_t) {
		       0x01, 0x01, 0x01, 0x01, 0x01}), 1, 0);

	uint8_t ar = (G_hwaddr & 0x01) != 0 ? 0x20 : 0x00; 
	uint8_t ag = (G_hwaddr & 0x02) != 0 ? 0x20 : 0x00; 
	uint8_t ab = (G_hwaddr & 0x04) != 0 ? 0x20 : 0x00;
	uint8_t aw = (G_hwaddr & 0x08) != 0 ? 0x20 : 0x00;
	uint8_t au = (G_hwaddr & 0x10) != 0 ? 0x20 : 0x00;

	pwm_fade_rgb(&((struct rgb_color_t) {
		       ar, ag, ab, aw, au}), 1, 0);


//	USART0_putc ('x');
//	void USART0_put_uint8(uint8_t);
//	USART0_crlf();
//	USART0_putc ('y');

	while (1) {

		pwm_poll();

		if (btn_state(BTNST_SUP, BTN_UP)) {
			pwm_fade_rgb(&((struct rgb_color_t) {
				       0xFF, 0x00, 0x00, 0x00, 0x00}), 2, 1);
			button_clear(BTN_UP);

		}

		if (btn_state(BTNST_SUP, BTN_RIGHT)) {
			pwm_fade_rgb(&((struct rgb_color_t) {
				       0x00, 0xFF, 0x00, 0x00, 0x00}), 2, 1);
			button_clear(BTN_RIGHT);
		}
		if (btn_state(BTNST_SUP, BTN_DOWN)) {
			pwm_fade_rgb(&((struct rgb_color_t) {
				       0x00, 0x00, 0xFF, 0x00, 0x00}), 2, 1);
			button_clear(BTN_DOWN);
		}
		if (btn_state(BTNST_SUP, BTN_LEFT)) {
			pwm_fade_rgb(&((struct rgb_color_t) {
				       0x00, 0x00, 0x00, 0xFF, 0x00}), 2, 1);
			button_clear(BTN_LEFT);
		}
		if (btn_state(BTNST_LUP, BTN_UP)) {
			pwm_fade_rgb(&((struct rgb_color_t) {
				       0x00, 0x00, 0x00, 0x00, 0xFF}), 2, 1);
			button_clear(BTN_UP);
		}
		if (btn_state(BTNST_LUP, BTN_CENTER)) {
			pwm_fade_rgb(&((struct rgb_color_t) {
				       0x00, 0x00, 0x00, 0x00, 0x00}), 2, 1);
			button_clear(BTN_CENTER);
		}
		if (btn_state(BTNST_SUP, BTN_CENTER)) {
			pwm_fade_rgb(&((struct rgb_color_t) {
				       0xFF, 0xFF, 0xFF, 0xFF, 0xFF}), 2, 1);
			button_clear(BTN_CENTER);
		}
		pwm_poll();
		pwm_poll_fading();
		buscmd_poll();
		pwm_poll();
		button_poll();
		pwm_poll();

	}

	return 0;
}				// END main(void)
