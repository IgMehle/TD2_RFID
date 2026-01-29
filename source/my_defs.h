/*
 * defs.h
 *
 *  Created on: 21 ene. 2026
 *      Author: Ignacio
 */

#ifndef MY_DEFS_H_
#define MY_DEFS_H_

// BOARD
#include "board.h"
#include "clock_config.h"
#include "peripherals.h"
#include "pin_mux.h"
// DEVICE
#include "LPC845.h"
// DRIVERS
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_swm.h"
#include "fsl_spi.h"
#include "fsl_i2c.h"
#include "fsl_iocon.h"	// For Glitch Filter
// UTILITIES
#include "fsl_debug_console.h"
#include <stdio.h>
#include <string.h>

/******************************
 * 	PINOUT
 ****************************** */

// PORT 1
#define RELAY_PIN		0
#define LEDRUN_PIN		1
#define LEDERR_PIN		2
// PORT 0
#define RFID_RST_PIN	16
#define RFID_MISO_PIN	17
#define RFID_MOSI_PIN	18
#define RFID_SCK_PIN	19
#define RFID_SS_PIN		20

#define SCL_PIN			10
#define SDA_PIN			11

#define BUZZER_PIN		31
#define PULS_PIN		4
#define HALLSENS_PIN	21

#define UART0_TX_PIN	24
#define UART0_RX_PIN	25
#define UART1_TX_PIN	7
#define UART1_RX_PIN	12

#define LCD_RS_PIN		0
#define	LCD_E_PIN		1
#define LCD_DB4_PIN		6
#define LCD_DB5_PIN		15
#define LCD_DB6_PIN		14
#define	LCD_DB7_PIN		13
#define LCD_BL_PIN		30

#define KBD_R1_PIN		22
#define KBD_R2_PIN		23
#define KBD_R3_PIN		26
#define	KBD_R4_PIN		27
#define KBD_C1_PIN		28
#define KBD_C2_PIN		29
#define KBD_C3_PIN		9
#define KBD_C4_PIN		8

#define RET_OK			0
#define RET_ERR			1
/******************************************
 * 	MACROS
 ****************************************** */
#define SYSTICK_IRQEN	NVIC_EnableIRQ(SysTick_IRQn)
#define SYSTICK_IRQDIS	NVIC_DisableIRQ(SysTick_IRQn)

//-----------------------------------------------------------//
// PROTOTYPES
//-----------------------------------------------------------//
void delay_ms(uint32_t ms);

///// PERIPHERALS /////
void gpio_init(void);
void i2c_init(void);
void spi_init(void);
void uart_init(void);

#endif /* MY_DEFS_H_ */
