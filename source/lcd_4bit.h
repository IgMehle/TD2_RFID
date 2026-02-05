/*
 * lcd_4bit.h
 *
 *  Created on: 22 ene. 2026
 *      Author: Ignacio
 */

#ifndef LCD_4BIT_H_
#define LCD_4BIT_H_

#include <stdint.h>

//#define LCD_BUSY	1
//#define LCD_NBUSY	0

typedef union {
	uint8_t data;
	struct {
		uint8_t b0:1;
		uint8_t b1:1;
		uint8_t b2:1;
		uint8_t b3:1;
		uint8_t b4:1;
		uint8_t b5:1;
		uint8_t b6:1;
		uint8_t b7:1;
	} d;
} lcd_t;
/*-------------------------------------------------------------------*/
void lcd_pins_init(void);
/*-------------------------------------------------------------------*/
static inline void lcd_delay(uint32_t dl);
/*-------------------------------------------------------------------*/
void lcd_4bit_init(void);
void lcd4_write_com(unsigned char w4b);
void lcd4_write_data(unsigned char wd);
void lcd4_print(unsigned char *p,unsigned char r);
void lcd_mode4bit(void);	// mode 4bit
void ClearDisplay4(void);
void ReturnHome4(void);
void EntryModeSet4(unsigned char ems);
void DisplayOnOff4(unsigned char dof);
void CurDisShift4(unsigned char cdf);
void FunctionSet4(unsigned char fs);
void SetCGRAMAddress4(unsigned char sca);
void SetDDRAMAddress4(unsigned char sda);
//unsigned char read_data_lcd4(void);
//void nibble_lcd4(unsigned char nb);
//unsigned char check_busy4(void);
/*-------------------------------------------------------------------*/

#endif /* LCD_4BIT_H_ */
