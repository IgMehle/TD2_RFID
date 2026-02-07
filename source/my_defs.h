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

/* ----- GPIO ------------------------------ */
#define RELAY_ON		GPIO_PinWrite(GPIO, 1, RELAY_PIN, 0)
#define RELAY_OFF		GPIO_PinWrite(GPIO, 1, RELAY_PIN, 1)
#define LED_ERR_ON		GPIO_PinWrite(GPIO, 1, LED_ERR_PIN, 0)
#define LED_ERR_OFF		GPIO_PinWrite(GPIO, 1, LED_ERR_PIN, 1)
#define LED_RUN_ON		GPIO_PinWrite(GPIO, 1, LED_RUN_PIN, 0)
#define LED_RUN_OFF		GPIO_PinWrite(GPIO, 1, LED_RUN_PIN, 1)
#define LCD_BL_ON		GPIO_PinWrite(GPIO, 0, BUZZER_PIN, 0)
#define LCD_BL_OFF		GPIO_PinWrite(GPIO, 0, BUZZER_PIN, 1)
#define BUZZER_ON		GPIO_PinWrite(GPIO, 0, BUZZER_PIN, 0)
#define BUZZER_OFF		GPIO_PinWrite(GPIO, 0, BUZZER_PIN, 1)
#define PULSADOR		GPIO_PinRead(GPIO, 0, PULS_PIN)
#define HALLSENS		GPIO_PinRead(GPIO, 0, HALLSENS_PIN)

/******************************************
 * 	PROTOTYPES
 ****************************************** */
void delay_ms(uint32_t ms);

///// BOARD INIT /////
void gpio_init(void);
void i2c_init(void);
void spi_init(void);
void uart_init(void);

#endif /* MY_DEFS_H_ */
