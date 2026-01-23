/*
 * lcd_4bit.c
 *
 *  Created on: 22 ene. 2026
 *      Author: Ignacio
 */

#include "lcd_4bit.h"
#include "lcd_4bit_port.h"

static inline void lcd_delay(uint32_t dl)
{
	uint32_t d;
	for(d=0;d<dl;d++);
}
/*-------------------------------------------------------------------*/
/* Ciclo de inicializacion modificado,
 * según datasheet HD44780U,
 * Fig. 24 "4 bit Interface" (pag. 46),
 * se coloca ClearDisplay4() antes de
 * EntryModeSet4() y se asegura el comando
 * 0x28 en FunctionSet4()
 * A: I. Mehle, L. Paolucci */
void lcd_4bit_init(void)
{
	//LCD_RW(0);
	LCD_RS(1);
	LCD_E(0);
	lcd_mode4bit();
	FunctionSet4(0x28);
	DisplayOnOff4(0x0C); // agregar esta linea
	ClearDisplay4();
	EntryModeSet4(0x06);
	ReturnHome4();
}
/*-------------------------------------------------------------------*/
void lcd4_write_com(unsigned char wb){
	LCD_RS(0);
	//LCD_RW(0);
	lcd.data = wb;
	LCD_DB7(lcd.d.b7);
	LCD_DB6(lcd.d.b6);
	LCD_DB5(lcd.d.b5);
	LCD_DB4(lcd.d.b4);
	LCD_E(1);
	lcd_delay(DELAY_SHORT);
	LCD_E(0);
	LCD_DB7(lcd.d.b3);
	LCD_DB6(lcd.d.b2);
	LCD_DB5(lcd.d.b1);
	LCD_DB4(lcd.d.b0);
	LCD_E(1);
	lcd_delay(DELAY_SHORT);
	LCD_E(0);
	lcd_delay(DELAY_SHORT);
}
/*-------------------------------------------------------------------*/
void lcd4_write_data(unsigned char wd)
{
	LCD_RS(1);
	//LCD_RW(0);
	lcd.data = wd;
	LCD_DB7(lcd.d.b7);
	LCD_DB6(lcd.d.b6);
	LCD_DB5(lcd.d.b5);
	LCD_DB4(lcd.d.b4);
	LCD_E(1);
	lcd_delay(DELAY_SHORT);
	LCD_E(0);
	LCD_DB7(lcd.d.b3);
	LCD_DB6(lcd.d.b2);
	LCD_DB5(lcd.d.b1);
	LCD_DB4(lcd.d.b0);
	LCD_E(1);
	lcd_delay(DELAY_SHORT);
	LCD_E(0);
	lcd_delay(DELAY_SHORT);
}
/*-------------------------------------------------------------------*/
void lcd4_line(unsigned char *p,unsigned char r)
{
	switch(r)
	{
	case 1:
		r = 0;
		break;
	case 2:
		r = 0x40;
		break;
	case 3:
		r = 0x14;
		break;
	case 4:
		r = 0x54;
		break;
	default:
		r = 0;
		break;
	}
	//if(r==1) r = 0;
	//if(r==2) r = 0x40;
	//if(r==3) r = 0x14;
	//if(r==4) r = 0x54;

	SetDDRAMAddress4(r);
	r = 0;
	while(r<20){
		lcd4_write_data(*p);
		p++;
		r++;
		if(*p == 0) r = 20;
	}
}
/*-------------------------------------------------------------------*/
/* Inicializacion de modo de 4 bits,
 * modificado según datasheet HD44780U
 * Fig. 24 "4 bit Interface" (pag. 46)
 * A: I. Mehle, L. Paolucci */
void lcd_mode4bit(void)
{
	// Delay at least 15ms
	// to ensure display init
	lcd_delay(DELAY_LONG);
	lcd_delay(DELAY_LONG);
	lcd_delay(DELAY_LONG);
	lcd_delay(DELAY_LONG);
	lcd_delay(DELAY_LONG);
	lcd_delay(DELAY_LONG);
	LCD_RS(0);
	// 0x30
	LCD_DB7(0);
	LCD_DB6(0);
	LCD_DB5(1);
	LCD_DB4(1);
	LCD_E(1);
	lcd_delay(DELAY_SHORT);
	LCD_E(0);
	// Wait for more than 4,1 ms
	lcd_delay(DELAY_LONG);
	lcd_delay(DELAY_LONG);
	lcd_delay(DELAY_LONG);
	// 0x30
	LCD_DB7(0);
	LCD_DB6(0);
	LCD_DB5(1);
	LCD_DB4(1);
	LCD_E(1);
	lcd_delay(DELAY_SHORT);
	LCD_E(0);
	// Wait for more than 100 us
	lcd_delay(DELAY_SHORT);
	// 0x30
	LCD_DB7(0);
	LCD_DB6(0);
	LCD_DB5(1);
	LCD_DB4(1);
	LCD_E(1);
	lcd_delay(DELAY_SHORT);
	LCD_E(0);
	// 4 bit mode (0x20)
	LCD_DB7(0);
	LCD_DB6(0);
	LCD_DB5(1);
	LCD_DB4(0);
	LCD_E(1);
	lcd_delay(DELAY_SHORT);
	LCD_E(0);
	lcd_delay(DELAY_SHORT);
}
/*-------------------------------------------------------------------*/
void ClearDisplay4(void){
	lcd4_write_com(0x01);		// Display clear
	lcd_delay(DELAY_LONG);		// delay 1.53mseg
}
/*-------------------------------------------------------------------*/
void ReturnHome4(void){
	lcd4_write_com(0x02);		// Display clear
	lcd_delay(DELAY_LONG);			// delay 1.53mseg
}
/*-------------------------------------------------------------------*/
void EntryModeSet4(unsigned char ems){
	ems &= 0x03; ems |= 0x04;
	lcd4_write_com(ems);		// Entry mode set: I/D=1-increment mode,SH=0-shift off
	lcd_delay(DELAY_SHORT);			// delay 39useg
}
/*-------------------------------------------------------------------*/
void DisplayOnOff4(unsigned char dof){
	//dof &= 0x07; dof |= 0x08;
	lcd4_write_com(dof);		// Display ON/OFF:D=1 display on,C=0 cursor off,B=0,blink off
	lcd_delay(DELAY_SHORT);			// delay 39useg
}
/*-------------------------------------------------------------------*/
void CurDisShift4(unsigned char cdf){
	cdf &= 0x0C; cdf |= 0x10;
	lcd4_write_com(cdf);		// cursor display shift
	lcd_delay(DELAY_SHORT);			// delay 39useg
}
/*-------------------------------------------------------------------*/
void FunctionSet4(unsigned char fs){
	fs &= 0x2C;	// DL = 0; forces 4 bit mode
	//fs |= 0x20;
	lcd4_write_com(fs);		// Function set: 4bit bus,N=1-1line,F=0-5x8dots
	lcd_delay(DELAY_SHORT);		// delay 39useg
}
/*-------------------------------------------------------------------*/
void SetCGRAMAddress4(unsigned char sca){
	sca &= 0x3F; sca |= 0x40;
	lcd4_write_com(sca);		// Funtion set: 8bit bus,N=1-1line,F=0-5x8dots
	lcd_delay(DELAY_SHORT);			// delay 39useg
}
/*-------------------------------------------------------------------*/
void SetDDRAMAddress4(unsigned char sda){
	sda &= 0x7F; sda |= 0x80;
	lcd4_write_com(sda);		// Funtion set: 8bit bus,N=1-1line,F=0-5x8dots
	lcd_delay(DELAY_SHORT);			// delay 39useg
}
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*
unsigned char char_read_lcd4(void){
	unsigned char l;
	l = read_data_lcd4();
	return l;
}*/
/*-------------------------------------------------------------------*/
