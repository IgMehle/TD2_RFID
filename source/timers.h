/*
 * timers.h
 *
 *  Created on: 1 feb. 2026
 *      Author: Ignacio
 */

#ifndef TIMERS_H_
#define TIMERS_H_

// MY_DEFS
#include "my_defs.h"

// TIMER_T
typedef struct timer {
	uint8_t name;
	uint8_t enabled;
	uint8_t periodic;
	uint8_t event;
	uint8_t (*function)(void);
	uint32_t count;
	uint32_t time;
} timer_t;

#define N_TIMERS		8
#define TIMER_PERIODIC	1
#define TIMER_ONESHOT	0

// TIMERS FUNCTIONS
void init_timers(void);
uint8_t give_timer(uint32_t time, uint8_t (*function)());
void on_timer(uint8_t name, uint8_t periodic);
void pause_timer(uint8_t name);
void continue_timer(uint8_t name);
void reset_timer(uint8_t name);
void resize_timer(uint8_t name, uint32_t time);
void off_timer(uint8_t name);
void refresh_timers(void);
timer_t get_timer_status(uint8_t name);

#endif /* TIMERS_H_ */
