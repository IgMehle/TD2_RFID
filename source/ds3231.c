/*
 * ds3231.c
 *
 *  Created on: 22 ene. 2026
 *      Author: Ignacio
 */
#include "ds3231.h"

uint8_t rtc_load(rtc_t data)
{
	uint8_t bf[8];
	uint8_t resp = 1;
	status_t stat;

	bf[0] = 0x00; // RTC WRITE ADDRESS
	bf[1] = 0x7F & BIN2BCD(data.sec);
	bf[2] = BIN2BCD(data.min);
	bf[3] = 0x3F & BIN2BCD(data.hour);
	bf[4] = data.weekday;
	bf[5] = BIN2BCD(data.day);
	bf[6] = BIN2BCD(data.month);
	bf[7] = BIN2BCD(data.year);

	// PAUSO SYSTICK
	SYSTICK_IRQDIS;
	// I2C START
	stat = I2C_MasterStart(I2C0, RTC_ADDR, kI2C_Write);

	if(stat == kStatus_Success){
		// I2C WRITE
		stat = I2C_MasterWriteBlocking(I2C0, bf, 8, 0);
		I2C_MasterStop(I2C0);
		SYSTICK_IRQEN;

		if (stat != kStatus_Success){
			PRINTF("[RTC] I2C Write Error: %d\n", r);
			resp = 0;
		}
	}
	else {
		SYSTICK_IRQEN;
		resp = 0;
	}
	return resp;
}

uint8_t rtc_read(rtc_t *data)
{
	uint8_t resp = 1;
	status_t stat;
	static uint8_t read_addr[2] = {0x00, 0xFF};
	static uint8_t x[7];

	// Lectura de datos del RTC
	SYSTICK_IRQDIS;
	// I2C START
	stat = I2C_MasterStart(I2C0, RTC_ADDR, kI2C_Write);
	if(stat == kStatus_Success){
		// Puntero a 0x00
		stat = I2C_MasterWriteBlocking(I2C0, read_addr, 1, 0);
		if(stat == kStatus_Success){
			// Repeat Start con Read Bit
			stat = I2C_MasterRepeatedStart(I2C0, RTC_ADDR, kI2C_Read);
			if(stat == kStatus_Success){
				// Leer datos
				stat = I2C_MasterReadBlocking(I2C0, x, 7, 0);
				I2C_MasterStop(I2C0);
				if(stat != kStatus_Success){
					PRINTF("[RTC] I2C Read Error: %d\n", r);
					resp = 0;
				}
			}
			else {
				PRINTF("[RTC] I2C Restart Error: %d\n", r);
				resp = 0;
			}
		}
		else {
			PRINTF("[RTC] I2C Write Error: %d\n", r);
			resp = 0;
		}
		SYSTICK_IRQEN;

		// FORMATEO BCD A BIN
		data->sec = ((x[0] & 0x70) >> 4)*10 + (x[0] & 0x0F);
		data->min = (x[1] >> 4)*10 + (x[1] & 0x0F);
		data->hs = ((x[2] & 0x20) >> 4)*10 + (x[2] & 0x0F);
		data->weekday = x[3];
		data->day = (x[4] >> 4)*10 + (x[4] & 0x0F);
		data->month = (x[5] >> 4)*10 + (x[5] & 0x0F);
		data->year = (x[6] >> 4)*10 + (x[6] & 0x0F);
	}
	else {
		SYSTICK_IRQEN;
		resp = 0;
	}
	return resp;
}

void rtc_time2bytes(rtc_t data, uint8_t *bytes)
{
	bytes[0] = data.sec;
	bytes[1] = data.min;
	bytes[2] = data.hour;
	bytes[3] = data.weekday;
	bytes[4] = data.day;
	bytes[5] = data.month;
	bytes[6] = data.year;
}
