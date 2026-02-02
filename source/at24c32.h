/*
 * at24c32.h
 *
 *  Created on: 28 ene. 2026
 *      Author: Ignacio
 */

#ifndef AT24C32_H_
#define AT24C32_H_

#include <stdint.h>

#define EEPROM_ADDR 		0x50
#define EEPROM_PAGE_SIZE 	0x20
#define EEPROM_OK			0
#define EEPROM_ERR			1

// I2C
uint8_t eeprom_i2c_write(uint8_t *frame, uint8_t len);
uint8_t eeprom_i2c_read(uint8_t *data, uint8_t *addr, uint8_t len);

// EEPROM

uint8_t eeprom_wait_ready(void);

/// @brief Escribe bytes en la eeprom
/// @param data Puntero al array de bytes a escribir en la eeprom
/// @param address Direccion interna de memoria a escribir
/// @param bytes Cantidad de bytes a escribir
/// @return 0 si se escriben los bytes OK, 1 si hay algun error
uint8_t eeprom_write(uint8_t *data, uint16_t address, uint8_t bytes);

/// @brief Lee bytes de la eeprom
/// @param data Puntero al array de bytes donde escribo los bytes leidos
/// @param address Direccion interna de memoria a leer
/// @param bytes Cantidad de bytes a leer
/// @return 0 si se leen los bytes OK, 1 si hay algun error
uint8_t eeprom_read(uint8_t *data, uint16_t address, uint8_t bytes);

#endif /* AT24C32_H_ */
