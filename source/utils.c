/*
 * utils.c
 *
 *  Created on: 21 feb. 2026
 *      Author: ThinkPad
 */

#include "my_defs.h"

void buzzer_beep(uint32_t ms)
{
	resize_timer(timers_id.off, ms);
	BUZZER_ON();
	on_timer(timers_id.off, TIMER_ONESHOT);
}

uint8_t load_header(header_t *header)
{

}

uint8_t save_header(header_t *header)
{

}

void error_msg(uint8_t error_code, char *text)
{
	while (1) {
		PRINTF("\n[FAILED] Code %d, %s", error_code, text);
		delay_ms(1000);
	}
}
