/*
 * Copyright 2016-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    LPC845_tests.c
 * @brief   Application entry point.
 */
#include "board.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_spi.h"
#include "fsl_swm.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include <stdio.h>
#include <string.h>
/* TODO: insert other include files here. */
//#include "rfid.h"
#include "mfrc522td2.h"
/* TODO: insert other definitions and declarations here. */
//void init(void);
//bool leer_tarjeta(uint8_t * uid,char *hex_uid);
void delay_ms(uint32_t ms);
volatile uint32_t count_mseg = 0;

// TD2 MASSOTTO
unsigned char serNum[5];
/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    //BOARD_InitBootPins();
    //BOARD_InitBootClocks();
    //BOARD_InitBootPeripherals();

	BOARD_InitDebugConsole();
	BOARD_BootClockFRO24M();
	(void) SysTick_Config(SystemCoreClock/1000);

	//init();
	LED_GREEN_INIT(1)	// LED
	spi_init();
	spi_config();
	rfid_init();

	//uint8_t n = 0;
	while(1){
		//MFRC522_AutoTest();
		//delay_ms(3000);
		if(isCard()){
			LED_GREEN_ON();
			if(readCardSerial()){
				PRINTF("\nUID: ");
				for(uint8_t i=0; i<4;i++){
					PRINTF("%x ",serNum[i]);
				}
				delay_ms(2000);
				LED_GREEN_OFF();
			}
		}
	}
	SPI_Deinit(SPI0);
	while (1);
	return 0;
}

//---------------------------------------------------------------//
// Interruptions
//---------------------------------------------------------------//

void SysTick_Handler(void)
{
	if (count_mseg > 0) count_mseg--;
}
//---------------------------------------------------------------//
// Functions
//---------------------------------------------------------------//

void delay_ms(uint32_t ms)
{
	count_mseg = ms;
	while(count_mseg > 0);
}

/*void init(void)
{
	//PINES SPI
	CLOCK_EnableClock(kCLOCK_Swm);
	SWM_SetMovablePinSelect(SWM0, kSWM_SPI0_SCK, kSWM_PortPin_P0_19);
	SWM_SetMovablePinSelect(SWM0, kSWM_SPI0_MOSI, kSWM_PortPin_P0_18);
	SWM_SetMovablePinSelect(SWM0, kSWM_SPI0_MISO, kSWM_PortPin_P0_17);
	CLOCK_DisableClock(kCLOCK_Swm);
	CLOCK_Select(kSPI0_Clk_From_MainClk);

	// GPIO
	GPIO_PortInit(GPIO, 0);
	gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 1};
	GPIO_PinInit(GPIO, 0, CS_PIN, &out_config);
	GPIO_PinInit(GPIO, 0, RST_PIN, &out_config);
}*/

//bool leer_tarjeta(uint8_t * uid,char *hex_uid)
//{
//    uint8_t i;
//    uid_t aux;
//    if (MFRC522_IsNewCardPresent()) {
//    	PRINTF("Tarj. Detectada\n");
//    	if (!MFRC522_ReadCardSerial( & aux))
//    		return 0;
//    	for (i = 0; i < aux.size; i++) {
//    		uid[i] = aux.uidByte[i];
//    		// Convierte el valor actual a hexadecimal y guárdalo en hex_uid
//    		sprintf(hex_uid + i * 2, "%02x", aux.uidByte[i]);
//    	}
//    MFRC522_HaltA();
//    MFRC522_StopCrypto();
//    PRINTF("\n\n"); //cambio de linea
//    delay_ms(1000);
//    return 0;
//    }
//    return 1;
//}
