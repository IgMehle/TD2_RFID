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

// TIMERS NAMES
uint8_t timer_ledrun;
uint8_t timer_keypad;
uint8_t timer_buzzer;
//---------------------------------------------------------------//
// Prototypes
//---------------------------------------------------------------//
void delay_ms(uint32_t ms);
void test_keypad(void);

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
	// i2c_init();
	// spi_init();
	// uart_init();

	// TIMERS
	init_timers();
	timer_ledrun = give_timer(500, toggle_ledrun);
	on_timer(timer_ledrun, TIMER_PERIODIC);
	timer_keypad = give_timer(5, keypad_update);
	on_timer(timer_keypad, TIMER_PERIODIC);
	timer_buzzer = give_timer(20, off_buzzer);

	// LOOP DE EJECUCION
    while (1) {
    	PRINTF("\nTecla: ");
    	test_keypad();
    	delay_ms(200);
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
	return 0;
}

void test_keypad(void)
{
	static char tecla = 0;
	tecla = keypad_readkey();
	PRINTF("%d", tecla);
	if (tecla != KEY_NONE) {
		BUZZER_ON;
		on_timer(timer_buzzer, TIMER_ONESHOT);
	}
}

uint8_t off_buzzer(void)
{
	BUZZER_OFF;
	return 0;
}
