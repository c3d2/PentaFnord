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


static uint8_t oldinput; // button readings from last poll cycle
static uint8_t curinput; // button readings from current poll cycle
static uint8_t btnstates[BTN_BUTTONS];
static uint8_t btncounters[BTN_BUTTONS];
static timer_t btntimers[BTN_BUTTONS];


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
	for(uint8_t i=0; i<BTN_BUTTONS; i++){
		btnstates[i] = BTNST_NTRL;
		btncounters[i] = 0;
		timer_set(&btntimers[i], 0x05); //50ms
	}
	//init button counter

}


// reset button to neutral state
void button_clear(uint8_t button){
	btnstates[button] = BTNST_NTRL;
}




void button_poll(){
	curinput = ~((PINC & 0b00001111) | ((PINB << 3) & 0b00010000)) & 0b00011111;
	uint8_t mask = 1;
	for(uint8_t i=0; i<BTN_BUTTONS; i++){
		if (timer_expired(&btntimers[i])){
			switch(btnstates[i])
			{
				case BTNST_NTRL: //NEUTRAL
					if (curinput & mask){ //button down
						btncounters[i] = 0;
						btnstates[i] = BTNST_DBNC;
					}
					break;

				case BTNST_DBNC: //intermediate state, check if button is still pressed to debounce
			    	if (curinput & mask){
			    		//button still down, so it is pressed for sure
			    		btncounters[i]++;
			    		btnstates[i] = BTNST_SDN;

			    	} else {
			    		//button was released
			    		btnstates[i] = BTNST_NTRL;
			    	}
			        break;
			    case BTNST_SDN: //is shortpressed and still held down
			    	if (curinput & mask){
			    		btncounters[i]++;
			    		if (btncounters[i] > BTN_T_LONGFACT){ //500ms held
			    			btnstates[i] = BTNST_LDN;
			    		}
			    	} else { //button was released
			    		btnstates[i] = BTNST_SUP;
			    		//signal shortclick
			    	}
			        break;
			    case BTNST_LDN: //is longpressed and still held down
			    	if (!(curinput & mask)){
			    		//button was released
			    		btnstates[i] = BTNST_LUP; //signal longpress

			    	}
			    	break;
			    default: //curently catches BTNST_SUP and BTNST_LUP
			    	{}
			    	// do nothing yet
			} //end switch

			timer_set(&btntimers[i], BTN_T_DEBOUNCE);
		} //end if timer expired
		mask<<=1;
	} //end for
	oldinput = curinput;
	return;

}

bool btn_state(uint8_t btnstate, uint8_t btn){
	return (btnstates[btn]==btnstate);
}




