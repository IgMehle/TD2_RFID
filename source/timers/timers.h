/*
 * timers.h
 *
 *  Created on: 1 feb. 2026
 *      Author: Ignacio
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include <stdint.h>

// TIMER_T
typedef struct timer {
	uint8_t enabled;
	uint8_t periodic;
	uint8_t (*function)(void);
	uint32_t count;
	uint32_t time;
} timer_t;

#define TIMER_PERIODIC	1
#define TIMER_ONESHOT	0
#define MAX_N_TIMERS	16

// TIMERS FUNCTIONS
void init_timers(timer_t *my_timers, uint8_t n);
uint8_t give_timer(uint32_t time, uint8_t (*function)());
void on_timer(uint8_t id, uint8_t periodic);
void pause_timer(uint8_t id);
void continue_timer(uint8_t id);
void reset_timer(uint8_t id);
void resize_timer(uint8_t id, uint32_t time);
void off_timer(uint8_t id);
void refresh_timers(void);
timer_t get_timer_status(uint8_t id);

#endif /* TIMERS_H_ */
