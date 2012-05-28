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

#ifndef IO_H
#define IO_H

#include <avr/io.h>

/* cpu specific configuration registers */


#if defined(__AVR_ATmega88__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega48__)


#define _TIFR_TIMER1 TIFR1
#define _UCSRA_UART0 UCSR0A
#define _UCSRB_UART0 UCSR0B
#define _UDRIE_UART0 UDRIE0
#define _UDRE_UART0 UDRE0
#define _RXC_UART0 RXC0
#define _TXC_UART0 TXC0
#define _TXEN_UART0 TXEN0
#define _RXEN_UART0 RXEN0
#define _RXCIE_UART0 RXCIE0
#define _UBRRH_UART0 UBRR0H
#define _UBRRL_UART0 UBRR0L
#define _UCSRC_UART0 UCSR0C
#define _UCSZ0_UART0 UCSZ00
#define _UCSZ1_UART0 UCSZ01
#define _U2X_UART0 U2X0
#define _SIG_UART_RECV_UART0 SIG_USART_RECV
#define _SIG_UART_DATA_UART0 SIG_USART_DATA
#define _UDR_UART0 UDR0
#define _IVREG MCUCR
#define _IFR_INT0 EIFR
#define _ICR_INT0 EIMSK
#endif


#endif
