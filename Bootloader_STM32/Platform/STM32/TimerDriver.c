/*
 * TimerDriver.c
 *
 *  Created on: Jan 1, 2026
 *      Author: TTN
 */

#include "TimerDriver.h"

void DelayMs(uint32_t time_delay)
{
	HAL_Delay(time_delay);
}