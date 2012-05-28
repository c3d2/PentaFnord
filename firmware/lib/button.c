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

#include <stdio.h>
#include <avr/io.h>
#include "timer.h"
#include "../globals.h"
#include "button.h"



/*
 * RIGHT	ADC2		PC2				PC0
 * LEFT		SPARE_C3	PC3
 * DOWN		ADC1		PC1		  PC3   PB1   PC2
 * UP		ADC0		PC0
 * PRESS	SPARE_B1	PB1				PC1
 */


uint8_t oldinput; // button readings from last poll cycle
uint8_t curinput; // button readings from current poll cycle
uint8_t btnstate;
uint8_t btncounter;

static timer_t btntimer;


void button_init(){
	//PB1 as Input
	DDRB &=  0b11111101;
	//enable PULLUP - do we have to?
	PORTB |= 0b00000010;
	//PC0-3 as input
	DDRC &=  0b11110000;
	PORTC |= 0b00001111;
	// set predefined buttonstates
	oldinput = 0;
	curinput = 0;
	btnstate = 0;
	btncounter = 0;
	//init button counter
	timer_set(&btntimer, 0x05); //50ms
}


void button_clear(void){
//	G_buttonKlick = false;
//	G_buttonHold = false;
}

uint8_t btn_down(){
//	G_buttonKlick = false;
//	G_buttonHold = false;
	return (curinput & 0b00010000);
}


void button_poll(){
	if (timer_expired(&btntimer)){

		curinput = ~((PINC & 0b00001111) | ((PINB << 3) & 0b00010000)) & 0b00011111;

		switch(btnstate)
		{
		    case 1: //intermediate state, check if button is still pressed to debounce
		    	if (curinput & 0b01){
		    		//button still down, so it is pressed for sure
		    		btncounter++;
		    		btnstate = 2;

		    	} else {
		    		//button was released
		    		btnstate = 0;
		    	}
		        break;
		    case 2: //is shortpressed and still held down
		    	if (curinput & 0b01){
		    		btncounter++;
		    		if (btncounter >10){ //500ms held
		    			btnstate =3;
		    		}
		    	} else { //button was released
		    		btnstate = 0;;
		    		//signal shortclick
		    		G_buttonShort = true;
		    	}
		        break;
		    case 3: //is longpressed and still held down
		    	if (!(curinput & 0b01)){
		    		//button was released
		    		btnstate = 0;
		    		//signal longpress
		    		G_buttonLong = true;
		    	}
		    	break;
		    default : //0
		        if (curinput & 0b00000001){
		        	//button down
		        	btncounter = 0;
		        	btnstate = 1;

		        }

		}
		oldinput = curinput;
		timer_set(&btntimer, 0x05);
	}


	return;

}





