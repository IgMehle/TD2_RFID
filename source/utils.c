/*
 * utils.c
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

const char *estados_names[] = {
	"IDLE",
	"RFID_READ",
	"PIN_READ",
	"VALIDAR_UID",
	"VALIDAR_PIN",
	"DENEGADO",
	"ABRIR_PUERTA",
	"PUERTA_ABIERTA",
	"PUERTA_CERRADA",
	"MENU_ADMIN",
	"ALTA",
	"BAJA",
	"EDITAR",
	"DUMP_LOG",
	"CLEAR_LOG",
	"INFO_USERS"};

void buzzer_beep(uint32_t ms)
{
	resize_timer(timers_id.off, ms);
	BUZZER_ON();
	on_timer(timers_id.off, TIMER_ONESHOT);
}

uint8_t read_header(header_t *header)
{
	uint8_t status = STATUS_OK;
	uint8_t bf[sizeof(header_t)];
	uint8_t i = 0;
	uint8_t j = 0;

	// Leo el header
	status = eeprom_read(bf, EEPROM_HEADER_BASE, 32);
	if (status != STATUS_OK) error_msg(status, "Error eeprom_read()");

//	PRINTF("HEADER_READ: ");
//	for (i = 0; i < 32; i++) PRINTF("%02x ", bf[i]);
//	PRINTF("\n");

	// Cargo campos desde el buffer
	// firma
	for (j = 0; j < 5; j++) {
		header->firma[j] = bf[i + j];
	}
	// version
	header->version[0] = bf[5];
	header->version[1] = bf[6];
	// admin_uid_len
	header->admin_uid_len = bf[7];
	// admin_uid
//	i = 8;
//	for (j = 0; j < 10; j++) {
//		header->admin_uid[j] = bf[i + j];
//	}
	header->admin_uid[0] = bf[8];
	header->admin_uid[1] = bf[9];
	header->admin_uid[2] = bf[10];
	header->admin_uid[3] = bf[11];
	// admin pin
//	i = 18;
//	for (j = 0; j < 4; j++) {
//		header->admin_pin[j] = bf[i + j];
//	}
	header->admin_pin[0] = bf[18];
	header->admin_pin[1] = bf[19];
	header->admin_pin[2] = bf[20];
	header->admin_pin[3] = bf[21];
	// settings
	i = 22;
	for (j = 0; j < 10; j++) {
		header->settings[j] = bf[i + j];
	}
	return status;
}

uint8_t save_header(header_t *header)
{
	uint8_t status = STATUS_OK;
	uint8_t bf[sizeof(header_t)];
	uint8_t i = 0;
	uint8_t j = 0;

	// Cargo datos en buffer
	// firma
	for (j = 0; j < 5; j++) {
		bf[i + j] = header->firma[j];
	}
	// version
	bf[5] = header->version[0];
	bf[6] = header->version[1];
	// admin_uid_len
	bf[7] = header->admin_uid_len;
	// admin_uid
//	i = 8;
//	for (j = 0; j < 10; j++) {
//		bf[i + j] = header->admin_uid[j];
//	}
	bf[8] = header->admin_uid[0];
	bf[9] = header->admin_uid[1];
	bf[10] = header->admin_uid[2];
	bf[11] = header->admin_uid[3];
	// admin pin
//	i = 18;
//	for (j = 0; j < 4; j++) {
//		bf[i + j] = header->admin_pin[j];
//	}
	bf[18] = header->admin_pin[0];
	bf[19] = header->admin_pin[1];
	bf[20] = header->admin_pin[2];
	bf[21] = header->admin_pin[3];
	// settings
	i = 22;
	for (j = 0; j < 10; j++) {
		bf[i + j] = header->settings[j];
	}

//	PRINTF("SAVE_HEADER: ");
//	for (i = 0; i < 32; i++) PRINTF("%02x ", bf[i]);
//	PRINTF("\n");

	// Escribo en eeprom header
	status = eeprom_write(bf, EEPROM_HEADER_BASE, 32);
	if (status != STATUS_OK) error_msg(status, "Error eeprom_read");

	return status;
}

uint8_t read_user(user_t *user, uint16_t id)
{
	uint8_t status = STATUS_OK;
	uint8_t bf[sizeof(user_t)];

	// Calculo direccion en memoria del usuario
	uint16_t address = EEPROM_USER_BASE + id * sizeof(user_t);

	// Levanto usuario de eeprom
	status = eeprom_read(bf, address, sizeof(user_t));
	if (status != STATUS_OK) error_msg(status, "Error eeprom_read()");

	// Cargo campos desde el buffer
	// valid
	user->valid = bf[0];
	// flags
	user->flags = bf[1];
	// index: 0: LSB, 1: MSB
	user->id = (bf[2] << 8) + bf[3];
	// uid
	user->uid[0] = bf[4];
	user->uid[1] = bf[5];
	user->uid[2] = bf[6];
	user->uid[3] = bf[7];
	// pin
	user->pin[0] = bf[8];
	user->pin[1] = bf[9];
	user->pin[2] = bf[10];
	user->pin[3] = bf[11];
	// hora entrada
	user->hora_entrada[0] = bf[12];
	user->hora_entrada[1] = bf[13];
	// hora salida
	user->hora_salida[0] = bf[14];
	user->hora_salida[1] = bf[15];

	return STATUS_OK;
}

uint8_t save_user(user_t *user, uint16_t id)
{
	uint8_t status = STATUS_OK;
	uint8_t bf[sizeof(user_t)];

	// Cargo campos al buffer
	// Valid
	bf[0] = user->valid;
	// flags
	bf[1] = user->flags;
	// id, 0: LSB, 1: MSB
	bf[2] = (uint8_t)(user->id & 0x00FF);
	bf[3] = (uint8_t)(user->id >> 8);
	// uid
	bf[4] = user->uid[0];
	bf[5] = user->uid[1];
	bf[6] = user->uid[2];
	bf[7] = user->uid[3];
	// pin
	bf[8] = user->pin[0];
	bf[9] = user->pin[1];
	bf[10] = user->pin[2];
	bf[11] = user->pin[3];
	// hora entrada
	bf[12] = user->hora_entrada[0];
	bf[13] = user->hora_entrada[1];
	// hora salida
	bf[14] = user->hora_salida[0];
	bf[15] = user->hora_salida[1];

	// Calculo direccion en memoria del usuario
	uint16_t address = EEPROM_USER_BASE + id * sizeof(user_t);
	// TODO: Chequeo que no me salga del limite de memoria
	// Guardo en eeprom
	status = eeprom_write(bf, address, sizeof(user_t));
	if (status != STATUS_OK) error_msg(status, "Error eeprom_write()");

	return STATUS_OK;
}

uint8_t read_log(log_t *log, uint16_t index)
{
	return STATUS_OK;
}

uint8_t save_log(log_t *log, uint16_t index)
{
	return STATUS_OK;
}

uint8_t uid_compare(uint8_t *uid1, uint8_t *uid2)
{
	uint8_t match = RET_OK;
	// Itero sobre los bytes del uid
	for (uint8_t i = 0; i < 4; i++) {
		// Al primer byte que no coincide, salgo y devuelvo -1
		if (uid1[i] != uid2[i]) {
			match = RET_FALSE;
			break;
		}
	}
	return match;
}

void error_msg(uint8_t error_code, char *text)
{
	while (1) {
		PRINTF("\n[FAILED] Code %d, %s", error_code, text);
		delay_ms(1000);
	}
}

void state_switch_print(estados_t from, estados_t to)
{
	PRINTF("[STATE] %s -> %s\n", estados_names[from], estados_names[to]);
}
