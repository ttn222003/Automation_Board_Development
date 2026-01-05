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

/*------- Member variable -------*/
extern esp_timer_handle_t mDebounceTimer;

void IimerInit(char* name_timer, void* callback_func, esp_timer_handle_t* timer_handle);
void DelayMs(uint32_t time_delay);

#endif /* PLATFORM_INCLUDE_TIMERDRIVER_H_ */
