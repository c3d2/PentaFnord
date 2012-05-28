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




//void mode_poll();

	volatile uint8_t count;

	uint8_t red=0;
	uint8_t green=0;


int main(void){
	// some people think this makes a difference
	// and who am I to argue
	//power_all_disable();

	/** main function
	 */

//	G_mode = STARTMODE;
//	G_trigger = INITIAL_TRIGGER;

    pwm_init();
    timer_init();
    button_init();
    G_buttonLong = false;
    G_buttonShort = false;


    /* enable interrupts globally */
    sei();

    //static  struct rgb_color_t col_antiquewhite PROGMEM = {0xFA,0xEB,0xD7};
    //static  struct rgb_color_t col_antiquewhite PROGMEM ={0xFA,0xEB,0xD7};
    //static  struct rgb_color_t col_purple PROGMEM = {0x80,0x00,0x80};
    //static  struct rgb_color_t col_darksalmon = {0xE9,0x96,0x7A};


    //pwm_fade_rgb(&((static struct rgb_color_t){0xFF,0xFF,0xFF}), 1,1);
    //pwm_fade_rgb(&((struct rgb_color_t){0xFF,0xFF,0xFF}), 1,4);
     //pwm_fade_rgb(&col_darksalmon, 1,10);
    //	pwm_fade(0x80,0x80,0x80,1,5);


//    pwm_fade_rgb(&COL_LIME, 1,1);
//    pwm_fade_rgb(&COL_CHARTREUSE, 1,1);
    //pwm_set_color(&((struct rgb_color_t){0xFF,0xF0,0x80,0x00,0x00}));
 //   pwm_fade_rgb(&((struct rgb_color_t){0xFF,0x20,0x20,0x80,0x00}),10,100);
    //pwm_set_color(&((struct rgb_color_t){0x00,0x00,0x20,0x00,0xFF}));



    pwm_fade_rgb(&((struct rgb_color_t){0x01,0x01,0x01,0x01,0x00}), 1,0);
    while (1)
    {

       pwm_poll();
//        if (G_buttonShort) {
//        	pwm_fade_rgb(&((struct rgb_color_t){250,0x00,0x00,0x00,0x00}),100,1);
//        	G_buttonShort = false;
//        }
//
//        if (G_buttonLong) {
//        	pwm_fade_rgb(&((struct rgb_color_t){0x02,0x02,0x02,0x02,0x00}), 100,1);
//        	G_buttonLong = false;
//        }

       if (G_buttonShort) {
       	pwm_fade_rgb(&((struct rgb_color_t){250,0x00,0x00,0x00,0x00}),1,2);
       	G_buttonShort = false;
       }

       if (G_buttonLong) {
       	pwm_fade_rgb(&((struct rgb_color_t){0x02,0xFF,0x02,0x02,0x00}), 1,2);
       	G_buttonLong = false;
       }

//       if (G_buttonShort) {
//    	red++;
//       	pwm_fade_rgb(&((struct rgb_color_t){red,0x00,0x00,0x00,0x00}),255,1);
//       	G_buttonShort = false;
//       }
//
//       if (G_buttonLong) {
//    	   red--;
//       	pwm_fade_rgb(&((struct rgb_color_t){red,0x00,0x00,0x00,0x00}), 255,1);
//       	G_buttonLong = false;
//       }
        pwm_poll();
        pwm_poll_fading();
        pwm_poll();
        button_poll();
        pwm_poll();

    }

    return 0;
} // END main(void)











