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
 * RIGHT	ADC2		PC2			PC0
 * LEFT		SPARE_C3	PC3
 * DOWN		ADC1		PC1		  PC3   PB1   PC2
 * UP		ADC0		PC0
 * PRESS	SPARE_B1	PB1			PC1
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


void stateswitch(uint8_t i ){
	switch(btnstates[i])
	{
		case BTNST_NTRL: //NEUTRAL
			if (curinput & (1<<i)){ //button down
				btncounters[i] = 0;
				btnstates[i] = BTNST_DBNC;
			}
			break;
		//intermediate state, check if button is still pressed to debounce
		case BTNST_DBNC: 
			btnstates[i] = (curinput & (1<<i))? BTNST_SDN: BTNST_NTRL;
			(btncounters[i])++;
			break;

		case BTNST_SDN: //is shortpressed and still held down
			if (curinput & (1<<i)){
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
			if (!(curinput & (1<<i))){
				//button was released
				btnstates[i] = BTNST_LUP; //signal longpress
			}
			break;
		case BTNST_SUP: //Button came up after being pressed shortly
			if ((curinput & (1<<i))){
				//button was pressed again or is bouncing after release
				btnstates[i] = BTNST_SUPDBNC; //going in special debounce
				btncounters[i] = 0;
			}
			break;
		case BTNST_LUP: //Button came up after being pressed for a long time
			if ((curinput & (1<<i))){
				//button was pressed again or is bouncing after release
				btnstates[i] = BTNST_LUPDBNC; //going in special debounce
				btncounters[i] = 0;
			}
			break;
		case BTNST_SUPDBNC: //Button was pressed again after beeing short pressed(or is bouncing)
			if ((curinput & (1<<i))){
				//button is still pressed --> going to shortpress
				btncounters[i]++;
				btnstates[i] = BTNST_SDN; //starting over from short pressed
			} else {
				btnstates[i] = BTNST_SUP; //nope, it was bouncing, back to old state
			}
			break;
		case BTNST_LUPDBNC: //Button was pressed again after beeing short pressed(or is bouncing)
			if ((curinput & (1<<i))){
				//button is still pressed --> going to shortpress
				btncounters[i]++;
				btnstates[i] = BTNST_SDN; //starting over from short pressed
			} else {
				btnstates[i] = BTNST_LUP; //nope, it was bouncing, back to old state
			}
			break;
		default: //curently catches nothing
			// do nothing yet
			;
	} //end switch
	timer_set(&btntimers[i], BTN_T_DEBOUNCE);
}



void button_poll(){
	curinput = ~((PINC & 0b00001111) | ((PINB << 3) & 0b00010000)) & 0b00011111;
	for(uint8_t i=0; i<BTN_BUTTONS; i++){
		if (timer_expired(&btntimers[i])){
			stateswitch(i);
		} //end if timer expired
	} //end for
	oldinput = curinput;
	return;
}

bool btn_state(uint8_t btnstate, uint8_t btn){
	return (btnstates[btn]==btnstate);
}




