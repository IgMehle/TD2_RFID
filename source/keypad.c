/*
 * keypad.c
 *
 *  Created on: 2 feb. 2026
 *      Author: Ignacio
 */

#include "keypad.h"

// Variables globales para manejo del teclado
static const char keys[] = { '1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D' };
static volatile key_t key = {KEY_NONE, 0, KEY_NONE};
static volatile kp_state_t kp_state = KP_IDLE;
static volatile char key_bf = 0;

uint8_t keypad_scan(void)
{
	// Escaneo fila por fila
	for (uint8_t r = 0; r < KEYPAD_ROWS; r++) {
		// Activo fila r
		keypad_row_write(r, 0);
		// Espero 1 o 2 us a que se estabilice la señal
		keypad_row_delay();
		// Leo columnas
		for (uint8_t c = 0; c < KEYPAD_COLS; c++) {
			// Si detecto columna activa
			if (keypad_col_read(c) == COL_ACTIVE) {
				// Desactivo la fila r antes de salir
				keypad_row_write(r, 1);
				// Devuelvo numero de tecla
				return (uint8_t)(r * KEYPAD_COLS + c);
			}
		}
		// Desactivo fila r
		keypad_row_write(r, 1);
	}
	return KEY_NONE;
}

void keypad_update(void)
{
	// Escaneo teclado
	uint8_t scan = keypad_scan();

	// Actualizo contador de muestras si se repite la tecla
	if (scan == key.last_key) {
		// Actualizo contador de repeticiones
		if (key.counter < (DEBOUNCE_N - 1)) {
			key.counter++;
		}
	}
	else {
		// Guardo tecla nueva
		key.last_key = scan;
		key.counter = 0;
	}

	// Si la tecla se repitio N veces
	if (key.counter == (DEBOUNCE_N - 1)) {
		// Si la tecla no es igual a la ultima tecla valida
		// Significa que detecto un cambio
		if (scan != key.valid_key) {
			// Actualizo la tecla leida como valida
			key.valid_key = scan;
		}
	}

	// ESTADO DEL KEYPAD
	switch (kp_state) {
	case KP_IDLE:
		if (key.valid_key != KEY_NONE) {
			// Tecla pulsada
			// Guardo en buffer si esta vacio
			if (key_bf == 0) key_bf = keys[key.valid_key];
			// Cambio a estado pressed
			kp_state = KP_PRESSED;
		}
		break;
	case KP_PRESSED:
		if (key.valid_key == KEY_NONE) {
			// Solte la tecla, vuelvo a idle
			kp_state = KP_IDLE;
		}
		break;
	default:
		kp_state = KP_IDLE;
		break;
	}
}

char keypad_readkey(void)
{
	char k;
	__disable_irq();
	// Tomo la tecla del buffer
	k = key_bf;
	key_bf = 0;
	__enable_irq();
	return k;
}

void keypad_readpin(char num1)
{

}
