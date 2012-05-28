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


#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <avr/io.h>
#include "io.h"
#include "timer.h"

static volatile uint8_t internal_counter;

void timer_init(void)
{

#if defined(__AVR_ATmega48__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega168__)
    /* initialize timer2, CTC at 10ms, prescaler 1024 */
    OCR2A = F_CPU/1024/100;
    TCCR2A = _BV(WGM21);
    TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);
    TIMSK2 = _BV(OCIE2A);
#else
#error "unknown controller, unable to initialize timer2"
#endif
}

void timer_set(timer_t *t, uint8_t timeout)
{
    t->current = internal_counter;
    t->timeout = timeout;
}

void timerL_set(timerL_t *t, uint16_t timeout)
{
    t->current = internal_counter;
    t->timeout = timeout;
}




bool timer_expired(timer_t *t)
{
    if (t->timeout == 0)
        return true;

    /* attention: this is not correct, if internal_counter is incremented by more than one
     * between two calls of timer_expired()! */
    if (t->current != internal_counter) {
        t->timeout--;
        t->current = internal_counter;
    }

    return false;
}


bool timerL_expired(timerL_t *t)
{
    if (t->timeout == 0)
        return true;

    /* attention: this is not correct, if internal_counter is incremented by more than one
     * between two calls of timer_expired()! */
    if (t->current != internal_counter) {
        t->timeout--;
        t->current = internal_counter;
    }

    return false;
}



/* timer interrupt function */
#if defined(__AVR_ATmega8__)
ISR(TIMER2_COMP_vect, ISR_NOBLOCK) {
    internal_counter++;
}
#elif defined(__AVR_ATmega48__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega168__)
ISR(TIMER2_COMPA_vect, ISR_NOBLOCK) {
    internal_counter++;
}
#endif
