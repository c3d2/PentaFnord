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
#include <avr/interrupt.h>

//#include "main.h"
#include "usart.h"

#define UART_RXBUFSIZE 32

volatile static uint8_t rxbuf0[UART_RXBUFSIZE];
volatile static uint8_t *volatile rxhead0, *volatile rxtail0;
//volatile uint8_t xon = 0;


ISR (USART_RX_vect)
{
	UCSR0B &= ~(1 << RXCIE0);
	asm volatile("sei");

	int diff;
	uint8_t c;
	c=UDR0;
	diff = rxhead0 - rxtail0;
	if (diff < 0) diff += UART_RXBUFSIZE;
	if (diff < UART_RXBUFSIZE -1)
	{
		*rxhead0 = c;
		++rxhead0;
		if (rxhead0 == (rxbuf0 + UART_RXBUFSIZE)) rxhead0 = rxbuf0;
//		if((diff > 100)&&(xon==0))
//		{
//			xon=1;
//			//set the CTS pin
//		}
	}
	UCSR0B |= (1 << RXCIE0);
}


void USART0_Init (void)
{
	// set clock divider
	//	#undef BAUD
	//	#define BAUD 1000000
	//#	#define BAUD 1000000
	//	#include <util/setbaud.h>
	//	UBRR0H = UBRRH_VALUE;
	//	UBRR0L = UBRRL_VALUE;


	DDRD |= (1<<PORTD2);
	PORTD &= ~(1<<PORTD2);

	UBRR0H = 0;
	UBRR0L = 4;//500000
	//UBRR0L = 21;//115200

	//#if USE_2X
	UCSR0A |= (1 << U2X0);	// enable double speed operation
	//#else
	//	UCSR0A &= ~(1 << U2X0);	// disable double speed operation
	//#endif


	// flush receive buffer
	while ( UCSR0A & (1 << RXC0) ) UDR0;

	// set 8N1
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	UCSR0B &= ~(1 << UCSZ02);



	UCSR0B |= (1 << RXEN0);
	UCSR0B &= ~(1 << TXEN0);
	//	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	UCSR0B |= (1 << RXCIE0);

	rxhead0 = rxtail0 = rxbuf0;

}



void USART0_putc (char c)
{
	PORTD |= (1<<PORTD2);
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	PORTD &= ~(1<<PORTD2);
}


uint8_t USART0_Getc_nb(uint8_t *c)
{
	if (rxhead0==rxtail0) return 0;
	*c = *rxtail0;
	if (++rxtail0 == (rxbuf0 + UART_RXBUFSIZE)) rxtail0 = rxbuf0;

//	uint8_t diff = rxhead0 - rxtail0;
//	if((diff < 10)&&(xon==1))
//	{
//		xon=0;
//		//set the CTS pin
//	}

	return 1;
}
