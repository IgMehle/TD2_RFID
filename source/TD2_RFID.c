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
void print_time(void);
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
	LCD_BL_OFF();
	BUZZER_OFF();

	lcd_4bit_init();
	// lcd4_print("Hola!!", 1);
	char s_toggles[17];

	uint8_t stat_i2c = 0;
	rtc_t fyh = {0,0,12,1,1,1,26};
	stat_i2c = rtc_load(fyh);
	if (stat_i2c != STATUS_OK) {
		while (1) {
			PRINTF("\n[I2C] Error rtc_load()");
			delay_ms(1000);
		}
	}

	uint8_t demo[] = "Hola!";
	stat_i2c = eeprom_write(demo, 0, sizeof(demo));
	if (stat_i2c != STATUS_OK) {
		while (1) {
			PRINTF("\n[I2C] Error eeprom_write()");
			delay_ms(1000);
		}
	}

	uint8_t lectura[17];
	stat_i2c = eeprom_read(lectura, 0, sizeof(demo));
	if (stat_i2c != STATUS_OK) {
		while (1) {
			PRINTF("\n[I2C] Error eeprom_read()");
			delay_ms(1000);
		}
	}

	lcd4_print(lectura, 1);

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
    	scan_rfid();
    	loops++;
    	if (loops > 9) {
    		print_time();
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

void print_time(void)
{
	rtc_t time;
	uint8_t status = 0;
	char s_time[17];

	status = rtc_read(&time);
	if (status != STATUS_OK) {
		while (1) {
			PRINTF("\n[I2C] Error rtc_read()");
			delay_ms(1000);
		}
	}

	sprintf(s_time, "%2d/2d/2d 2d:2d:2d",
			time.day, time.month, time.year,
			time.hour, time.min, time.sec);
	lcd4_print(s_time, 2);
	PRINTF("\n%s", s_time);
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
			PRINTF("\n%s", s_uid);
		}
	}
}

uint8_t off_buzzer(void)
{
	BUZZER_OFF();
	RELAY_OFF();
	return 0;
}
