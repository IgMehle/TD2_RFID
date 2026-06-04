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

// Comandos
#define LCD_CLEARDISPLAY 	0x01
#define LCD_RETURNHOME 		0x02
#define LCD_ENTRYMODESET 	0x04
#define LCD_DISPLAYCONTROL 	0x08
#define LCD_CURSORSHIFT 	0x10
#define LCD_FUNCTIONSET 	0x20
#define LCD_SETCGRAMADDR 	0x40
#define LCD_SETDDRAMADDR 	0x80

// Flags para tipo de entrada
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYLEFT 			0x02

// Flags para control de display y cursor
#define LCD_BLINKON 		0x01
#define LCD_CURSORON 		0x02
#define LCD_DISPLAYON 		0x04

// Flags para control de movimiento de cursor
#define LCD_MOVERIGHT 		0x04
#define LCD_DISPLAYMOVE 	0x08

// Flags para seteo de funcion
#define LCD_5x10DOTS 		0x04
#define LCD_2LINE 			0x08
#define LCD_8BITMODE 		0x10

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
static inline void lcd_delay(uint32_t dl)
{
	uint32_t d;
	for(d = 0; d < dl; d++);
}
/*-------------------------------------------------------------------*/
void lcd_4bit_init(void);
void lcd_mode4bit(void);	// mode 4bit
/*-------------------------------------------------------------------*/
void lcd4_write_com(uint8_t w4b);
void lcd4_write_char(char wd);
/*-------------------------------------------------------------------*/
void lcd4_set_cursor(uint8_t line, uint8_t position);
void lcd4_string(const char *p_str);
void lcd4_print(char *p, unsigned char r);
/*-------------------------------------------------------------------*/
void ClearDisplay4(void);
void ReturnHome4(void);
void EntryModeSet4(uint8_t ems);
void DisplayOnOff4(uint8_t dof);
void CurDisShift4(uint8_t cdf);
void FunctionSet4(uint8_t fs);
void SetCGRAMAddress4(uint8_t sca);
void SetDDRAMAddress4(uint8_t sda);
//unsigned char read_data_lcd4(void);
//void nibble_lcd4(unsigned char nb);
//unsigned char check_busy4(void);
/*-------------------------------------------------------------------*/

#endif /* LCD_4BIT_H_ */
