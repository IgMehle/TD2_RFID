/*
 * at24c32_port.h
 *
 *  Created on: 23 feb. 2026
 *      Author: Ignacio
 */

#ifndef AT24C32_PORT_H_
#define AT24C32_PORT_H_

#include "at24c32.h"
// LPC845
#include "LPC845.h"
#include "fsl_i2c.h"
#include "fsl_debug_console.h"

#define SYSTICK_IRQEN	NVIC_EnableIRQ(SysTick_IRQn)
#define SYSTICK_IRQDIS	NVIC_DisableIRQ(SysTick_IRQn)

// I2C
uint8_t eeprom_i2c_write(uint8_t *frame, uint8_t len);
uint8_t eeprom_i2c_read(uint8_t *data, uint8_t *addr, uint8_t len);

#endif /* AT24C32_PORT_H_ */
