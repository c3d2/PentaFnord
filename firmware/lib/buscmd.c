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
#include "addr.h"


uint8_t	led_r = 1;//white
uint8_t	led_g = 1;//blue
uint8_t	led_b = 1;//green
uint8_t	led_w = 1;//red

uint8_t idx = 0;
uint8_t escape = 0;
uint8_t our_data = 0;


void buscmd_poll(void) {

	for(uint8_t i=0; i<0x10; i++) //read max 16 Byte 
	{
		uint8_t data = 0;

		if(USART0_Getc_nb(&data))
		{

			if(data == 0x42)
			{
				idx = 0;
				escape = 0;
				our_data = 0;
				continue;
				
			} else if (data == 0x23)
			{
				idx=5;
				continue;
			} else if (data == 0x65)
			{
				escape = 1;
				continue;
			}

			if(escape == 1)
			{
				if(data == 1)
				{
					data = 0x23;
				}
				else if (data == 2)
				{
					data = 0x42;
				}
				else if (data == 3)
				{
					data = 0x65;
				}
				else if (data == 4)
				{
					data = 0x66;
				}
				escape = 0;
			}

			if(idx == 0)
			{
				if(data == G_swaddr)//addr
				{
					our_data = 1;
				} else if(data == ADDR_OFFSET)//bcast
				{
					our_data = 1;
				}
				else
				{
					our_data = 0;
				}
			} 
			else if(our_data == 1)
			{
			
				if (idx == 1)
				{
					led_r = data;//red
				} else if (idx == 2)
				{
					led_g  = data;//green
				} else if (idx == 3)
				{
					led_b = data;//blue
				} else if (idx == 4)
				{
					led_w = data;//white
					our_data = 0;
					pwm_set_rgb(&((struct rgb_color_t) {
				        led_r, led_g, led_b, led_w, 0x00}));
				}
				
			}
			if(idx < 5)
			{
				idx++;
			}
		}

	}
} //end buscmd_poll()





