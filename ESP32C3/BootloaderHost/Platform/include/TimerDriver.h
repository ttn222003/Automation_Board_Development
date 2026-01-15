/*
 * TimerDriver.h
 *
 *  Created on: Jan 2, 2026
 *      Author: TTN
 */

#ifndef PLATFORM_INCLUDE_TIMERDRIVER_H_
#define PLATFORM_INCLUDE_TIMERDRIVER_H_

#include <stdint.h>

#ifndef TEST
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "portmacro.h"
#include "esp_timer.h"
#endif

#define TIMER_MAX_INSTANCE   2

/*------- Member variable -------*/
typedef void* TimerDriverHandle_t;

TimerDriverHandle_t TimerCreate(void);
void InitializeIimer(char* name_timer, TimerDriverHandle_t timer_driver_handle, void* callback_func);
void TimerStartOnceMs(TimerDriverHandle_t timer_driver_handle, uint32_t time_delay);
void DelayMs(uint32_t time_delay);

#endif /* PLATFORM_INCLUDE_TIMERDRIVER_H_ */
