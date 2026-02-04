/*
 * keypad.h
 *
 *  Created on: 2 feb. 2026
 *      Author: Ignacio
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <stdint.h>

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4
#define KEY_NONE	0xFF
#define DEBOUNCE_N  4   // 4 muestras * 5ms = 20ms
#define COL_ACTIVE	1


typedef enum {
    KEY_EVENT_NONE = 0,
    KEY_EVENT_DOWN,
    KEY_EVENT_UP
} key_event_type_t;

typedef struct {
	uint8_t last_key;
	uint8_t valid_key;
	uint8_t counter;
} key_t;

typedef struct {
	key_event_type_t type;
	uint8_t key;
} key_event_t;

// IO
static inline void keypad_row_write(uint8_t row, uint8_t level);
static inline uint8_t keypad_col_read(uint8_t col);
static inline void keypad_row_delay(void);

// FUNCTIONS
uint8_t keypad_scan(void);
void keypad_update(void);
char keypad_readkey(void);
void keypad_readpin(void);

#endif /* KEYPAD_H_ */
