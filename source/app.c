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

		// char str[17];
		// sprintf(str, "%02x %02x %02x %02x", uid[0], uid[1], uid[2], uid[3]);
		//lcd4_print("ADMIN", 1);
		// lcd4_print(str, 2);
		// PRINTF("NEW ADMIN: %s\n", str);
		// Asigno el uid como admin
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

uint8_t validar_admin(uint8_t *uid)
{
	// TODO: Tengo que buscar el uid del admin en el header y compararlo con el que lei
	header_t header;
	uint8_t status = STATUS_OK;
	uint8_t comp;

	// Levanto el header
	status = read_header(&header);
	if (status != STATUS_OK) error_msg(status, "Error read_header()");

//	PRINTF("UID: %02x %02x %02x %02x\n",
//			uid[0], uid[1], uid[2], uid[3]);
//	PRINTF("ADMIN UID: %02x %02x %02x %02x\n",
//				header.admin_uid[0],
//				header.admin_uid[1],
//				header.admin_uid[2],
//				header.admin_uid[3]);
	// Comparo uid leido con el uid admin
	if (uid_compare(uid, header.admin_uid) != STATUS_OK) {
		status = STATUS_ERR;
	}
	return status;
}

uint8_t alta_usuario(void)
{
	uint8_t status = STATUS_OK;
	uint16_t address = EEPROM_USER_BASE;
	uint16_t index = 0;
	uint8_t bf[2] = {0}; // Ver si funciona igual con un byte individual

	PRINTF("[ALTA] Buscando slot libre...\n");

	// Leo el primer slot de user
//	status = eeprom_read(bf, address, 1);
//	if (status != STATUS_OK) error_msg(status, "Error eeprom_read()");
//
//	while (bf[0] == VALID_USER) {
//		index++;
//		address += sizeof(user_t);
//		if (address < EEPROM_LOGS_BASE) {
//			status = eeprom_read(bf, address, 1);
//			if (status != STATUS_OK) error_msg(status, "Error eeprom_read()");
//		}
//		else {
//			PRINTF("[ALTA] Memoria llena, cancelando...");
//			return STATUS_ERR;
//		}
//	}

	// Slot vacio en index

	return status;
}

uint8_t baja_usuario(void)
{
	return STATUS_OK;
}

uint8_t info_usuarios(void)
{
	return STATUS_OK;
}
/*---------------------------------------------------------------------*/
