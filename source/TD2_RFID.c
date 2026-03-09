/*
 * Copyright 2016-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    TD2_RFID.c
 * @brief   Application entry point.
 */

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

/* TODO: insert other include files here. */
// MY DEFS
#include "my_defs.h"
// DRIVERS
#include "mfrc522_td2.h"
#include "lcd_4bit.h"
#include "ds3231.h"
#include "at24c32.h"
#include "keypad.h"
#include "timers.h"
// PINOUT
#include "pinout.h"
// MY TIMERS
#include "my_timers.h"

/* TODO: insert other definitions and declarations here. */
//---------------------------------------------------------------//
// Global Variables
//---------------------------------------------------------------//
volatile uint32_t count_mseg = 0;
volatile uint8_t led_run = 0;
volatile uint16_t toggles = 0;
uint8_t serNum[5];

// TIMERS ID
timers_id_t timers_id;

//---------------------------------------------------------------//
// Prototypes
//---------------------------------------------------------------//
void test_keypad(void);
void print_date(void);
void save_time(void);
void scan_rfid(void);

// TIMER CALLBACKS
uint8_t toggle_ledrun(void);
uint8_t off_buzzer(void);
uint8_t off_relay(void);
uint8_t off_ledaux(void);

/*
 * @brief   Application entry point.
 */
int main(void)
{
	BOARD_BootClockFRO24M();
	BOARD_InitDEBUG_UARTPins();
	BOARD_InitDebugConsole();
	(void) SysTick_Config(SystemCoreClock/1000);

	// BOARD INIT
	board_gpio_init();
	board_i2c_init();
	board_spi_init();
	// board_uart_init();
	LCD_BL_ON();
//	BUZZER_ON();
//	delay_ms(500);
//	BUZZER_OFF();

	lcd_4bit_init();
	lcd4_print("Hola Nacho!", 1);
	//char s_toggles[17];

	uint8_t stat_i2c = 0;
	rtc_t fyh;
	uint8_t fyh_bytes[7];

	// Levanto fyh de eeprom
	stat_i2c = eeprom_read(fyh_bytes, 0x0030, sizeof(fyh_bytes));
	if (stat_i2c != STATUS_OK) error_msg(stat_i2c, "Error eeprom_read()");

	// Convierto a rtc_t
	rtc_bytes2time(fyh_bytes, &fyh);

	// Cargo fyh en RTC
	stat_i2c = rtc_load(fyh);
	if (stat_i2c != STATUS_OK) {
		while (1) {
			PRINTF("\n[I2C] Error rtc_load()");
			delay_ms(1000);
		}
	}

	// INIT MFRC522
	mfrc522_init();

	// TIMERS
	init_timers();
	// TIMERS PERIODICOS
	timers_id.ledrun = give_timer(500, toggle_ledrun);
	on_timer(timers_id.ledrun, TIMER_PERIODIC);
	timers_id.keypad = give_timer(5, keypad_update);
	on_timer(timers_id.keypad, TIMER_PERIODIC);
	// TIMERS RETARDO DE APAGADO
	timers_id.off = give_timer(20, off_buzzer);
	timers_id.relay = give_timer(1000, off_relay);
	timers_id.ledaux = give_timer(1000, off_ledaux);

	// uint8_t loops = 0;
	// CONFIG HEADER
	header_config();

	uint8_t status = STATUS_OK;
	estados_t estado = IDLE;
	uint8_t uid[5];
	char opc = KEY_NONE;
	// LOOP DE EJECUCION
    while (1) {
    	switch(estado) {
    	/*----- IDLE ---------------------------*/
    	case IDLE:
    		if (isCard()) {
    			if (readCardSerial(uid)) {
    				estado = VALIDAR_UID;
    				state_switch_print(IDLE, VALIDAR_UID);
    				break;
    			}
    		}
    		else estado = IDLE;
    		break;
    	/*----- VALIDAR UID --------------------*/
    	case VALIDAR_UID:

    		// Validar admin
    		if (validar_admin(uid) == STATUS_OK) {
    			estado = MENU_ADMIN;
    			state_switch_print(VALIDAR_UID, MENU_ADMIN);
    			break;
    		}
    		// Validar user
    		else if (validar_usuario(uid) == STATUS_OK) {
				estado = ABRIR_PUERTA;
				state_switch_print(VALIDAR_UID, ABRIR_PUERTA);
				break;
			}
    		else estado = IDLE;
    		state_switch_print(VALIDAR_UID, IDLE);
    		break;
    	/*----- ABRIR PUERTA -------------------*/
    	case ABRIR_PUERTA:
    		abrir_puerta();
			estado = PUERTA_ABIERTA;
			state_switch_print(ABRIR_PUERTA, PUERTA_ABIERTA);
			break;
		/*----- ABRIR PUERTA -------------------*/
    	case PUERTA_ABIERTA:
			if (PULSADOR() == 0) {
				estado = PUERTA_CERRADA;
				state_switch_print(PUERTA_ABIERTA, PUERTA_CERRADA);
			}
			else estado = PUERTA_ABIERTA;
			break;
		/*----- PUERTA CERRADA -----------------*/
    	case PUERTA_CERRADA:
    		//cerrar_puerta();
    		RELAY_OFF();
    		estado = IDLE;
    		state_switch_print(PUERTA_CERRADA, IDLE);
    		break;
    	/*----- MENU ADMIN ---------------------*/
		case MENU_ADMIN:
			opc = keypad_readkey();
			switch (opc) {
			case '0':
				state_switch_print(MENU_ADMIN, IDLE);
				estado = IDLE;
				break;
			case '1':
				estado = ALTA;
				state_switch_print(MENU_ADMIN, ALTA);
				break;
			case '2':
				estado = BAJA;
				state_switch_print(MENU_ADMIN, BAJA);
				break;
			case '3':
				estado = INFO_USERS;
				state_switch_print(MENU_ADMIN, INFO_USERS);
				break;
			default:
				estado = MENU_ADMIN;
				break;
			}
			break;
		/*----- ALTA ---------------------------*/
		case ALTA:
			status = alta_usuario();
			estado = IDLE;
			state_switch_print(ALTA, IDLE);
			break;
		/*----- BAJA ---------------------------*/
		case BAJA:
			status = baja_usuario();
			estado = IDLE;
			state_switch_print(BAJA, IDLE);
			break;
		/*----- INFO USERS ---------------------*/
		case INFO_USERS:
			status = info_usuarios();
			estado = IDLE;
			state_switch_print(INFO_USERS, IDLE);
			break;
		/*----- DEFAULT ------------------------*/
		default:
			estado = IDLE;
			break;
		}

    	/*----- HARDWARE TEST ------------------*/
    	// PRINTF("\nTecla: ");
    	// test_keypad();
    	// itoa(toggles, s_toggles, 10);
    	// lcd4_print(s_toggles, 2);
    	//scan_rfid();
    	// loops++;
    	// if (loops > 9) {
    	// 	print_date();
    	// 	loops = 0;
    	// }
    	// delay_ms(100);
    }
    return 0;
}

/* --------------------------------------------------------------
 * INTERRUPTIONS
 * -------------------------------------------------------------- */
void SysTick_Handler(void)
{
	// soft timers
    refresh_timers();
    // delay_ms()
    if (count_mseg > 0) count_mseg--;
}

/* --------------------------------------------------------------
 * FUNCTIONS
 * -------------------------------------------------------------- */
void delay_ms(uint32_t ms)
{
	count_mseg = ms;
    while(count_mseg > 0);
}

uint8_t toggle_ledrun(void)
{
	// Toggle led
	led_run ^= 1;
	GPIO_PinWrite(GPIO, 1, LEDRUN_PIN, led_run);
	toggles++;
	return 0;
}

void test_keypad(void)
{
	static char tecla;
	static uint8_t pin_stat;
	char pin[5];
	tecla = keypad_readkey();
	if (tecla != KEY_NONE) {
		PRINTF("%c", tecla);
		buzzer_beep(20);
	}
	if (tecla >= '0' && tecla <= '9') {
		pin_stat = keypad_readpin(tecla, pin, buzzer_beep);
	}
}

void print_date(void)
{
	rtc_t date;
	uint8_t date_bytes[sizeof(rtc_t)];
	uint8_t status = 0;
	char date_str[17];

	status = rtc_read(&date);
	if (status != STATUS_OK) {
		while (1) {
			PRINTF("\n[I2C] Error rtc_read()");
			delay_ms(1000);
		}
	}

	if (PULSADOR() == 0) {
		BUZZER_ON();
		rtc_time2bytes(date, date_bytes);
		status = eeprom_write(date_bytes, 0x0030, sizeof(date_bytes));
		if (status != STATUS_OK) {
			while (1) {
				PRINTF("\n[I2C] Error eeprom_write()");
				delay_ms(1000);
			}
		}
		//PRINTF("\n[OK] Saved time on 0x0010");
		delay_ms(50);
		BUZZER_OFF();
	}

	sprintf(date_str, "%02d/%02d %02d:%02d:%02d",
			date.day, date.month,
			date.hour, date.min, date.sec);
	lcd4_print(date_str, 2);
	//PRINTF("\n[OK] Date: %s", date_str);
}

uint8_t off_buzzer(void)
{
	BUZZER_OFF();
	return 0;
}

void scan_rfid(void)
{
	static char s_uid[17];

	if (isCard()) {
		if (readCardSerial(serNum)) {
			sprintf(s_uid, "%02x %02x %02x %02x",
					serNum[0],
					serNum[1],
					serNum[2],
					serNum[3]);

			RELAY_ON();
			on_timer(timers_id.relay, TIMER_ONESHOT);
			buzzer_beep(100);

			lcd4_print(s_uid, 1);
			PRINTF("\n[RFID] %s", s_uid);
		}
	}
}

uint8_t off_relay(void)
{
	RELAY_OFF();
	return 0;
}

uint8_t off_ledaux(void)
{
	LED_AUX_OFF();
	return 0;
}
