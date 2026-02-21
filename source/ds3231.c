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

	bf[0] = 0x00; // RTC WRITE ADDRESS BASE
	// FORMATEO BIN A BCD
	bf[1] = BIN2BCD(data.sec);
	bf[2] = BIN2BCD(data.min);
	// Formato 24hs
	bf[3] = 0x3F & BIN2BCD(data.hour);
	bf[4] = data.weekday;
	bf[5] = BIN2BCD(data.day);
	// Century = 0
	bf[6] = 0x1F & BIN2BCD(data.month);
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
	data->sec = BCD2BIN(x[0], 0x70);
	data->min = BCD2BIN(x[1], 0x70);
	data->hour = BCD2BIN(x[2], 0x30);
	data->weekday = x[3];
	data->day = BCD2BIN(x[4], 0x30);
	data->month = BCD2BIN(x[5], 0x10);
	data->year = BCD2BIN(x[6], 0xF0);

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

void rtc_bytes2time(uint8_t *bytes, rtc_t *time)
{
	time->sec = bytes[0];
	time->min = bytes[1];
	time->hour = bytes[2];
	time->weekday = bytes[3];
	time->day = bytes[4];
	time->month = bytes[5];
	time->year = bytes[6];
}
