/*
 * UartBootloaderProtocolCoresHost.h
 *
 *  Created on: Nov 20, 2025
 *      Author: TTN
 */

#ifndef CORES_INCLUDE_UARTBOOTLOADERPROTOCOLCORESHOST_H_
#define CORES_INCLUDE_UARTBOOTLOADERPROTOCOLCORESHOST_H_

#include "UartBootloaderProtocolState.h"
#include <stdint.h>

/*------- Struct -------*/
typedef struct{
	uint8_t PreviousCommandCode;
	uint8_t CommandCode;
	uint8_t HandlingSteps;
} UartBootloaderProtocolHost_t;

/*------- Setter Getter -------*/
void SetPreviousCommandCode(UartBootloaderProtocolHost_t* uart_bootloader, uint8_t cmd_code);
uint8_t GetPreviousCommandCode(UartBootloaderProtocolHost_t uart_bootloader);
void SetCommandCode(UartBootloaderProtocolHost_t* uart_bootloader, uint8_t cmd_code);
uint8_t GetCommandCode(UartBootloaderProtocolHost_t uart_bootloader);
void SetHandlingStep(UartBootloaderProtocolHost_t* uart_bootloader, uint8_t step);
uint8_t GetHandlingStep(UartBootloaderProtocolHost_t uart_bootloader);

/*------- API -------*/
void InitializeUartBootloaderProtocol(UartBootloaderProtocolHost_t* uart_bootloader);
void HandleBeginingProcessData(UartBootloaderProtocolHost_t uart_bootloader, uint8_t* transmitted_data);
#endif /* CORES_INCLUDE_UARTBOOTLOADERPROTOCOLCORESHOST_H_ */
