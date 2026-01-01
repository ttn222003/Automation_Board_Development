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
	uint8_t Phase;
	eFrameStatus FrameStatus;
} UartBootloaderProtocolDevice_t;

/*------- Interface -------*/
void InitializeUartBootloaderProtocol(UartBootloaderProtocolDevice_t* uart_bootloader);
void ResetUartBootloaderProtocol(UartBootloaderProtocolDevice_t* uart_bootloader);
uint8_t GetCommandCode(UartBootloaderProtocolDevice_t uart_bootloader);
void SetCommandCode(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t cmd_code);
uint8_t GetPhase(UartBootloaderProtocolDevice_t uart_bootloader);
void SetPhase(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t phase);
eFrameStatus GetFrameStatus(UartBootloaderProtocolDevice_t uart_bootloader);
void SetFrameStatus(UartBootloaderProtocolDevice_t* uart_bootloader, eFrameStatus frame_status);

/*------- API -------*/
eFrameStatus ParseFrameHandshakeRequestGetCommandFromHost(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t data_buffer[]);
eFrameStatus ParseFrameDataRequestGetCommandFromHost(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t data_buffer[]);
eFrameStatus ParseFrameEndHandshakeGetCommandFromHost(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t data_buffer[]);
void HandleAckForTransmission(uint8_t* transmitted_data);
void HandleNackForTransmission(uint8_t* transmitted_data);
void HandleDataGetCommandForTransmission(uint8_t* transmitted_data);
/*----------------*/

#endif
