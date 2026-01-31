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
/* TODO: insert other definitions and declarations here. */

// TIMER_T
typedef struct timer {
	uint8_t name;
	uint8_t enabled;
	uint8_t periodic;
	uint8_t event;
	uint8_t (*function)(void);
	uint32_t count;
	uint32_t time;
} timer_t;

#define N_TIMERS		8
#define TIMER_PERIODIC	1
#define TIMER_ONESHOT	0

//---------------------------------------------------------------//
// Global Variables
//---------------------------------------------------------------//
volatile uint32_t count_mseg = 0;

// TIMERS
volatile timer_t timers[N_TIMERS];
volatile uint8_t timers_count = 0;
//---------------------------------------------------------------//
// Prototypes
//---------------------------------------------------------------//
void delay_ms(uint32_t ms);

// TIMERS FUNCTIONS
void init_timers(void);
uint8_t give_timer(uint32_t time, uint8_t (*function)());
void on_timer(uint8_t name, uint8_t periodic);
void pause_timer(uint8_t name);
void continue_timer(uint8_t name);
void reset_timer(uint8_t name);
void resize_timer(uint8_t name, uint32_t time);
void off_timer(uint8_t name);
void refresh_timers(void);
timer_t get_timer_status(uint8_t name);

// TIMER CALLBACKS
uint8_t toggle_ledr(void);
uint8_t toggle_ledb(void);
uint8_t toggle_ledg(void);

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

	// init led pins
	GPIO_PortInit(GPIO, 1);
	gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 1};
	GPIO_PinInit(GPIO, 1, RELAY_PIN, &out_config);
	GPIO_PinInit(GPIO, 1, LEDRUN_PIN, &out_config);
	GPIO_PinInit(GPIO, 1, LEDERR_PIN, &out_config);

	init_timers();
	uint8_t timer1, timer2, timer3;

	timer1 = give_timer(100, toggle_ledr);
	timer2 = give_timer(500, toggle_ledb);
	timer3 = give_timer(1000, toggle_ledg);

	on_timer(timer1, TIMER_PERIODIC);
	on_timer(timer2, TIMER_PERIODIC);
	on_timer(timer3, TIMER_PERIODIC);

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

uint8_t toggle_ledr(void)
{
	if(GPIO_PinRead(GPIO, 0, LEDERR_PIN))
		GPIO_PinWrite(GPIO, 0, LEDERR_PIN, 0);
	else GPIO_PinWrite(GPIO, 0, LEDERR_PIN, 1);
	return 0;
}

uint8_t toggle_ledb(void)
{
	if(GPIO_PinRead(GPIO, 0, LEDRUN_PIN))
		GPIO_PinWrite(GPIO, 0, LEDRUN_PIN, 0);
	else GPIO_PinWrite(GPIO, 0, LEDRUN_PIN, 1);
	return 0;
}

uint8_t toggle_ledg(void)
{
	if(GPIO_PinRead(GPIO, 0, RELAY_PIN))
		GPIO_PinWrite(GPIO, 0, RELAY_PIN, 0);
	else GPIO_PinWrite(GPIO, 0, RELAY_PIN, 1);
	return 0;
}

void init_timers(void)
{
	timers_count = 0;
}

uint8_t give_timer(uint32_t time, uint8_t (*function)())
{
	uint8_t n_timer = 0xFF;
	// Si no llegue al maximo de timers, agrego
	if (timers_count < N_TIMERS){
		// Lleno los campos del nuevo timer
		// Inicia apagado
		timers[timers_count].name = timers_count;
		timers[timers_count].event = 0;
		timers[timers_count].enabled = 0;
		timers[timers_count].periodic = 0;
		timers[timers_count].time = time;
		timers[timers_count].count = time;
		timers[timers_count].function = function;
		// Devuelvo el numero de timer
		n_timer = timers[timers_count].name;
		// Incremento la cantidad de timers creados
		timers_count++;
	}
	return n_timer;
}

void on_timer(uint8_t name, uint8_t periodic)
{
	timers[name].count = timers[name].time;
	timers[name].periodic = periodic;
	timers[name].enabled = 1;
}

void pause_timer(uint8_t name)
{
	timers[name].enabled = 0;
}

void continue_timer(uint8_t name)
{
	timers[name].enabled = 1;
}

void reset_timer(uint8_t name)
{
	timers[name].count = timers[name].time;
}

void resize_timer(uint8_t name, uint32_t time)
{
	timers[name].time = time;
	timers[name].count = time;
}

void off_timer(uint8_t name)
{
	timers[name].enabled = 0;
	timers[name].count = timers[name].time;
}

void refresh_timers(void)
{
	// static uint8_t func_status = 0;
	// Itero por todos los timers creados
	for(uint8_t i = 0; i < timers_count; i++){
		// Si el timer esta habilitado
		if(timers[i].enabled){
			// Si count es cero, dispara el callback del timer
			if(timers[i].count == 0){
				timers[i].function();
				// Si el timer es periodico, recargo el contador
				if(timers[i].periodic) timers[i].count = timers[i].time;
				// Si es one shot, lo deshabilito
				else timers[i].enabled = 0;
			}
			// Si el contador no esta en cero, decremento
			else {
				timers[i].count--;
			}
		}
	}
}


