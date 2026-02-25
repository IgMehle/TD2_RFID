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
	if (status != STATUS_OK) error_msg(status, "Error eeprom_read");

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
	i = 8;
	for (j = 0; j < 10; j++) {
		header->admin_uid[j] = bf[i + j];
	}
	// admin pin
	i = 18;
	for (j = 0; j < 4; j++) {
		header->admin_pin[j] = bf[i + j];
	}
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
	i = 8;
	for (j = 0; j < 10; j++) {
		bf[i + j] = header->admin_uid[j];
	}
	// admin pin
	i = 18;
	for (j = 0; j < 4; j++) {
		bf[i + j] = header->admin_pin[j];
	}
	// settings
	i = 22;
	for (j = 0; j < 10; j++) {
		bf[i + j] = header->settings[j];
	}

	// Escribo en eeprom header
	status = eeprom_write(bf, EEPROM_HEADER_BASE, 32);
	if (status != STATUS_OK) error_msg(status, "Error eeprom_read");

	return status;
}

uint8_t read_user(user_t *user)
{
	return STATUS_OK;
}

uint8_t save_user(user_t *user)
{
	return STATUS_OK;
}

uint8_t read_log(log_t *log)
{
	return STATUS_OK;
}

uint8_t save_log(log_t *log)
{
	return STATUS_OK;
}

void error_msg(uint8_t error_code, char *text)
{
	while (1) {
		PRINTF("\n[FAILED] Code %d, %s", error_code, text);
		delay_ms(1000);
	}
}
