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
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "my_defs.h"
#include "mfrc522_td2.h"
#include "mfrc522_td2_port.h"
#include "lcd_4bit.h"
#include "lcd_4bit_port.h"
#include "ds3231.h"
#include "at24c32.h"
#include "keypad.h"
#include "timers.h"
/* TODO: insert other definitions and declarations here. */

//---------------------------------------------------------------//
// Global Variables
//---------------------------------------------------------------//
volatile uint32_t count_mseg = 0;
volatile uint8_t led_run = 0;
volatile uint16_t toggles = 0;
volatile unsigned char serNum[5];

// TIMERS NAMES
uint8_t timer_ledrun;
uint8_t timer_keypad;
uint8_t timer_off;
//---------------------------------------------------------------//
// Prototypes
//---------------------------------------------------------------//
void delay_ms(uint32_t ms);
void test_keypad(void);
void print_date(void);
void save_time(void);
void scan_rfid(void);

// TIMER CALLBACKS
uint8_t toggle_ledrun(void);
uint8_t off_buzzer(void);

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
	gpio_init();
	i2c_init();
	// spi_init();
	// uart_init();
//	LCD_BL_ON();
//	BUZZER_ON();
//	delay_ms(500);
//	BUZZER_OFF();

	lcd_4bit_init();
	lcd4_print("Hola Nacho!", 1);
	char s_toggles[17];

	uint8_t stat_i2c = 0;
	rtc_t fyh;
	uint8_t fyh_bytes[7];

	// init test
	fyh.sec = 0;
	fyh.min = 0;
	fyh.hour = 12;
	fyh.weekday = 1;
	fyh.day = 16;
	fyh.month = 2;
	fyh.year = 26;

	// Guardo primera fyh en eeprom
	rtc_time2bytes(fyh, fyh_bytes);

	stat_i2c = eeprom_write(fyh_bytes, 0x0010, sizeof(fyh_bytes));
	if (stat_i2c != STATUS_OK) {
		while (1) {
			PRINTF("\n[I2C] Error eeprom_write()");
			delay_ms(1000);
		}
	}

	// Levanto fyh de eeprom
	stat_i2c = eeprom_read(fyh_bytes, 0x0010, sizeof(fyh_bytes));
	if (stat_i2c != STATUS_OK) {
		while (1) {
			PRINTF("\n[I2C] Error eeprom_read()");
			delay_ms(1000);
		}
	}

	// Convierto a rtc_t
	fyh.sec 	= fyh_bytes[0];
	fyh.min 	= fyh_bytes[1];
	fyh.hour 	= fyh_bytes[2];
	fyh.weekday = fyh_bytes[3];
	fyh.day 	= fyh_bytes[4];
	fyh.month 	= fyh_bytes[5];
	fyh.year 	= fyh_bytes[6];

	// Cargo fyh en RTC
	stat_i2c = rtc_load(fyh);
	if (stat_i2c != STATUS_OK) {
		while (1) {
			PRINTF("\n[I2C] Error rtc_load()");
			delay_ms(1000);
		}
	}

	// mfrc522_spi_config();
	// mfrc522_init();

	// TIMERS
	init_timers();
	timer_ledrun = give_timer(500, toggle_ledrun);
	on_timer(timer_ledrun, TIMER_PERIODIC);
	//timer_keypad = give_timer(5, keypad_update);
	//on_timer(timer_keypad, TIMER_PERIODIC);
	timer_off = give_timer(50, off_buzzer);

	uint8_t loops = 0;
	// LOOP DE EJECUCION
    while (1) {
    	// PRINTF("\nTecla: ");
    	// test_keypad();
    	// itoa(toggles, s_toggles, 10);
    	// lcd4_print(s_toggles, 2);
    	// scan_rfid();
    	loops++;
    	if (loops > 9) {
    		print_date();
    		loops = 0;
    	}
    	delay_ms(100);
    }
    return 0;
}

/* --------------------------------------------------------------
 * INTERRUPTIONS
 * -------------------------------------------------------------- */
void SysTick_Handler(void)
{
	// timers
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
	//static char tecla = 0;
	// tecla = keypad_readkey();
	// PRINTF("%d", tecla);
	keypad_row_write(0, 0);
	delay_ms(1);
	keypad_row_write(0, 1);
	keypad_row_write(1, 0);
	delay_ms(1);
	keypad_row_write(1, 1);
	keypad_row_write(2, 0);
	delay_ms(1);
	keypad_row_write(2, 1);
	keypad_row_write(3, 0);
	delay_ms(1);
	keypad_row_write(3, 1);
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
		status = eeprom_write(date_bytes, 0x0010, sizeof(date_bytes));
		if (status != STATUS_OK) {
			while (1) {
				PRINTF("\n[I2C] Error eeprom_write()");
				delay_ms(1000);
			}
		}
		PRINTF("\n[OK] Saved time on 0x0010");
		delay_ms(50);
		BUZZER_OFF();
	}

	sprintf(date_str, "%02d/%02d %02d:%02d:%02d",
			date.day, date.month,
			date.hour, date.min, date.sec);
	lcd4_print(date_str, 2);
	PRINTF("\n[OK] Date: %s", date_str);
}

void scan_rfid(void)
{
	static char s_uid[17];

	if (isCard()) {
		if (readCardSerial()) {
			sprintf(s_uid, "%2x 2x 2x 2x",
					serNum[0],
					serNum[1],
					serNum[2],
					serNum[3]);

			BUZZER_ON();
			RELAY_ON();
			on_timer(timer_off, TIMER_ONESHOT);

			lcd4_print(s_uid, 1);
			PRINTF("\n[RFID] %s", s_uid);
		}
	}
}

uint8_t off_buzzer(void)
{
	BUZZER_OFF();
	RELAY_OFF();
	return 0;
}
