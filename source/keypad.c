/*
 * keypad.c
 *
 *  Created on: 2 feb. 2026
 *      Author: Ignacio
 */

#include "keypad.h"
#include "keypad_port.h"

// Variables globales para manejo del teclado
static const char keys[] = { '1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D' };
static volatile key_t key = {KEY_NONE, 0, KEY_NONE};
static volatile kp_state_t kp_state = KP_IDLE;
static volatile char key_bf = KEY_NONE;
//typedef struct scan_bf {
//	uint8_t bf[128];
//	uint8_t count;
//} scan_bf_t;
//volatile scan_bf_t scan_bf = { {0}, 0 };

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

uint8_t keypad_update(void)
{
	// Escaneo teclado
	uint8_t scan = keypad_scan();
	// APPEND a scan_bf para debug
//	if (scan_bf.count < 129) {
//		scan_bf.bf[scan_bf.count] = scan;
//		scan_bf.count++;
//	}

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
			if (key_bf == KEY_NONE) key_bf = keys[key.valid_key];
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
	return 0;
}

char keypad_readkey(void)
{
	char k;
	// __disable_irq();
	// Tomo la tecla del buffer
	k = key_bf;
	key_bf = KEY_NONE;
	// DEBUG de secuencia de lecturas de keypad_scan
	// __enable_irq();
	return k;
}

uint8_t keypad_readpin(char first_dig, char *pin, void (*beep_func)(uint32_t))
{
	uint8_t i = 0;
	char k;

	// Validacion
	if (first_dig < '0' || first_dig > '9') return PIN_CANCEL;
	// Guardo el primer digito
	pin[0] = first_dig;
	pin[1] = '\0';
	// Muestro pin
	keypad_show_pin(pin, PIN_SHOW);
	// Incremento indice
	i++;

	while(1) {
		// Leo tecla
		k = keypad_readkey();
		if (k == KEY_NONE) continue;
		// Llamo a beep_func() si la tecla es valida
		beep_func(20);

		// Cancelacion
		if (k == 'C') {
			pin[0] = '\0';
			return PIN_CANCEL;
		}

		// Borrado
		if (k == 'B') {
			// Si no tengo 0 digitos en el string
			if (i > 0) {
				i--;
				pin[i] = '\0';
				// Muestro pin
				keypad_show_pin(pin, PIN_SHOW);
			}
		}

		// Si la tecla es numerica la agrego al pin
		if (k >= '0' && k <= '9') {
			if (i < PIN_LEN) {
				pin[i] = k;
				// Incremento indice
				i++;
				pin[i] = '\0';
				// Muestro pin
				keypad_show_pin(pin, PIN_SHOW);
				// Si llego a 4, tengo el pin completo
				if (i == PIN_LEN) return PIN_OK;
			}
		}
	}
}


