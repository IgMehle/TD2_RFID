/*
 * config.c
 *
 *  Created on: 22 ene. 2026
 *      Author: Ignacio
 */

#include "my_defs.h"

void gpio_init(void)
{
	GPIO_PortInit(GPIO, 0);
	GPIO_PortInit(GPIO, 1);
	gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 1};
	gpio_pin_config_t in_config = {kGPIO_DigitalInput, 1};

	// LCD PINS
	GPIO_PinInit(GPIO, 0, LCD_RS_PIN, &out_config);
	GPIO_PinInit(GPIO, 0, LCD_E_PIN, &out_config);
	GPIO_PinInit(GPIO, 0, LCD_DB4_PIN, &out_config);
	GPIO_PinInit(GPIO, 0, LCD_DB5_PIN, &out_config);
	GPIO_PinInit(GPIO, 0, LCD_DB6_PIN, &out_config);
	GPIO_PinInit(GPIO, 0, LCD_DB7_PIN, &out_config);
	GPIO_PinInit(GPIO, 0, LCD_BL_PIN, &out_config);

	// RFID PINS
	GPIO_PinInit(GPIO, 0, RFID_SS_PIN, &out_config);
	GPIO_PinInit(GPIO, 0, RFID_RST_PIN, &out_config);

	// TECLADO
	GPIO_PinInit(GPIO, 0, KBD_R1_PIN, &out_config);
	GPIO_PinInit(GPIO, 0, KBD_R2_PIN, &out_config);
	GPIO_PinInit(GPIO, 0, KBD_R3_PIN, &out_config);
	GPIO_PinInit(GPIO, 0, KBD_R4_PIN, &out_config);
	GPIO_PinInit(GPIO, 0, KBD_C1_PIN, &in_config);
	GPIO_PinInit(GPIO, 0, KBD_C2_PIN, &in_config);
	GPIO_PinInit(GPIO, 0, KBD_C3_PIN, &in_config);
	GPIO_PinInit(GPIO, 0, KBD_C4_PIN, &in_config);

	// IO
	GPIO_PinInit(GPIO, 0, BUZZER_PIN, &out_config);
	GPIO_PinInit(GPIO, 0, PULS_PIN, &in_config);
	GPIO_PinInit(GPIO, 0, HALLSENS_PIN, &in_config);

	// RELAY - LEDS
	GPIO_PinInit(GPIO, 1, RELAY_PIN, &out_config);
	GPIO_PinInit(GPIO, 1, LEDRUN_PIN, &out_config);
	GPIO_PinInit(GPIO, 1, LEDERR_PIN, &out_config);
}

void i2c_init(void)
{
	const uint32_t baudRate = 100000;
	const uint32_t frequency = SystemCoreClock;
	///////// I2C0 //////////////////////////////////
	CLOCK_Select(kI2C0_Clk_From_MainClk);
	CLOCK_EnableClock(kCLOCK_Swm);
	SWM_SetFixedPinSelect(SWM0, kSWM_I2C0_SCL, kSWM_PortPin_P0_10);
	SWM_SetFixedPinSelect(SWM0, kSWM_I2C0_SDA, kSWM_PortPin_P0_11);
	CLOCK_DisableClock(kCLOCK_Swm);
	i2c_master_config_t masterConfig;
	/*
	 * masterConfig.debugEnable = false;
	 * masterConfig.ignoreAck = false;
	 * masterConfig.pinConfig = kI2C_2PinOpenDrain;
	 * masterConfig.baudRate_Bps = 100000U;
	 * masterConfig.busIdleTimeout_ns = 0;
	 * masterConfig.pinLowTimeout_ns = 0;
	 * masterConfig.sdaGlitchFilterWidth_ns = 0;
	 * masterConfig.sclGlitchFilterWidth_ns = 0;
	 */
	I2C_MasterGetDefaultConfig(&masterConfig);
	//masterConfig.baudRate_Bps = baudRate;
	I2C_MasterInit(I2C0, &masterConfig, SystemCoreClock);

///////// I2C1 //////////////////////////////////
//	CLOCK_Select(kI2C1_Clk_From_MainClk);
//	CLOCK_EnableClock(kCLOCK_Swm);
//	SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SCL, kSWM_PortPin_P0_1);
//	SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SDA, kSWM_PortPin_P0_0);
//	CLOCK_DisableClock(kCLOCK_Swm);
//
//	i2c_master_config_t masterConfig;
//	I2C_MasterGetDefaultConfig(&masterConfig);
//	masterConfig.baudRate_Bps = baudRate;
//	I2C_MasterInit(I2C1, &masterConfig, frequency);
}

void spi_init(void)
{
	// Config SPI PINS
	CLOCK_EnableClock(kCLOCK_Swm);
	SWM_SetMovablePinSelect(SWM0, kSWM_SPI0_MISO, RFID_MISO_PIN);
	SWM_SetMovablePinSelect(SWM0, kSWM_SPI0_MOSI, RFID_MOSI_PIN);
	SWM_SetMovablePinSelect(SWM0, kSWM_SPI0_SCK, RFID_SCK_PIN);
	// SWM_SetMovablePinSelect(SWM0, kSWM_SPI0_SSEL0, RFID_SS_PIN);
	CLOCK_DisableClock(kCLOCK_Swm);
	CLOCK_Select(kSPI0_Clk_From_MainClk);
}

void uart_init(void)
{

}
