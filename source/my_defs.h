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
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// PINOUT
#include "pinout.h"
// MY TIMERS
#include "my_timers.h"

#define FW_VERSION	0
#define FW_REV		1

#define STATUS_OK		0
#define STATUS_ERR		1

#define EEPROM_HEADER_BASE	0x0000
#define EEPROM_USER_BASE	0x0040
#define EEPROM_LOGS_BASE	0x0840
#define EEPROM_END			0x0FFF

/******************************************
 * 	MACROS
 ****************************************** */
#define SYSTICK_IRQEN	NVIC_EnableIRQ(SysTick_IRQn)
#define SYSTICK_IRQDIS	NVIC_DisableIRQ(SysTick_IRQn)
#define NOP() __asm volatile ("nop")

/* ----- GPIO ------------------------------ */
#define RELAY_ON()			GPIO_PinWrite(GPIO, 1, RELAY_PIN, 0)
#define RELAY_OFF()			GPIO_PinWrite(GPIO, 1, RELAY_PIN, 1)
#define LED_AUX_ON()		GPIO_PinWrite(GPIO, 1, LEDAUX_PIN, 0)
#define LED_AUX_OFF()		GPIO_PinWrite(GPIO, 1, LEDAUX_PIN, 1)
#define LED_RUN_ON()		GPIO_PinWrite(GPIO, 1, LEDRUN_PIN, 0)
#define LED_RUN_OFF()		GPIO_PinWrite(GPIO, 1, LEDRUN_PIN, 1)
#define LCD_BL_ON()			GPIO_PinWrite(GPIO, 0, LCD_BL_PIN, 0)
#define LCD_BL_OFF()		GPIO_PinWrite(GPIO, 0, LCD_BL_PIN, 1)
#define BUZZER_ON()			GPIO_PinWrite(GPIO, 0, BUZZER_PIN, 0)
#define BUZZER_OFF()		GPIO_PinWrite(GPIO, 0, BUZZER_PIN, 1)
#define PULSADOR()			GPIO_PinRead(GPIO, 0, PULS_PIN)
#define HALLSENS()			GPIO_PinRead(GPIO, 0, HALLSENS_PIN)

/******************************************
 * 	TYPEDEFS
 ****************************************** */
typedef struct header {
	char firma[5];
	uint8_t version[2];
	uint8_t admin_uid_len;
	uint8_t admin_uid[10];
	uint8_t admin_pin[4];
	uint8_t settings[10];
} header_t;

typedef struct user {
	uint8_t valid;
	uint16_t id;
	uint8_t flags;
	uint8_t uid[4];
	uint8_t pin[4];
	uint8_t hora_entrada[2];
	uint8_t hora_salida[2];
} user_t;

typedef struct log {
	uint16_t user_id;
	uint8_t mmdd[2];
	uint16_t minutos;
	uint8_t event;
	uint8_t flags;
} log_t;

/******************************************
 * 	ENUMS
 ****************************************** */
typedef enum estados {
	IDLE,
	RFID_READ,
	PIN_READ,
	VALIDAR,
	DENEGADO,
	ABRIR_PUERTA,
	PUERTA_ABIERTA,
	PUERTA_CERRADA,
	MENU_ADMIN,
	ALTA,
	BAJA,
	DUMP_LOG,
	CLEAR_LOG
} estados_t;

/******************************************
 * 	PROTOTYPES
 ****************************************** */
void delay_ms(uint32_t ms);

///// BOARD INIT /////
void gpio_init(void);
void i2c_init(void);
void spi_init(void);
void uart_init(void);

///// APP /////
uint8_t header_config(void);

///// UTILS /////
void buzzer_beep(uint32_t ms, uint8_t timer);
uint8_t read_header(header_t *header);
uint8_t save_header(header_t *header);
uint8_t read_user(user_t *user);
uint8_t save_user(user_t *user);
uint8_t read_log(log_t *log);
uint8_t save_log(log_t *log);
void error_msg(uint8_t error_code, char *text);

#endif /* MY_DEFS_H_ */
