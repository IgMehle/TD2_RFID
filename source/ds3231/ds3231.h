/*
 * rtc.h
 *
 *  Created on: 22 ene. 2026
 *      Author: Ignacio
 */

#ifndef DS3231_H_
#define DS3231_H_

#include <stdint.h>

#define RTC_ADDR			0x68	// Address DS3231 0b1101000
#define RTC_OK				0
#define RTC_ERR				1
// #define BIN2BCD(x)			( (uint8_t) ( (((x / 10) << 4) & 0xF0) | (x % 10) ) )
#define BIN2BCD(x)			( (uint8_t) ( (((uint8_t)(x/10)) << 4) | (x % 10) ) )
#define BCD2BIN(x, mask)	( (uint8_t) ( ((x & mask) >> 4) * 10 + (x & 0x0F) ) )

// RTC_T FOR RTC //
typedef struct {
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t weekday;
	uint8_t day;
	uint8_t month;
	uint8_t year;
} rtc_t;

// RTC
uint8_t rtc_load(rtc_t data);
uint8_t rtc_read(rtc_t *data);
void rtc_time2bytes(rtc_t time, uint8_t *bytes);
void rtc_bytes2time(uint8_t *bytes, rtc_t *time);

#endif /* DS3231_H_ */
