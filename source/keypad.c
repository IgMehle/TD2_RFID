/*
 * keypad.c
 *
 *  Created on: 2 feb. 2026
 *      Author: Ignacio
 */

#include "keypad.h"

// Variables para manejo del teclado
//const char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
//  { '1','2','3','A' },
//  { '4','5','6','B' },
//  { '7','8','9','C' },
//  { '*','0','#','D' }
//};
const char keys[] = { '1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D' };
volatile key_t key = {KEY_NONE, KEY_NONE, 0};
volatile key_event_t key_event = {KEY_EVENT_NONE, KEY_NONE};
static volatile char key_ready = 0;

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
			// Guardo la ultima tecla valida
			uint8_t last_valid = key.valid_key;
			// Actualizo la tecal leida como valida
			key.valid_key = scan;

			// Si detecto una tecla valida
			// es un evento de KEY DOWN
			if (last_valid == KEY_NONE && scan != KEY_NONE) {
				key_event.type = KEY_EVENT_DOWN;
				key_event.key = scan;
			}
			// Si no, es un evento de KEY UP
			// Guardo la tecla que se levanto
			else if (last_valid != KEY_NONE && scan == KEY_NONE) {
				key_event.type = KEY_EVENT_UP;
				key_event.key = last_valid;
			}
			else {
				// last_valid != NONE y scan != NONE
				// No acepto nueva hasta ver NONE estable
				key_event.type = KEY_EVENT_NONE;
				key_event.key = last_valid;
				key.valid_key = KEY_NONE;
			}
			// Publico la tecla para que readkey() la consuma una vez
			if (key_event.type == KEY_EVENT_DOWN) {
			    key_ready = keys[key_event.key];
			}
		}
	}
}

char keypad_readkey(void)
{
	char k = key_ready;
	if (k != 0) {
		key_ready = 0;
		// Limpio el evento
		key_event.type = KEY_EVENT_NONE;
		key_event.key = KEY_NONE;
	}
	return k;
}
