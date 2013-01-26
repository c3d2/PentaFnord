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
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../globals.h"
#include "pwm.h"
#include "color.h"
#include "buscmd.h"
#include "usart.h"

void buscmd_poll(void) {
	uint8_t data = 0;
	if (USART0_Getc_nb(&data)) {
		//USART0_putc(~0x55);
		if (data == 'w') {  //0x30 = 0
			pwm_fade_rgb(&((struct rgb_color_t) {
				       0xFF, 0xFF, 0xFF, 0xFF, 0xFF}),
				     0xFF, 1);
			//USART0_putc('w');
		}
		if (data == 'r') {
			pwm_fade_rgb(&((struct rgb_color_t) {
				       0xFF, 0x00, 0x00, 0x00, 0x00}),
				     0xFF, 1);
	//				USART0_putc('b');
		
	//				USART0_putc('2');
	//				USART0_putc('3');

		//USART0_crlf();
			USART0_put_uint8(G_hwaddr);
			//USART0_crlf();

		
		}
		if (data == 'g') {
			pwm_fade_rgb(&((struct rgb_color_t) {
				       0x00, 0xFF, 0x00, 0x00, 0x00}),
				     0x08, 1);
			//USART0_putc('g');
		}
		if (data == 'b') {
			pwm_fade_rgb(&((struct rgb_color_t) {
				       0x00, 0x00, 0xFF, 0x00, 0x00}),
				     0x01, 2);
			//USART0_putc('b');
		}
		if (data == 'u') {
			pwm_fade_rgb(&((struct rgb_color_t) {
				       0x00, 0x00, 0x00, 0x00, 0xFF}),
				     0x01, 2);
			//USART0_putc('b');
		}

		if (data == ' ') {
			pwm_fade_rgb(&((struct rgb_color_t) {
				       0x00, 0x00, 0x00, 0x00, 0x00}),
				     0xFF, 1);
			//USART0_putc('0');
		}
	}
} //end buscmd_poll()
