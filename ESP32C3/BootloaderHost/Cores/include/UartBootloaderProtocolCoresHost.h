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
typedef struct {
	uint8_t SupportVersion;
	uint8_t SupportId;
	uint8_t SupportReadMem;
	uint8_t SupportGo;
	uint8_t SupportWriteMem;
	uint8_t SupportEraseMem;
	uint8_t SupportGetChecksum;
} GetCommandParameter_t;

typedef struct{
	uint8_t PreviousCommandCode;
	uint8_t CommandCode;
	uint8_t HandlingSteps;
	GetCommandParameter_t get_command_parameter;
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
void HandleBeginingProcessData(uint8_t* transmitted_data, UartBootloaderProtocolHost_t uart_bootloader);
StatusResult ParseGetCommand(UartBootloaderProtocolHost_t* uart_bootloader, uint8_t received_data[]);
#endif /* CORES_INCLUDE_UARTBOOTLOADERPROTOCOLCORESHOST_H_ */
