/*
 * ds3231_port.h
 *
 *  Created on: 23 feb. 2026
 *      Author: Ignacio
 */

#ifndef DS3231_PORT_H_
#define DS3231_PORT_H_

#include "ds3231.h"
// BOARD
#include "board.h"
#include "clock_config.h"
#include "peripherals.h"
#include "pin_mux.h"
// DEVICE
#include "LPC845.h"
#include "fsl_i2c.h"
#include "fsl_debug_console.h"

#define SYSTICK_IRQEN	NVIC_EnableIRQ(SysTick_IRQn)
#define SYSTICK_IRQDIS	NVIC_DisableIRQ(SysTick_IRQn)

uint8_t rtc_i2c_write(uint8_t *bf, uint8_t len);
uint8_t rtc_i2c_read(uint8_t *bf, uint8_t len, uint8_t ptr);

#endif /* DS3231_PORT_H_ */
