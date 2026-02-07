/*
 * timers.c
 *
 *  Created on: 1 feb. 2026
 *      Author: Ignacio
 */

#include "timers.h"

/* -----------------------------------------------------
 * GLOBAL VARIABLES
 * -----------------------------------------------------*/
volatile timer_t timers[N_TIMERS];
volatile uint8_t timers_count = 0;

/* -----------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------*/
void init_timers(void)
{
	timers_count = 0;
}

uint8_t give_timer(uint32_t time, uint8_t (*function)())
{
	uint8_t n_timer = 0xFF;
	// Si no llegue al maximo de timers, agrego
	if (timers_count < N_TIMERS) {
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
	for (uint8_t i = 0; i < timers_count; i++) {
		// Si el timer esta habilitado
		if (timers[i].enabled) {
			// Si count es cero, dispara el callback del timer
			if (timers[i].count == 0) {
				timers[i].function();
				// Si el timer es periodico, recargo el contador
				if (timers[i].periodic) timers[i].count = timers[i].time;
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

