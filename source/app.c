/*
 * app.c
 *
 *  Created on: 21 feb. 2026
 *      Author: ThinkPad
 */

#include "my_defs.h"

uint8_t header_config(void)
{
	header_t header;
	uint8_t status = STATUS_OK;
	uint8_t uid[5];

	status = read_header(&header);
	if (status != STATUS_OK) error_msg(status, "Error load_header()");

	///// CONFIGURACION INICIAL /////
	if (strcmp(header.firma, "RFID") != 0) {
		// Cargo firma
		sprintf(header.firma, "RFID");
		// Version del firmware
		header.version[0] = FW_VERSION;
		header.version[1] = FW_REV;
		// Escaneo continuamente hasta leer una UID valida
		while (1) {
			if (isCard()) {
				if (readCardSerial(uid)) break;
			}
		}
		// Asigno el uid como admin
		strcpy(header.admin_uid, uid);
		header.admin_uid_len = 4;
		buzzer_beep(100);
		// TODO: escanear admin_pin
		// TODO: asignar settings
	}
	////////////////////////////////
	// Guardar header en memoria
	status = save_header(&header);
	if (status != STATUS_OK) error_msg(status, "Error save_header()");
	return status;
}
