/*
 * keypad.h
 *
 *  Created on: 2 feb. 2026
 *      Author: Ignacio
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <stdint.h>
#include <ctype.h>

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4
#define KEY_NONE	0xFF
#define DEBOUNCE_N  4   // 4 muestras * 5ms = 20ms
#define COL_ACTIVE	1
#define PIN_LEN   	4
#define PIN_OK    	0
#define PIN_CANCEL 	0xFF
#define PIN_SHOW	0
#define PIN_HIDE	1
#define PIN_LINE	1

typedef struct key {
	uint8_t last_key;
	uint8_t counter;
	uint8_t valid_key;
} key_t;

typedef enum kp_state {
	KP_IDLE = 0,
	KP_PRESSED
} kp_state_t;

// IO
void keypad_row_write(uint8_t row, uint8_t level);
uint8_t keypad_col_read(uint8_t col);
void keypad_show_pin(char *pin, uint8_t hide);
void keypad_row_delay(void);

// FUNCTIONS
uint8_t keypad_scan(void);
void keypad_update(void);
char keypad_readkey(void);
uint8_t keypad_readpin(char first_dig, char *pin);

#endif /* KEYPAD_H_ */
