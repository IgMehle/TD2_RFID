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
static timer_t *timers = 0;
static uint8_t n_timers = 0;
volatile uint8_t timers_count = 0;

/* -----------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------*/
void init_timers(timer_t *my_timers, uint8_t n)
{
	timers = my_timers;
	if (n < MAX_N_TIMERS) n_timers = n;
	else n_timers = MAX_N_TIMERS;
}

uint8_t give_timer(uint32_t time, uint8_t (*function)())
{
	uint8_t n_timer = 0xFF;
	// Si no llegue al maximo de timers, agrego
	if (timers_count < n_timers) {
		// Lleno los campos del nuevo timer
		// Inicia apagado
		timers[timers_count].enabled = 0;
		timers[timers_count].periodic = 0;
		timers[timers_count].time = time;
		timers[timers_count].count = time;
		timers[timers_count].function = function;
		// Devuelvo el numero de timer
		n_timer = timers_count;
		// Incremento la cantidad de timers creados
		timers_count++;
	}
	return n_timer;
}

void on_timer(uint8_t id, uint8_t periodic)
{
	timers[id].count = timers[id].time;
	timers[id].periodic = periodic;
	timers[id].enabled = 1;
}

void pause_timer(uint8_t id)
{
	timers[id].enabled = 0;
}

void continue_timer(uint8_t id)
{
	timers[id].enabled = 1;
}

void reset_timer(uint8_t id)
{
	timers[id].count = timers[id].time;
}

void resize_timer(uint8_t id, uint32_t time)
{
	timers[id].time = time;
	timers[id].count = time;
}

void off_timer(uint8_t id)
{
	timers[id].enabled = 0;
	timers[id].count = timers[id].time;
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

