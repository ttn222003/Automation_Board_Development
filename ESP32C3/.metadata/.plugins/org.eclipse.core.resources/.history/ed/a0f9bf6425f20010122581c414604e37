/*
 * TimerDriver.c
 *
 *  Created on: Jan 2, 2026
 *      Author: TTN
 */

#include "TimerDriver.h"

void InitializeIimer(char* name_timer, TimerDriver_t* timer_driver, void* callback_func)
{
	esp_timer_create_args_t timer_config = {
		.callback = callback_func,
		.name = name_timer,
		.arg = NULL
	};
	
	esp_timer_create(&timer_config, &timer_driver->mTimerHandle);
}

void DelayMs(uint32_t time_delay)
{
	vTaskDelay(time_delay / portTICK_PERIOD_MS);
}
