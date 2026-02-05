/*
 * keypad_port.c
 *
 *  Created on: 3 feb. 2026
 *      Author: Ignacio
 */

#include "keypad.h"
// MY DEFS
#include "my_defs.h"

// Ajustar delay segun frecuencia de clock
#define ROW_DELAY	10

const uint8_t rows[KEYPAD_ROWS] = { KBD_R0_PIN, KBD_R1_PIN, KBD_R2_PIN, KBD_R3_PIN };
const uint8_t columns[KEYPAD_COLS] = { KBD_C0_PIN, KBD_C1_PIN, KBD_C2_PIN, KBD_C3_PIN };

static inline void keypad_row_write(uint8_t row, uint8_t level)
{
	GPIO_PinWrite(GPIO, 0, rows[row], level);
}

static inline uint8_t keypad_col_read(uint8_t col)
{
	return (uint8_t) (GPIO_PinRead(GPIO, 0, columns[col]));
}

static inline void keypad_show_pin(char *pin, uint8_t hide)
{
	char ast[PIN_LEN + 1];
	uint8_t n = 0;

	while (pin[n] != '\0' && n < PIN_LEN) n++;

	if (hide == PIN_SHOW) {
		// mostrar pin
		lcd4_print(pin, PIN_LINE);
		// printf("\r%s", pin);
	}
	else {
		for (uint8_t i = 0; i < n; i++) ast[i] = '*';
		ast[n] = '\0';
		lcd4_print(ast, PIN_LINE);
		// printf("\r%s", ast);
	}
}

static inline void keypad_row_delay(void)
{
	for (uint8_t i = 0; i < ROW_DELAY; i++);
}
