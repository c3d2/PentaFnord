/*
 * PentaFnord Firmware
 *
 * by Alexander Lorz <bigalex@gmx.de>
 *
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

#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>  
#include "addr.h"
#include "../globals.h"


// 0x00001 --> 0x00011
// 0x00010 --> 0x00101
// 0x00011 --> 0x00101
// 0x00100 --> 0x01001

void ADDR_Init (void) {  //initializes the address by reading it from hardware
	G_hwaddr = 0;
	DDRD |= 0xFE; //D1-D7 as output	
	DDRB  &= 0xFE; // PB0 as input

	PORTB |= 0x01; // pull up on PB0
/*
	PORTD = 0b11111101;
	_delay_us(10);
	G_hwaddr = (PINB & 0x01)==0 ? G_hwaddr + 1 : G_hwaddr; 
	PORTD = 0b11111011;
	_delay_us(10);
	G_hwaddr = (PINB & 0x01)==0 ? G_hwaddr + 2 : G_hwaddr;
	PORTD = 0b11110111;
	_delay_us(10);
	G_hwaddr = (0x01 & PINB)==0 ? G_hwaddr + 4 : G_hwaddr;
	PORTD = 0b11101111;
	_delay_us(10);
	G_hwaddr = (PINB & 0x01)==0 ? G_hwaddr + 8 : G_hwaddr;
	PORTD = 0b11011111;
	_delay_us(10);
	G_hwaddr = (PINB & 0x01)==0 ? G_hwaddr + 16 : G_hwaddr;
	PORTD = 0b10111111;
	_delay_us(10);
	G_hwaddr = (PINB & 0x01)==0 ? G_hwaddr + 32 : G_hwaddr;
	PORTD = 0b01111111;
	_delay_us(10);
	G_hwaddr = (PINB & 0x01)==0 ? G_hwaddr + 64 : G_hwaddr;
*/
	uint8_t mymask = 0b010;  //for ledbar,else 0b001
	for(uint8_t i=0; i<6; i++){  //for lebar, else i<7
		mymask = mymask << 1;
		PORTD = ~mymask;
		_delay_us(1);
		if ((PINB & 0x01)==0) 
			G_hwaddr |= mymask;
			
	};

	PORTD &= 0x01;
//	G_hwaddr = G_hwaddr >> 1;
	G_hwaddr = G_hwaddr >> 2; //for ledbar
//	G_hwaddr = 0x03;
//	G_hwaddr = 4;
	G_swaddr = (uint16_t) G_hwaddr + ADDR_OFFSET;
	//G_swaddr = 0x00;


};


