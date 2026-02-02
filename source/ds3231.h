/*
 * rtc.h
 *
 *  Created on: 22 ene. 2026
 *      Author: Ignacio
 */

#ifndef DS3231_H_
#define DS3231_H_

#include <stdint.h>

#define RTC_ADDR		0x68	// Address DS3231 0b1101000
#define RTC_OK			0
#define RTC_ERR			1
#define BIN2BCD(x)		((unsigned char) (((x / 10 ) << 4) | ((x % 10))))
#define BCD2BIN(x)		((unsigned char) ((((x >> 4) & 0x0F) * 10) + (x & 0x0F)))

typedef struct {
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t weekday;
	uint8_t day;
	uint8_t month;
	uint8_t year;
} rtc_t;

// I2C
uint8_t rtc_i2c_write(uint8_t *bf, uint8_t len);
uint8_t rtc_i2c_read(uint8_t *bf, uint8_t len, uint8_t ptr);

uint8_t rtc_load(rtc_t data);
uint8_t rtc_read(rtc_t *data);
void rtc_time2bytes(rtc_t time, uint8_t *bytes);

#endif /* DS3231_H_ */
