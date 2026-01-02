/*
 * UartDriver.c
 *
 *  Created on: Dec 28, 2025
 *      Author: TTN
 */

#include "UartDriver.h"

void UartTransmittOneByteData(uint8_t transmitted_data)
{
	HAL_UART_Transmit(&huart1, &transmitted_data, 1, 1);
}
