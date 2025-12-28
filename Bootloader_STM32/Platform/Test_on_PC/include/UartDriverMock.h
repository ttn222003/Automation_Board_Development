/*
 * UartDriverMock.h
 *
 *  Created on: Dec 28, 2025
 *      Author: TTN
 */

#ifndef _UART_DRIVER_H_
#define _UART_DRIVER_H_

#include <stdint.h>

void DelayMs(uint32_t time_delay);
void UartTransmittOneByteData(uint8_t transmitted_data);

#endif