/*
 * pinout.h
 *
 *  Created on: 4 feb. 2026
 *      Author: Ignacio
 */

#ifndef PINOUT_H_
#define PINOUT_H_

/* ================= PORT 0 ================= */
/* ----------------- RFID ------------------- */
#define RFID_RST_PIN	16
#define RFID_MISO_PIN	17
#define RFID_MOSI_PIN	18
#define RFID_SCK_PIN	19
#define RFID_SS_PIN		20
/* ----------------- I2C -------------------- */
#define SCL_PIN			10
#define SDA_PIN			11
/* ----------------- IO --------------------- */
#define BUZZER_PIN		31
#define PULS_PIN		4
#define HALLSENS_PIN	21
/* ----------------- LCD -------------------- */
#define LCD_RS_PIN		0
#define	LCD_E_PIN		1
#define LCD_DB4_PIN		6
#define LCD_DB5_PIN		15
#define LCD_DB6_PIN		14
#define	LCD_DB7_PIN		13
#define LCD_BL_PIN		30
/* ----------------- KEYPAD ----------------- */
#define KBD_R0_PIN		22
#define KBD_R1_PIN		23
#define KBD_R2_PIN		26
#define	KBD_R3_PIN		27
#define KBD_C0_PIN		28
#define KBD_C1_PIN		29
#define KBD_C2_PIN		9
#define KBD_C3_PIN		8
/* ----------------- UART ------------------- */
#define UART0_TX_PIN	24
#define UART0_RX_PIN	25
#define UART1_TX_PIN	7
#define UART1_RX_PIN	12

/* ================= PORT 1 ================== */
#define RELAY_PIN		0
#define LEDRUN_PIN		1
#define LEDERR_PIN		2

#endif /* PINOUT_H_ */
