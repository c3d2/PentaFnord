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

/* includes */
#include "../globals.h"

#include <stdint.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/io.h>

#include "common.h"
#include "pwm.h"
#include "timer.h"

/* abbreviations for port, ddr and pin */
#define P_PORT 	_OUTPORT(PWM_PORT)
#define P_DDR 	_DDRPORT(PWM_PORT)

/* TYPES AND PROTOTYPES */
#define PWM_MAX_TIMESLOTS (2*(PWM_CHANNELS+2))

/* internal data for the fading engine */
struct fading_engine_t {
	/* a timer for each channel */
	timer_t timer[PWM_CHANNELS];

	/* and a bitmask, which stands for 'timer is running' */
	uint8_t running;
};

/* timer top values for 256 brightness levels (stored in flash) */

static const uint16_t timeslot_table[] PROGMEM =	//ALEX' EXPONENTIAL CURVE
{
	//14, 28, 42, 56, 71, 86, 102, 118, 134, 150, 167, 184, 202, 220,
	//238, 257, 276, 295, 315, 335, 356, 377, 399, 421, 443, 466, 490,
	//514, 538, 563, 589, 615, 641, 669, 696, 725, 754, 783, 813, 844,
	//875, 907, 940, 974, 1008, 1043, 1078, 1114, 1151, 1189, 1228,
	//1267, 1308, 1349, 1391, 1434, 1478, 1522, 1568, 1615, 1662, 1711,
	//1760, 1811, 1863, 1916, 1970, 2025, 2081, 2138, 2197, 2257, 2318,
	//2380, 2444, 2509, 2575, 2643, 2712, 2783, 2855, 2929, 3004, 3081,
	//3159, 3239, 3321, 3404, 3489, 3576, 3665, 3756, 3848, 3943, 4039,
	//4138, 4239, 4341, 4446, 4553, 4663, 4774, 4888, 5005, 5124, 5245,
	//5369, 5495, 5624, 5756, 5891, 6028, 6169, 6312, 6458, 6608, 6760,
	//6916, 7075, 7237, 7403, 7572, 7745, 7921, 8101, 8285, 8473, 8665,
	//8860, 9060, 9264, 9473, 9685, 9902, 10124, 10351, 10582, 10818, 11059,
	//11305, 11556, 11812, 12074, 12342, 12615, 12893, 13178, 13469, 13765,
	//14068, 14377, 14693, 15016, 15345, 15681, 16024, 16374, 16732, 17097,
	//17470, 17851, 18240, 18637, 19042, 19456, 19878, 20310, 20750, 21200,
	//21659, 22128, 22606, 23095, 23594, 24103, 24623, 25154, 25697, 26250,
	//26815, 27393, 27982, 28583, 29198, 29825, 30465, 31119, 31787, 32468,
	//33164, 33875, 34600, 35341, 36097, 36869, 37657, 38462, 39284, 40123,
	//40980, 41855, 42748, 43660, 44591, 45541, 46512, 47503, 48515, 49548,
	//50603, 51680, 52779, 53902, 55048, 56218, 57413, 58633, 59879, 61151,
	//62450, 63775, 129, 1511, 2923, 4363, 5835, 7337, 8870, 10436, 12035,
	//13667, 15334, 17035, 18773, 20547, 22358, 24207, 26095, 28023, 29991,
	//32001, 34053, 36148, 38287, 40471, 42701, 44977, 47302, 49675, 52098,
	//54572, 57099, 59678, 62311
	//
	1     , 28    , 42    , 56    , 71    , 86    , 102   , 118   , 
	134   , 150   , 167   , 184   , 202   , 220   , 238   , 257   , 
	276   , 295   , 315   , 335   , 356   , 377   , 399   , 421   , 
	443   , 466   , 490   , 514   , 538   , 563   , 589   , 615   , 
	641   , 669   , 696   , 725   , 754   , 783   , 813   , 844   , 
	875   , 907   , 940   , 974   , 1008  , 1043  , 1078  , 1114  , 
	1151  , 1189  , 1228  , 1267  , 1308  , 1349  , 1391  , 1434  , 
	1478  , 1522  , 1568  , 1615  , 1662  , 1711  , 1760  , 1811  , 
	1863  , 1916  , 1970  , 2025  , 2081  , 2138  , 2197  , 2257  , 
	2318  , 2380  , 2444  , 2509  , 2575  , 2643  , 2712  , 2783  , 
	2855  , 2929  , 3004  , 3081  , 3159  , 3239  , 3321  , 3404  , 
	3489  , 3576  , 3665  , 3756  , 3848  , 3943  , 4039  , 4138  , 
	4239  , 4341  , 4446  , 4553  , 4663  , 4774  , 4888  , 5005  , 
	5124  , 5245  , 5369  , 5495  , 5624  , 5756  , 5891  , 6028  , 
	6169  , 6312  , 6458  , 6608  , 6760  , 6916  , 7075  , 7237  , 
	7403  , 7572  , 7745  , 7921  , 8101  , 8285  , 8473  , 8665  , 
	8860  , 9060  , 9264  , 9473  , 9685  , 9902  , 10124 , 10351 , 
	10582 , 10818 , 11059 , 11305 , 11556 , 11812 , 12074 , 12342 , 
	12615 , 12893 , 13178 , 13469 , 13765 , 14068 , 14377 , 14693 , 
	15016 , 15345 , 15681 , 16024 , 16374 , 16732 , 17097 , 17470 , 
	17851 , 18240 , 18637 , 19042 , 19456 , 19878 , 20310 , 20750 , 
	21200 , 21659 , 22128 , 22606 , 23095 , 23594 , 24103 , 24623 , 
	25154 , 25697 , 26250 , 26815 , 27393 , 27982 , 28583 , 29198 , 
	29825 , 30465 , 31119 , 31787 , 32468 , 33164 , 33875 , 34600 , 
	35341 , 36097 , 36869 , 37657 , 38462 , 39284 , 40123 , 40980 , 
	41855 , 42748 , 43660 , 44591 , 45541 , 46512 , 47503 , 48515 , 
	49548 , 50603 , 51680 , 52779 , 53902 , 55048 , 56218 , 57413 , 
	58633 , 59879 , 61151 , 62450 , 63775 , 129   , 1511  , 2923  , 
	4363  , 5835  , 7337  , 8870  , 10436 , 12035 , 13667 , 15334 , 
	17035 , 18773 , 20547 , 22358 , 24207 , 26095 , 28023 , 29991 , 
	32001 , 34053 , 36148 , 38287 , 40471 , 42701 , 44977 , 47302 , 
	49675 , 52098 , 54572 , 57099 , 59678 , 62311
};

/* GLOBAL VARIABLES */
struct global_pwm_t global_pwm;
static struct timeslots_t timeslots;
static struct fading_engine_t fading;

/* encapsulates all pwm data including timeslot and output mask array */
static uint8_t timeslots_masks[2 * (PWM_CHANNELS + 2)];
static uint16_t timeslots_tops[2 * (PWM_CHANNELS + 2)];
static uint8_t timeslots_write;	/* current write head in 'slot' array */
register uint8_t timeslots_read asm("r3");	/* current read head in 'slot' array */

/* FUNCTIONS AND INTERRUPTS */
/* prototypes */
void update_pwm_timeslots(struct rgb_color_t *target);
void update_rgb(uint8_t c);
void enqueue_timeslot(uint8_t mask, uint16_t top);
struct timeslot_t *dequeue_timeslot(void);
void update_last_timeslot(uint8_t mask);
uint8_t timeslots_fill(void);

/* initialize pwm hardware and structures */
void pwm_init(void)
{
	/* init output pins */

	/* set all pins low -> leds off */
	P_PORT &= ~(PWM_CHANNEL_MASK);

	/* configure pins as outputs */
	P_DDR |= PWM_CHANNEL_MASK;

	/* initialize timer 1 */

	/* no prescaler, CTC mode */
	TCCR1B = _BV(CS10) | _BV(WGM12);

	/* enable timer1 overflow (=output compare 1a)
	 * and output compare 1b interrupt */
	TIMSK1 |= _BV(OCIE1A) | _BV(OCIE1B);

	/* set TOP for CTC mode */
	OCR1A = 64000;

	/* load initial delay, trigger an overflow */
	OCR1B = 65000;

	/* reset structures */
	for (uint8_t i = 0; i < PWM_CHANNELS; i++) {
		global_pwm.fade_delay[i] = 1;
		global_pwm.fade_step[i] = 1;
	}

	/* calculate initial timeslots (2 times) */
	update_pwm_timeslots(&global_pwm.current);
	update_pwm_timeslots(&global_pwm.current);

	/* disable fading timers */
	fading.running = 0;
}

/* prepare new timeslots */
void pwm_poll(void)
{
	/* refill timeslots queue */
	while (timeslots_fill() < (PWM_MAX_TIMESLOTS - PWM_CHANNELS - 2))
		update_pwm_timeslots(&global_pwm.current);
}

/* update color values for current fading */
void pwm_poll_fading(void)
{
	uint8_t mask = 1;
	for (uint8_t i = 0; i < PWM_CHANNELS; i++) {
		/* check running timers */
		if ((fading.running & mask) && timer_expired(&fading.timer[i])) {
			update_rgb(i);
			fading.running &= ~mask;
		}

		/* if timer is not running and current != target, start timer */
		if (!(fading.running & mask)
		    && global_pwm.current.rgbwu[i] != global_pwm.target.rgbwu[i]
		    && global_pwm.fade_delay[i] > 0) {
			timer_set(&fading.timer[i], global_pwm.fade_delay[i]);
			fading.running |= mask;
		}

		/* shift mask */
		mask <<= 1;
	}
}

/** update pwm timeslot table (for target color) */
void update_pwm_timeslots(struct rgb_color_t *target)
{
	static uint8_t sorted[PWM_CHANNELS] = { 0, 1, 2, 3, 4 };

	/* sort channels according to the current brightness */
	for (uint8_t i = 0; i < PWM_CHANNELS; i++) {
		for (uint8_t j = i + 1; j < PWM_CHANNELS; j++) {
			if (target->rgbwu[sorted[j]] < target->rgbwu[sorted[i]]) {
				uint8_t temp;

				temp = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = temp;
			}
		}
	}

	/* calculate initial bitmask */
	uint8_t initial_bitmask = 0;

	uint8_t chanmask = 1;
	for (uint8_t i = 0; i < PWM_CHANNELS; i++) {
		if (target->rgbwu[i] > PWM_MIN_BRIGHTNESS) {

			initial_bitmask |= chanmask;

		}

		chanmask <<= 1;
	}

	/* insert first timeslot */
	enqueue_timeslot(initial_bitmask, 65000);

	/* calculate timeslots and masks */
	uint8_t mask = initial_bitmask;
	uint8_t last_brightness = 0;
	for (uint8_t i = 0; i < PWM_CHANNELS; i++) {
		uint8_t brightness = target->rgbwu[sorted[i]];

		/* if color is (nearly off) or max, process next color */
		if (brightness <= PWM_MIN_BRIGHTNESS || brightness == 255)
			continue;

		/* check if current timeslot would happen after the middle interrupt */
		//if (last_brightness < 181 && brightness >= 181) {  //ORIGINAL
		if (last_brightness < 222 && brightness >= 222) {	//ALEX' EXPONENTIAL CURVE
			/* insert (normal) middle interrupt */
			enqueue_timeslot(mask, 65000);
		}

		/* if brightness is new, allocate a new timeslot */
		if (brightness > last_brightness) {

			/* update mask and last_brightness */
			last_brightness = brightness;
			mask &= ~_BV(sorted[i]);

			/* save (normal) timeslot */
			uint16_t top =
			    pgm_read_word(&timeslot_table
					  [target->rgbwu[sorted[i]] - 1]);
			enqueue_timeslot(mask, top);
		} else {
			/* just update mask of last timeslot */
			mask &= ~_BV(sorted[i]);

			update_last_timeslot(mask);
		}
	}

	/* if all interrupts happen before the middle interrupt, insert it here */
	//if (last_brightness < 181) //ORIGINAL
	if (last_brightness < 222)	//ALEX EXPONENTIAL CURVE
		enqueue_timeslot(mask, 65000);

}

//void _update_pwm_timeslots(struct rgb_color_t *target){
//      enqueue_timeslot(0b00000000, 65000);
//      enqueue_timeslot(0b00000001, 1);
//
//      enqueue_timeslot(0b00000000, 2);
//      enqueue_timeslot(0b00000000, 65000);
//      enqueue_timeslot(0b00000000, 65000);
//      enqueue_timeslot(0b00000000, 65000);
//      enqueue_timeslot(0b00000000, 65000);
//      enqueue_timeslot(0b00000000, 65000);
//      enqueue_timeslot(0b00000000, 65000);
//      enqueue_timeslot(0b00000000, 65000);
//}

/** fade any channels not already at their target brightness */
void update_rgb(uint8_t c)
{
	/* return if target reached */
	if (global_pwm.current.rgbwu[c] == global_pwm.target.rgbwu[c])
		return;

	/* check direction */
	if (global_pwm.current.rgbwu[c] < global_pwm.target.rgbwu[c]) {
		uint8_t diff =
		    global_pwm.target.rgbwu[c] - global_pwm.current.rgbwu[c];

		if (diff >= global_pwm.fade_step[c])
			global_pwm.current.rgbwu[c] += global_pwm.fade_step[c];
		else
			global_pwm.current.rgbwu[c] += diff;

	} else {
		uint8_t diff =
		    global_pwm.current.rgbwu[c] - global_pwm.target.rgbwu[c];

		if (diff >= global_pwm.fade_step[c])
			global_pwm.current.rgbwu[c] -= global_pwm.fade_step[c];
		else
			global_pwm.current.rgbwu[c] -= diff;
	}
}

/* timeslot queue handling */
void enqueue_timeslot(uint8_t mask, uint16_t top)
{
	timeslots_masks[timeslots_write] = mask << PWM_SHIFT;
	timeslots_tops[timeslots_write] = top;
	timeslots_write++;
	if (timeslots_write >= PWM_MAX_TIMESLOTS)
		timeslots_write = 0;
}

//inline struct timeslot_t * dequeue_timeslot()
//{
//    struct timeslot_t *t = &timeslots.slot[timeslot_read];
//    timeslot_read++;
//    if (timeslot_read >= PWM_MAX_TIMESLOTS) timeslot_read =0;
//    return t;
//}

void update_last_timeslot(uint8_t mask)
{
	uint8_t i;
	if (timeslots_write > 0)
		i = timeslots_write - 1;
	else
		i = PWM_MAX_TIMESLOTS - 1;

	timeslots_masks[i] = mask << PWM_SHIFT;
}

uint8_t timeslots_fill(void)
{
	if (timeslots_write >= timeslots_read)
		return timeslots_write - timeslots_read;
	else
		return PWM_MAX_TIMESLOTS - (timeslots_read - timeslots_write);
}

/* stop fading, hold current color */
void pwm_stop_fading(void)
{
	memcpy(&global_pwm.target.rgbwu, &global_pwm.current.rgbwu,
	       sizeof(struct rgb_color_t));

	/* ignore all timers */
	fading.running = 0;
}

static uint8_t diff_abs(uint8_t a, uint8_t b)
{
	if (a > b)
		return a - b;
	else
		return b - a;
}

static void compute_speed(uint8_t step, uint8_t delay)
{
	/* search for max distance */
	uint8_t max = 0;
	uint8_t dist = 0;

	for (uint8_t i = 0; i < PWM_CHANNELS; i++) {
		uint8_t d = diff_abs(global_pwm.target.rgbwu[i],
				     global_pwm.current.rgbwu[i]);

		if (d > dist) {
			max = i;
			dist = d;
		}
	}

	/* adjust fading speeds, relative to max distance */
	global_pwm.fade_step[max] = step;
	global_pwm.fade_delay[max] = delay;
	uint8_t cycles = dist / step;	// number of timer cycles needed to reach target
	if (0 == cycles)
		cycles = 1;

	for (uint8_t i = 0; i < PWM_CHANNELS; i++) {
		if (i == max)
			continue;

		uint8_t d = diff_abs(global_pwm.target.rgbwu[i],
				     global_pwm.current.rgbwu[i]);

		// number of timer cycles cyc for significiant channel to reach destination is
		// roughly cyc= dist/step  (it might be dist/step+1, but we are generous here)
		// generally we try to use the same delay (we could be using a smaller delay to have
		// smoother fading than the significiant channel, but whats the point?

		uint8_t mydelay = delay;

		//the stepsize to reach our target in time is

		uint8_t mystep = d / cycles;
		if (0 == mystep)
			mystep = 1;

		// if d is small and we use many cycles we might reach the target much to early
		// so to make things smoother, we adjust the delay time
		uint8_t mycycles = d / mystep;
		if (0 == mycycles)
			mycycles = 1;

		mydelay = cycles / mycycles * delay;
		if (0 == mydelay)
			mydelay = 1;

		global_pwm.fade_delay[i] = mydelay;
		global_pwm.fade_step[i] = mystep;

	}
}

void pwm_fade_rgb(const struct rgb_color_t *color, uint8_t step, uint8_t delay)
{

	/* set target color */
	for (uint8_t i = 0; i < PWM_CHANNELS; i++)
		global_pwm.target.rgbwu[i] = color->rgbwu[i];

	/* compute correct speed for all channels */
	if (delay == 0)
		delay = 1;
	compute_speed(step, delay);

	/* disable timer */
	fading.running = 0;

}

bool pwm_target_reached(void)
{
	for (uint8_t i = 0; i < PWM_CHANNELS; i++) {
		if (global_pwm.target.rgbwu[i] != global_pwm.current.rgbwu[i])
			return false;
	}

	return true;
}

/* modify current color */
void pwm_modify_rgb(struct rgb_color_offset_t *color, uint8_t step,
		    uint8_t delay)
{
	for (uint8_t i = 0; i < PWM_CHANNELS; i++) {
		int16_t current = global_pwm.target.rgbwu[i];
		current += color->rgbwu[i];

		if (current > 255)
			current = 255;
		if (current < 0)
			current = 0;

		global_pwm.target.rgbwu[i] = LO8(current);
	}

	compute_speed(step, delay);

	/* disable timer */
	fading.running = 0;
}

/** interrupts*/

/** timer1 overflow (=output compare a) interrupt */
ISR(TIMER1_COMPA_vect)
{
	/* set initial timeslot (that is initial value for bitmask */

	/* output new values */
	P_PORT =
	    (P_PORT & ~(PWM_CHANNEL_MASK)) | timeslots_masks[timeslots_read];
	timeslots_read++;
	if (timeslots_read >= PWM_MAX_TIMESLOTS)
		timeslots_read = 0;

	//  P_PORT = 0b00010000;
	//   P_PORT = 0b00000000;

	/* if next timeslot would happen too fast or has already happened, just spinlock */
	while (TCNT1 + 100 > timeslots_tops[timeslots_read]) {
		/* spin until timer interrupt is near enough */
		while (timeslots_tops[timeslots_read] > TCNT1) ;

		/* output new values */
		P_PORT =
		    (P_PORT & ~(PWM_CHANNEL_MASK)) |
		    timeslots_masks[timeslots_read];
		/* read forward to next timeslot */
		timeslots_read++;
		if (timeslots_read >= PWM_MAX_TIMESLOTS)
			timeslots_read = 0;

	}

	/* save values for next interrupt */
	OCR1B = timeslots_tops[timeslots_read];

}

/** timer1 output compare b interrupt */
ISR(TIMER1_COMPB_vect, ISR_ALIASOF(TIMER1_COMPA_vect));
