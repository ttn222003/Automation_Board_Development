/*
 * TimerDriver.c
 *
 *  Created on: Jan 2, 2026
 *      Author: TTN
 */

#include "TimerDriver.h"
#include "esp_timer.h"
#include <stdint.h>

typedef struct
{
	esp_timer_handle_t mTimerHandle;
} TimerDriver_t;

static TimerDriver_t mTimerDriverArray[TIMER_MAX_INSTANCE];
static uint8_t mTimerUsed[TIMER_MAX_INSTANCE];

TimerHandleType TimerCreate(void)
{
	for (uint8_t i = 0; i < TIMER_MAX_INSTANCE; i++)
	{
		if(!mTimerUsed[i])
		{
			mTimerUsed[i]++;
			return &mTimerDriverArray[i];
		}
	}
	
	return NULL;
}

void InitializeIimer(char* name_timer, TimerHandleType timer_driver_handle, void* callback_func)
{
	TimerDriver_t* timer_driver = (TimerDriver_t*)timer_driver_handle;
	
	if(timer_driver == NULL)
	{
		return;
	}
	
	esp_timer_create_args_t timer_config = {
		.callback = callback_func,
		.name = name_timer,
		.arg = NULL
	};
	
	esp_timer_create(&timer_config, &timer_driver->mTimerHandle);
}


void TimerStartOnceMs(TimerHandleType timer_driver_handle, uint32_t time_delay)
{
	TimerDriver_t* timer_driver = (TimerDriver_t*)timer_driver_handle;
	
	if(timer_driver == NULL)
	{
		return;
	}
	
	esp_timer_start_once(timer_driver->mTimerHandle, time_delay);
}


void DelayMs(uint32_t time_delay)
{
	vTaskDelay(time_delay / portTICK_PERIOD_MS);
}
