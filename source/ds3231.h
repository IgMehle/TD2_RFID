/*
 * rtc.h
 *
 *  Created on: 22 ene. 2026
 *      Author: Ignacio
 */

#ifndef DS3231_H_
#define DS3231_H_

#define DS3231_ADDRESS	0x68	// Address DS3231 0b1101000
#define BIN2BCD(x)		((uint8_t) (((x / 10 ) << 4) | ((x % 10))))
#define BCD2BIN(x)		((uint8_t) ((((x >> 4) & 0x0F) * 10) + (x & 0x0F)))

typedef struct {
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t weekday;
	uint8_t day;
	uint8_t month;
	uint8_t year;
} rtc_t;

void rtc_set(void);
rtc_t rtc_read(void);
void rtc_time2bytes(rtc_t time, uint8_t *bytes);

#endif /* DS3231_H_ */
