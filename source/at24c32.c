/*
 * at24c32.c
 *
 *  Created on: 28 ene. 2026
 *      Author: Ignacio
 */

#include "at24c32.h"

uint8_t eeprom_wait_ready()
{
//    uint8_t dummy = 0;
//    for (int i = 0; i < 100; i++) {   // hasta ~10 ms
//        int resp = i2c_write_blocking(i2c1, AT24C32_ADDR, &dummy, 0, false);
//        if (resp >= 0) {
//            return 1; // ACK recibido → ya está lista
//        }
//        sleep_us(100); // esperar 0.1 ms antes de reintentar
//    }
//    return 0; // timeout → error
	return 0;
}

uint8_t eeprom_write(uint8_t *data, uint16_t address, uint8_t bytes)
{
    uint8_t len = bytes + 2;
    // Frame = Address + Data
    uint8_t frame[len];
    uint8_t offset;
    uint8_t resp;

    // Chequeo que no voy a querer escribir
    // fuera de los 32 bytes de la pagina
    offset = address % EEPROM_PAGE_SIZE;
    if ((bytes + offset) <= 32){
        // Desempaqueto la direccion de memoria a la que escribir
        frame[0] = (uint8_t)((address >> 8) & 0x00FF);
        frame[1] = (uint8_t)(address & 0x00FF);
        // Cargo los datos en la trama
        for(uint8_t i = 0; i<bytes; i++) frame[i+2] = data[i];
        // Envio bytes a escribir
        resp = eeprom_i2c_write(frame, len);
    }
    else resp = EEPROM_ERR;
    return resp;
}

uint8_t eeprom_read(uint8_t *data, uint16_t address, uint8_t bytes)
{
    uint8_t addr[2];
    uint8_t resp;

    // Desempaqueto la direccion de memoria a la que escribir
    addr[0] = (uint8_t)(address >> 8);
    addr[1] = (uint8_t)(address & 0x00FF);
    // Leo bytes de la eeprom
    resp = eeprom_i2c_read(data, addr, bytes);
    return resp;
}
