/*
 * UartBootloaderProtocolState.h
 *
 *  Created on: Nov 1, 2025
 *      Author: ASUS
 */

#ifndef CORES_INCLUDE_UARTBOOTLOADERPROTOCOLCORES_H_
#define CORES_INCLUDE_UARTBOOTLOADERPROTOCOLCORES_H_

#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "UartBootloaderProtocolState.h"

/*------- Struct -------*/
typedef struct {
	uint8_t CommandCode;
	uint8_t HandlingSteps;
	ProcessingStatus_t ProcessStatus;
} UartBootloaderProtocolDevice_t;

/*------- Interface -------*/
void InitializeUartBootloaderProtocol(UartBootloaderProtocolDevice_t* uart_bootloader);
void ResetUartBootloaderProtocol(UartBootloaderProtocolDevice_t* uart_bootloader);
uint8_t GetCommandCode(UartBootloaderProtocolDevice_t uart_bootloader);
void SetCommandCode(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t cmd_code);
uint8_t GetHandlingStep(UartBootloaderProtocolDevice_t uart_bootloader);
void SetHandlingStep(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t step);
uint8_t GetProcessStatus(UartBootloaderProtocolDevice_t uart_bootloader);
void SetProcessStatus(UartBootloaderProtocolDevice_t* uart_bootloader, ProcessingStatus_t status);


void GetVersion(uint8_t* data_version, uint8_t protocol_version);
void GetId(uint8_t* data_id, uint8_t id);
uint32_t ProcessAddressAndChecksum(uint8_t* received_data);
bool CheckNumberOfByteAndChecksum(uint8_t received_data[]);
void PrepareDataToSendToHost(uint8_t* prepared_data, uint32_t start_address, uint8_t number_of_bytes);
void GotoApplication(uint32_t start_address);
/*----------------*/

#endif
