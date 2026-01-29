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
	uint8_t resp;

	bf[0] = 0x00; // RTC WRITE ADDRESS
	bf[1] = 0x7F & BIN2BCD(data.sec);
	bf[2] = BIN2BCD(data.min);
	bf[3] = 0x3F & BIN2BCD(data.hour);
	bf[4] = data.weekday;
	bf[5] = BIN2BCD(data.day);
	bf[6] = BIN2BCD(data.month);
	bf[7] = BIN2BCD(data.year);

	resp = rtc_i2c_write(bf, sizeof(bf));
	return resp;
}

uint8_t rtc_read(rtc_t *data)
{
	static uint8_t x[7];
	uint8_t resp;

	resp = rtc_i2c_read(x, sizeof(x), 0x00);

	// FORMATEO BCD A BIN
	data->sec = ((x[0] & 0x70) >> 4)*10 + (x[0] & 0x0F);
	data->min = (x[1] >> 4)*10 + (x[1] & 0x0F);
	data->hour = ((x[2] & 0x20) >> 4)*10 + (x[2] & 0x0F);
	data->weekday = x[3];
	data->day = (x[4] >> 4)*10 + (x[4] & 0x0F);
	data->month = (x[5] >> 4)*10 + (x[5] & 0x0F);
	data->year = (x[6] >> 4)*10 + (x[6] & 0x0F);

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
