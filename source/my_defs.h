/*
 * defs.h
 *
 *  Created on: 21 ene. 2026
 *      Author: Ignacio
 */

#ifndef MY_DEFS_H_
#define MY_DEFS_H_

// BOARD
#include "board.h"
#include "clock_config.h"
#include "peripherals.h"
#include "pin_mux.h"
// DEVICE
#include "LPC845.h"
// DRIVERS
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_swm.h"
#include "fsl_spi.h"
#include "fsl_i2c.h"
#include "fsl_iocon.h"	// For Glitch Filter
// UTILITIES
#include "fsl_debug_console.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// PINOUT
#include "pinout.h"

#define STATUS_OK		0
#define STATUS_ERR		1
/******************************************
 * 	MACROS
 ****************************************** */
#define SYSTICK_IRQEN	NVIC_EnableIRQ(SysTick_IRQn)
#define SYSTICK_IRQDIS	NVIC_DisableIRQ(SysTick_IRQn)
#define NOP() __asm volatile ("nop")

//-----------------------------------------------------------//
// PROTOTYPES
//-----------------------------------------------------------//
void delay_ms(uint32_t ms);

///// PERIPHERALS /////
void gpio_init(void);
void i2c_init(void);
void spi_init(void);
void uart_init(void);

#endif /* MY_DEFS_H_ */
