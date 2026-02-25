/*
 * keypad_port.h
 *
 *  Created on: 23 feb. 2026
 *      Author: Ignacio
 */

#ifndef KEYPAD_PORT_H_
#define KEYPAD_PORT_H_

#include "keypad.h"
/*-------------------------------------------------------------------*/
// LPC845
#include "fsl_gpio.h"
#include "fsl_debug_console.h"
/*-------------------------------------------------------------------*/
// Ajustar delay segun frecuencia de clock
#define ROW_DELAY	10

// IO
void keypad_row_write(uint8_t row, uint8_t level);
uint8_t keypad_col_read(uint8_t col);
void keypad_row_delay(void);

#endif /* KEYPAD_PORT_H_ */
