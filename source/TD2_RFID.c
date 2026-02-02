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
#include "lcd_4bit.h"
#include "lcd_4bit_port.h"
#include "ds3231.h"
#include "at24c32.h"
#include "timers.h"
/* TODO: insert other definitions and declarations here. */

//---------------------------------------------------------------//
// Global Variables
//---------------------------------------------------------------//
volatile uint32_t count_mseg = 0;
volatile uint8_t led_run = 0;

// TIMERS NAMES
uint8_t timer1, timer2, timer3;
//---------------------------------------------------------------//
// Prototypes
//---------------------------------------------------------------//
void delay_ms(uint32_t ms);

// TIMER CALLBACKS
//uint8_t toggle_ledr(void);
//uint8_t toggle_ledb(void);
//uint8_t toggle_ledg(void);
uint8_t toggle_ledrun(void);
uint8_t ledr_off(void);
uint8_t ledg_off(void);

/*
 * @brief   Application entry point.
 */
int main(void)
{
	BOARD_BootClockFRO24M();
	BOARD_InitDebugConsole();
	(void) SysTick_Config(SystemCoreClock/1000);

	// PORT INIT
	//gpio_init();
	//i2c_init();
	//spi_init();

	// INIT LED PINS
	GPIO_PortInit(GPIO, 1);
	gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 1};
	GPIO_PinInit(GPIO, 1, RELAY_PIN, &out_config);
	GPIO_PinInit(GPIO, 1, LEDRUN_PIN, &out_config);
	GPIO_PinInit(GPIO, 1, LEDERR_PIN, &out_config);

	init_timers();
	// timer1 = give_timer(100, toggle_ledr);
	timer1 = give_timer(500, toggle_ledrun);
	// timer2 = give_timer(500, toggle_ledb);
	// timer3 = give_timer(1000, toggle_ledg);
	timer2 = give_timer(200, ledr_off);
	timer3 = give_timer(100, ledg_off);

	on_timer(timer1, TIMER_PERIODIC);
	// on_timer(timer2, TIMER_PERIODIC);
	// on_timer(timer3, TIMER_PERIODIC);

	// LOOP DE EJECUCION
    while(1) {
    }
    return 0;
}

/* --------------------------------------------------------------
 * Interruptions
 * -------------------------------------------------------------- */
void SysTick_Handler(void)
{
	// timers
    refresh_timers();
    // delay_ms()
    if (count_mseg > 0) count_mseg--;
}

//---------------------------------------------------------------//
// Functions
//---------------------------------------------------------------//
void delay_ms(uint32_t ms)
{
	count_mseg = ms;
    while(count_mseg > 0);
}

//uint8_t toggle_ledr(void)
//{
//	if(GPIO_PinRead(GPIO, 1, LEDERR_PIN))
//		GPIO_PinWrite(GPIO, 1, LEDERR_PIN, 0);
//	else GPIO_PinWrite(GPIO, 1, LEDERR_PIN, 1);
//	return 0;
//}

uint8_t toggle_ledrun(void)
{
	// Toggle led
	led_run ^= 1;
	GPIO_PinWrite(GPIO, 1, LEDRUN_PIN, led_run);
	// Si el led azul se apaga
	if(led_run){
		// Enciendo el led rojo
		GPIO_PinWrite(GPIO, 1, LEDERR_PIN, 0);
		// retardo off 200ms
		on_timer(timer2, TIMER_ONESHOT);
	}
	return 0;
}

uint8_t ledr_off(void)
{
	// Apago el led rojo
	GPIO_PinWrite(GPIO, 1, LEDERR_PIN, 1);
	// enciendo el led verde
	GPIO_PinWrite(GPIO, 1, RELAY_PIN, 0);
	// retardo off 100ms
	on_timer(timer3, TIMER_ONESHOT);
	return 0;
}

uint8_t ledg_off(void)
{
	// Apago led verde
	GPIO_PinWrite(GPIO, 1, RELAY_PIN, 1);
	return 0;
}

//uint8_t toggle_ledg(void)
//{
//	if(GPIO_PinRead(GPIO, 1, RELAY_PIN))
//		GPIO_PinWrite(GPIO, 1, RELAY_PIN, 0);
//	else GPIO_PinWrite(GPIO, 1, RELAY_PIN, 1);
//	return 0;
//}
