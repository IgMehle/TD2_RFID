/*
 * app.c
 *
 *  Created on: 21 feb. 2026
 *      Author: ThinkPad
 */

// MY DEFS
#include "my_defs.h"
// PINOUT
#include "pinout.h"
// MY TIMERS
#include "my_timers.h"
// BOARD
#include "board.h"
#include "clock_config.h"
#include "peripherals.h"
#include "pin_mux.h"
// DEVICE
#include "LPC845.h"
// PERIPHERALS
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_swm.h"
#include "fsl_spi.h"
#include "fsl_i2c.h"
#include "fsl_usart.h"
// #include "fsl_iocon.h"	// For Glitch Filter
// UTILITIES
#include "fsl_debug_console.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// DRIVERS
#include "mfrc522_td2.h"
#include "lcd_4bit.h"
#include "ds3231.h"
#include "at24c32.h"
#include "keypad.h"
#include "timers.h"


uint8_t header_config(void)
{
	header_t header;
	uint8_t status = STATUS_OK;
	uint8_t uid[5];

	status = read_header(&header);
	if (status != STATUS_OK) error_msg(status, "Error load_header()");

	///// CONFIGURACION INICIAL /////
	if (strcmp(header.firma, "RFID") != 0) {
	//if(status == STATUS_OK) {

		lcd4_print("WAIT FOR ADMIN", 1);
		// Cargo firma
		sprintf(header.firma, "RFID");
		// Version del firmware
		header.version[0] = FW_VERSION;
		header.version[1] = FW_REV;
		// Escaneo continuamente hasta leer una UID valida
		while (1) {
			PRINTF("Scan new admin...\n");
			if (isCard()) {
				if (readCardSerial(uid)) break;
			}
		}
		// Asigno uid a como admin
		header.admin_uid[0] = uid[0];
		header.admin_uid[1] = uid[1];
		header.admin_uid[2] = uid[2];
		header.admin_uid[3] = uid[3];
		header.admin_uid_len = 4;
		buzzer_beep(100);
		// TODO: escanear admin_pin
		// TODO: asignar settings
	}
	////////////////////////////////
	// Guardar header en memoria
	status = save_header(&header);
	if (status != STATUS_OK) error_msg(status, "Error save_header()");
	return status;
}

uint8_t validar_usuario(uint8_t *uid)
{
	user_t user;
	uint8_t status = STATUS_OK;
	uint16_t id = 0;

	// Leo slots de user buscando match
	// hasta que lea uno vacio
	do {
		status = read_user(&user, id);
		// Comparo uid del user con la leida
		if (uid_compare(uid, user.uid) == 0) {
			PRINTF("[VALIDAR] Usuario %d, autorizado!\n", id);
			return STATUS_OK;
		}
		if (id < 128) id++;
	} while (user.valid == VALID_USER);
	// No hubo match
	PRINTF("[VALIDAR] Acceso denegado.\n");
	// Prendo led rojo
	LED_AUX_ON();
	buzzer_beep(1000);
	on_timer(timers_id.off, TIMER_ONESHOT);
	return STATUS_ERR;
}

uint8_t abrir_puerta(void)
{
	RELAY_ON();
	buzzer_beep(1000);
	on_timer(timers_id.relay, TIMER_ONESHOT);
	return STATUS_OK;
}

uint8_t validar_admin(uint8_t *uid)
{
	// TODO: Tengo que buscar el uid del admin en el header y compararlo con el que lei
	header_t header;
	uint8_t status = STATUS_OK;

	// Levanto el header
	status = read_header(&header);
	if (status != STATUS_OK) error_msg(status, "Error read_header()");

	// Comparo uid leido con el uid admin
	if (uid_compare(uid, header.admin_uid) != STATUS_OK) {
		status = STATUS_ERR;
	}
	return status;
}

uint8_t alta_usuario(void)
{
	uint8_t status = STATUS_OK;
	user_t new = {0};
	uint16_t address = EEPROM_USER_BASE;
	uint16_t index = 0;
	uint8_t bf[2] = {0}; // Ver si funciona igual con un byte individual
	uint8_t uid[5];
	char key = KEY_NONE;

	PRINTF("[ALTA] Buscando slot libre...\n");

	// Leo el primer slot de user
	status = eeprom_read(bf, address, 1);
	if (status != STATUS_OK) error_msg(status, "Error eeprom_read()");

	while (bf[0] == VALID_USER) {
		// Si detecto un user valido, incremento el puntero
		PRINTF("Address 0x%x ocupada\n", address);
		index++;
		address += 0x10;
		// Reviso que no me pase del limite de memoria de usuarios
		if (address < EEPROM_LOGS_BASE) {
			status = eeprom_read(bf, address, 1);
			if (status != STATUS_OK) error_msg(status, "Error eeprom_read()");
		}
		else {
			PRINTF("[ALTA] Memoria llena, cancelando...\n");
			return STATUS_ERR;
		}
	}

	// Slot vacio en index
	PRINTF("Slot vacio en address 0x%x\n", address);
	PRINTF("[ALTA] Escanee tarjeta de nuevo usuario... ");
	// Escaneo continuamente hasta leer una UID valida
	while (1) {
		if (isCard()) {
			if (readCardSerial(uid)) break;
		}
		key = keypad_readkey();
		if (key == 'C') {
			PRINTF("\n[ALTA] Cancelando...\n");
			return STATUS_ERR;
		}
	}
	PRINTF("%02x %02x %02x %02x\n", uid[0], uid[1], uid[2], uid[3]);
	// Copio UID escaneado
	new.uid[0] = uid[0];
	new.uid[1] = uid[1];
	new.uid[2] = uid[2];
	new.uid[3] = uid[3];
//	PRINTF("[ALTA] UID: %02x %02x %02x %02x\n",
//			uid[0], uid[1], uid[2], uid[3]);
	// Ingresar hora de entrada
	PRINTF("[ALTA] Ingrese hora de entrada: ");
	while (1) {
		key = keypad_readkey();
		// Hora valida
		if ('0' <= key && key <= '9') {
			new.hora_entrada[0] = atoi(&key);
			PRINTF("%d:00\n", new.hora_entrada[0]);
			break;
		}
		// CANCELAR
		else if (key == 'C') {
			PRINTF("\n[ALTA] Cancelando...\n");
			return STATUS_ERR;
		}
		// Hora invalida
		else if (key != KEY_NONE) {
			PRINTF("\n[ALTA] Tecla invalida, reingrese: ");
		}
	}
	// Ingresar hora de salida
	PRINTF("[ALTA] Ingrese hora de salida: ");
	while (1) {
		key = keypad_readkey();
		// Hora valida
		if ('0' <= key && key <= '9') {
			new.hora_salida[0] = atoi(&key);
			PRINTF("%d:00\n", new.hora_salida[0]);
			break;
		}
		// CANCELAR
		else if (key == 'C') {
			PRINTF("\n[ALTA] Cancelando...\n");
			return STATUS_ERR;
		}
		// Hora invalida
		else if (key != KEY_NONE) {
			PRINTF("\n[ALTA] Tecla invalida, reingrese:");
		}
	}
	// Parametros del user
	new.valid = VALID_USER;
	new.id = index;
	new.flags = 0x00;

	PRINTF("----- NUEVO USUARIO -----\n");
	PRINTF("ID: %d\n", new.id);
	PRINTF("UID: %02x %02x %02x %02x\n",
				new.uid[0], new.uid[1], new.uid[2], new.uid[3]);
	PRINTF("PIN: %d%d%d%d\n",
					new.pin[0], new.pin[1], new.pin[2], new.pin[3]);
	PRINTF("Hora entrada: %02d:00\n", new.hora_entrada[0]);
	PRINTF("Hora salida: %02d:00\n", new.hora_salida[0]);
	PRINTF("--------------------------\n");

	// Guardo en el slot vacio
	status = save_user(&new, index);
	if (status == STATUS_OK) {
		PRINTF("[ALTA] Usuario guardado con exito\n");
	}
	else error_msg(status, "No se pudo guardar en memoria el usuario.");
	return status;
}

uint8_t baja_usuario(void)
{
	return STATUS_OK;
}

uint8_t info_usuarios(void)
{
	uint8_t status = STATUS_OK;
	user_t user = {0};
	uint16_t address = EEPROM_USER_BASE;
	uint16_t index = 0;
	uint8_t bf[2] = {0}; // Ver si funciona igual con un byte individual

	PRINTF("[INFO] Mostrando tamano de slots:\n");
	PRINTF("\t HEADER: %d\n", sizeof(header_t));
	PRINTF("\t USER: %d\n", sizeof(user_t));
	PRINTF("\t LOG: %d\n", sizeof(log_t));
	PRINTF("[INFO] Mostrando usuarios registrados\n");

	// Escanear usuarios validos e imprimir
	status = read_user(&user, index);
	while (user.valid == VALID_USER) {
		// Imprimo datos del usuario
		PRINTF("----- USUARIO %d -----\n", index);
		PRINTF("UID: %02x %02x %02x %02x\n",
				user.uid[0], user.uid[1], user.uid[2], user.uid[3]);
		PRINTF("PIN: %d%d%d%d\n",
					user.pin[0], user.pin[1], user.pin[2], user.pin[3]);
		PRINTF("Horario: %02d:00 | %02d:00\n\n",
				user.hora_entrada[0], user.hora_salida[0]);
		// Incremento index y address
		index++;
		address += 0x10;
		// Leo nuevo slot
		status = read_user(&user, index);
	}
	PRINTF("[INFO] %d usuarios registrados\n", index);
	return STATUS_OK;
}
/*---------------------------------------------------------------------*/
