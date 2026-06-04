/*
 * lcd_4bit_pins.h
 *
 *  Created on: 22 ene. 2026
 *      Author: Ignacio
 */

#ifndef LCD_4BIT_PORT_H_
#define LCD_4BIT_PORT_H_

#include "lcd_4bit.h"
/*-------------------------------------------------------------------*/
// LPC845
#include "fsl_gpio.h"
/*-------------------------------------------------------------------*/
// MY DEFS
#include "pinout.h"
#define lcd4_clear()	ClearDisplay4()
/*-------------------------------------------------------------------*/
// PINOUT GPIO
#define LCD_DB4(x)			GPIO_PinWrite(GPIO, 0, LCD_DB4_PIN, x)
#define LCD_DB5(x)			GPIO_PinWrite(GPIO, 0, LCD_DB5_PIN, x)
#define LCD_DB6(x)			GPIO_PinWrite(GPIO, 0, LCD_DB6_PIN, x)
#define LCD_DB7(x)			GPIO_PinWrite(GPIO, 0, LCD_DB7_PIN, x)
#define LCD_RS(x)			GPIO_PinWrite(GPIO, 0, LCD_RS_PIN, x)
//#define LCD_RW(x)			GPIO_PinWrite(GPIO, 0, RW, x)
#define LCD_E(x)			GPIO_PinWrite(GPIO, 0, LCD_E_PIN, x)
#define LCD_BL(x)			GPIO_PinWrite(GPIO, 0, LCD_BL_PIN, x)
/*-------------------------------------------------------------------*/
// DELAY LCD
/* Los delays bloqueantes deben ser ajustados segun
 * frecuencia del uC, y cantidad de ciclos por
 * ejecución de loop for (8 ciclos/loop aprox)
 * A: I. Mehle, L. Paolucci	 */

#define DELAY_SHORT	600		// 200us @ 24MHz
#define DELAY_LONG	6000	// 2ms @ 24MHz

// Dimensiones del LCD
#define LCD_MAX_LINES	2
#define LCD_MAX_CHARS	16

/*-------------------------------------------------------------------*/
#endif /* LCD_4BIT_PORT_H_ */
