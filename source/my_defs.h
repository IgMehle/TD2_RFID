/*
 * defs.h
 *
 *  Created on: 21 ene. 2026
 *      Author: Ignacio
 */

#ifndef MY_DEFS_H_
#define MY_DEFS_H_

#include <stdint.h>
// LPC845
#include "LPC845.h"
#include "fsl_gpio.h"
// PINOUT
#include "pinout.h"

/******************************************
 * 	TAGS
 ****************************************** */
#define FW_VERSION			0
#define FW_REV				1

#define STATUS_OK			0
#define STATUS_ERR			1

#define RET_OK				0
#define	RET_FALSE			0xFF

#define EEPROM_HEADER_BASE	0x0000
#define EEPROM_USER_BASE	0x0040
#define EEPROM_LOGS_BASE	0x0840
#define EEPROM_END			0x0FFF

#define OFFSET_ADMIN_UID	8U
#define VALID_USER			0xA5

#define N_TIMERS			8

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
	char firma[5];			// 0
	uint8_t version[2];		// 5
	uint8_t admin_uid_len;	// 7
	uint8_t admin_uid[10];	// 8
	uint8_t admin_pin[4];	// 18
	uint8_t settings[10];	// 22
} header_t;

typedef struct user {
	uint8_t valid;				// 0
	uint8_t flags;				// 1
	uint16_t id;				// 2
	uint8_t uid[4];				// 4
	uint8_t pin[4];				// 8
	uint8_t hora_entrada[2];	// 12
	uint8_t hora_salida[2];		// 14
} user_t;

typedef struct log {
	uint16_t user_id;	// 0
	uint8_t mmdd[2];	// 2
	uint16_t minutos;	// 4
	uint8_t event;		// 6
	uint8_t flags;		// 7
} log_t;

typedef struct {
	uint8_t ledrun;
	uint8_t keypad;
	uint8_t off;
	uint8_t relay;
	uint8_t ledaux;
} timers_id_t;

/******************************************
 * 	ENUMS
 ****************************************** */
typedef enum estados {
	IDLE,
	RFID_READ,
	PIN_READ,
	VALIDAR_UID,
	VALIDAR_PIN,
	DENEGADO,
	ABRIR_PUERTA,
	PUERTA_ABIERTA,
	PUERTA_CERRADA,
	MENU_ADMIN,
	ALTA,
	BAJA,
	EDITAR,
	DUMP_LOG,
	CLEAR_LOG,
	INFO_USERS
} estados_t;

/******************************************
 * 	PROTOTYPES
 ****************************************** */
void delay_ms(uint32_t ms);

///// BOARD INIT /////
void board_gpio_init(void);
void board_i2c_init(void);
void board_spi_init(void);
void board_uart_init(void);

///// APP /////
uint8_t header_config(void);
uint8_t validar_usuario(uint8_t *uid);
uint8_t abrir_puerta(void);
uint8_t cerrar_puerta(void);
uint8_t validar_admin(uint8_t *uid);
uint8_t alta_usuario(void);
uint8_t baja_usuario(void);
uint8_t editar_usuario(void);
uint8_t info_usuarios(void);

///// UTILS /////
void buzzer_beep(uint32_t ms);
uint8_t read_header(header_t *header);
uint8_t save_header(header_t *header);
uint8_t read_user(user_t *user, uint16_t id);
uint8_t save_user(user_t *user, uint16_t id);
uint8_t read_log(log_t *log, uint16_t index);
uint8_t save_log(log_t *log, uint16_t index);
uint8_t uid_compare(uint8_t *uid1, uint8_t *uid2);
void error_msg(uint8_t error_code, char *text);
void state_switch_print(estados_t from, estados_t to);

#endif /* MY_DEFS_H_ */
