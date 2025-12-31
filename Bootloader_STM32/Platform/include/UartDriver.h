/*
 * UartDriver.h
 *
 *  Created on: Dec 28, 2025
 *      Author: TTN
 */

#ifndef _UART_DRIVER_H_
#define _UART_DRIVER_H_

#include <stdint.h>

#ifndef TEST
#include "main.h"
extern UART_HandleTypeDef huart1;
#endif

void UartTransmittOneByteData(uint8_t transmitted_data);

#endif
