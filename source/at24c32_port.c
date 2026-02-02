/*
 * at24c32_port.c
 *
 *  Created on: 28 ene. 2026
 *      Author: Ignacio
 */

#include "at24c32.h"
// MY DEFS
#include "my_defs.h"

uint8_t eeprom_i2c_write(uint8_t *frame, uint8_t len)
{
	status_t status;

	SYSTICK_IRQDIS;	// PAUSO SYSTICK

	// I2C START
	status = I2C_MasterStart(I2C0, EEPROM_ADDR, kI2C_Write);
	if(status != kStatus_Success){
		SYSTICK_IRQEN;	// HABILITO SYSTICK
		return EEPROM_ERR;
	}

	// I2C WRITE
	status = I2C_MasterWriteBlocking(I2C0, frame, len, 0);
	I2C_MasterStop(I2C0);
	if(status != kStatus_Success){
		SYSTICK_IRQEN;	// HABILITO SYSTICK
		return EEPROM_ERR;
	}
	SYSTICK_IRQEN;	// HABILITO SYSTICK
	// Guarda de tiempo de escritura en eeprom PROBAR
	delay_ms(10);

	return EEPROM_OK;
}

uint8_t eeprom_i2c_read(uint8_t *data, uint8_t *addr, uint8_t len)
{
	status_t status;

	SYSTICK_IRQDIS;	// PAUSO SYSTICK

	// I2C START
	status = I2C_MasterStart(I2C0, EEPROM_ADDR, kI2C_Write);
	if(status != kStatus_Success){
		SYSTICK_IRQEN;	// HABILITO SYSTICK
		return EEPROM_ERR;
	}
	// I2C EEPROM POINTER
	status = I2C_MasterWriteBlocking(I2C0, addr, 2, 0);
	if(status != kStatus_Success){
		SYSTICK_IRQEN;	// HABILITO SYSTICK
		return EEPROM_ERR;
	}
	// I2C RESTART
	status = I2C_MasterRepeatedStart(I2C0, EEPROM_ADDR, kI2C_Read);
	if(status != kStatus_Success){
		SYSTICK_IRQEN;	// HABILITO SYSTICK
		return EEPROM_ERR;
	}
	// I2C READ
	status = I2C_MasterReadBlocking(I2C0, data, len, 0);
	I2C_MasterStop(I2C0);
	if(status != kStatus_Success){
		return EEPROM_ERR;
	}

	SYSTICK_IRQEN;	// HABILITO SYSTICK
	return EEPROM_OK;
}
