/*
 * ds3231_port.c
 *
 *  Created on: 28 ene. 2026
 *      Author: Ignacio
 */

#include "ds3231.h"
// MY DEFS
#include "my_defs.h"

uint8_t rtc_i2c_write(uint8_t *bf, uint8_t len)
{
	status_t status;

	SYSTICK_IRQDIS;	// PAUSO SYSTICK

	// I2C START
	status = I2C_MasterStart(I2C0, RTC_ADDR, kI2C_Write);
	if (status != kStatus_Success){
		SYSTICK_IRQEN;	// HABILITO SYSTICK
		PRINTF("[RTC] I2C Start Error: %d\n", status);
		return RTC_ERR;
	}

	// I2C WRITE
	status = I2C_MasterWriteBlocking(I2C0, bf, len, 0);
	I2C_MasterStop(I2C0);
	if(status != kStatus_Success){
		SYSTICK_IRQEN;	// HABILITO SYSTICK
		PRINTF("[RTC] I2C Write Error: %d\n", status);
		return RTC_ERR;
	}

	SYSTICK_IRQEN;	// HABILITO SYSTICK
	return RTC_OK;
}

uint8_t rtc_i2c_read(uint8_t *bf, uint8_t len, uint8_t ptr)
{
	status_t status;
	uint8_t read_addr[2] = {ptr, 0xFF};

	// Lectura de datos del RTC
	SYSTICK_IRQDIS;	// PAUSO SYSTICK

	// I2C START
	status = I2C_MasterStart(I2C0, RTC_ADDR, kI2C_Write);
	if(status != kStatus_Success){
		SYSTICK_IRQEN;	// HABILITO SYSTICK
		PRINTF("[RTC] I2C Start Error: %d\n", status);
		return RTC_ERR;
	}

	// Puntero a 0x00
	status = I2C_MasterWriteBlocking(I2C0, read_addr, 1, 0);
	if(status != kStatus_Success){
		SYSTICK_IRQEN;	// HABILITO SYSTICK
		PRINTF("[RTC] I2C Write Error: %d\n", status);
		return RTC_ERR;
	}

	// Repeat Start con Read Bit
	status = I2C_MasterRepeatedStart(I2C0, RTC_ADDR, kI2C_Read);
	if(status != kStatus_Success){
		SYSTICK_IRQEN;	// HABILITO SYSTICK
		PRINTF("[RTC] I2C Restart Error: %d\n", status);
		return RTC_ERR;
	}

	// Leer datos
	status = I2C_MasterReadBlocking(I2C0, bf, 7, 0);
	I2C_MasterStop(I2C0);
	if(status != kStatus_Success){
		SYSTICK_IRQEN;	// HABILITO SYSTICK
		PRINTF("[RTC] I2C Read Error: %d\n", status);
		return RTC_ERR;
	}

	SYSTICK_IRQEN;	// HABILITO SYSTICK
	return RTC_OK;
}
