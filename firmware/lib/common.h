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

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

/* macros for extracting low and high byte */
#define LO8(x) (uint8_t)(0x00ff & (x))
#define HI8(x) (uint8_t)((0xff00 & (x)) >> 8)

/* macros for concatenating PORT, PIN and DDR */
#define _CONCAT(a, b)            a ## b
#define _OUTPORT(name)           _CONCAT(PORT, name)
#define _INPORT(name)            _CONCAT(PIN, name)
#define _DDRPORT(name)           _CONCAT(DDR, name)
#define _PCIE(name)              _CONCAT(PCIE, name)
#define _PCIF(name)              _CONCAT(PCIF, name)
#define _PCMSK(name)             _CONCAT(PCMSK, name)
#define _PCINT(name)             _CONCAT(PCINT, name)

/* __noinline attribute (opposite of inline attribute */
#define __noinline __attribute__((noinline))

///* structure for accessing bytes and words in an uint32_t */
//void union uint32_t_access {
//    uint8_t bytes[4];
//    uint16_t words[2];
//    uint32_t raw;
//}

#endif
