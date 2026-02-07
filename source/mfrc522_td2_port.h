/*
 * mfrc522_td2_port.h
 *
 *  Created on: 6 feb. 2026
 *      Author: Ignacio
 */

#ifndef MFRC522_TD2_PORT_H_
#define MFRC522_TD2_PORT_H_

#include "mfrc522_td2.h"
//---------------------------------------------------------------//
// MY DEFS
#include "my_defs.h"
// LPC845
#include "fsl_clock.h"
#include "fsl_swm.h"
#include "fsl_gpio.h"
#include "fsl_spi.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_common.h"

//---------------------------------------------------------------//
// CONTROL PINS
//---------------------------------------------------------------//
#define CS_PIN			RFID_SS_PIN
#define RST_PIN         RFID_RST_PIN
//#define IRQ_PIN         0xFF
#define MFRC522_RST(x)  GPIO_PinWrite(GPIO, 0, RST_PIN, x)
#define MFRC522_CS(x)   GPIO_PinWrite(GPIO, 0, CS_PIN, x)

//--------------------------------------------------------------//
// VARIABLES Y FUNCIONES EXTERNAS
extern void delay_ms(uint32_t ms); // Delay function prototype
extern unsigned char serNum[5];

// PROTOTYPES
void mfrc522_spi_config(void);
uint8_t readMFRC522(uint8_t addr);
void writeMFRC522(uint8_t addr, uint8_t val);
//--------------------------------------------------------------//

#endif /* MFRC522_TD2_PORT_H_ */
