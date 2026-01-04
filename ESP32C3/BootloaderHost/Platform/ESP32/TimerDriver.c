/*
 * TimerDriver.c
 *
 *  Created on: Jan 2, 2026
 *      Author: TTN
 */

#include "TimerDriver.h"
#include "portmacro.h"

void DelayMs(uint32_t time_delay)
{
	vTaskDelay(time_delay / portTICK_PERIOD_MS);
}
