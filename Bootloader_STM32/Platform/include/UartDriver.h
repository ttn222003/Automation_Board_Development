/*
 * UartDriver.h
 *
 *  Created on: Dec 28, 2025
 *      Author: TTN
 */

#ifndef _UART_DRIVER_H_
#define _UART_DRIVER_H_

#ifndef TEST
#include "main.h"
extern UART_HandleTypeDef huart1;
#endif

void DelayMs(uint32_t time_delay);
void UartTransmittOneByteData(uint8_t transmitted_data);

#endif
