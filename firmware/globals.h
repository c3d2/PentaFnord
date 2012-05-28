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

#ifndef GLOBALS_H
#define GLOBALS_H


#include <stdbool.h>
#include <avr/version.h>
#include "lib/common.h"


#define F_CPU 20000000UL


/* check for avr-libc version */
#if __AVR_LIBC_VERSION__ < 10600UL
#error "newer libc version (>= 1.6.0) needed!"
#endif


/* check if cpu speed is defined */
#ifndef F_CPU
#error "please define F_CPU! (see Makefile)"
#endif



/* check if this cpu is supported */
#if !(defined(__AVR_ATmega48__) || defined(__AVR_ATmega168__))
#error "this cpu is not supported yet!"
#endif

/* Port D is used for PWM with:
 * 		PD3: red
 * 		PD4: green
 * 		PD5: blue
 * 		PD6: white
 * 		PD7: ultra violet
 */
#define PWM_PORT D
#define PWM_CHANNELS 5
#define PWM_SHIFT 3
#define PWM_CHANNEL_MASK (_BV(PD3) | _BV(PD4) | _BV(PD5) | _BV(PD6)| _BV(PD7)) //This is Red Green Blue White UV


/* min brightness level */
#define PWM_MIN_BRIGHTNESS 0



#ifndef EXTERN
#define EXTERN extern
#endif
//
//
EXTERN bool G_buttonShort; //Button has been clickes
EXTERN bool G_buttonLong;  //Button has been long pressed
//EXTERN uint8_t G_mode;     //Current Operating mode
//EXTERN uint8_t G_trigger;
//EXTERN bool G_greenUp;
//EXTERN bool G_greenDown;
//EXTERN bool G_redUp;
//EXTERN bool G_redDown;




//#define LONGPRESS  		30 //300ms //grenze zwischen Klicken und lange dr�cken
//#define STARTMODE 		 0 	   //operating mode to start with
//#define NUMMODES 		 5			//number of OP Modes
//#define VALUEHOLD 		50 //How long to hold maximum Values in Diags mode
//#define INITIAL_TRIGGER  5 //anf�ngliche Empfindlichkeitssschwelle
//#define HITEXPIRETIME  500 //maximal Dauer einer Trefferanzeige
//#define PWRFADE_CONST	64
//#define PWRFADE_DELAY  80
//#define PWR_HITUP 		 5


#endif /* GLOBALS_H */
