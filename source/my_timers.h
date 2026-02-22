/*
 * my_timers.h
 *
 *  Created on: 21 feb. 2026
 *      Author: ThinkPad
 */

#ifndef MY_TIMERS_H_
#define MY_TIMERS_H_

// TIMERS ID
typedef struct {
	uint8_t ledrun;
	uint8_t keypad;
	uint8_t off;
	uint8_t relay;
} timers_id_t;

// VARIABLE
extern timers_id_t timers_id;

#endif /* MY_TIMERS_H_ */
