/*
 * UartBootloaderProtocolDepenedencies.h
 *
 *  Created on: Nov 9, 2025
 *      Author: ASUS
 */

#ifndef DEPENDENCIES_INCLUDE_UARTBOOTLOADERPROTOCOLDEPENEDENCIES_H_
#define DEPENDENCIES_INCLUDE_UARTBOOTLOADERPROTOCOLDEPENEDENCIES_H_

#include "ssd1306.h"
#include "fonts.h"
#include "UartBootloaderProtocolCore.h"
#include "UartBootloaderProtocolState.h"

typedef struct {
	uint8_t CommandCode;
	ProcessingStatus_t ProcessStatus;
	uint8_t HandlingSteps;
} UartBootloaderProtocolDepeDevice_t;

/*------- Interface -------*/
ProcessingStatus_t IsInProcessCommand(uint8_t received_data[]);

#endif /* DEPENDENCIES_INCLUDE_UARTBOOTLOADERPROTOCOLDEPENEDENCIES_H_ */
