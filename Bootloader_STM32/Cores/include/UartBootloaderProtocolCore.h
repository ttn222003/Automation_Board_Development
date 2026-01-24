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
	uint8_t MajorVersion;
	uint8_t MinorVersion;
	eFrameStatus FrameStatus;
} UartBootloaderProtocolDevice_t;

/*------- Setter - Getter -------*/
void InitializeUartBootloaderProtocol(UartBootloaderProtocolDevice_t* uart_bootloader);
void ResetUartBootloaderProtocol(UartBootloaderProtocolDevice_t* uart_bootloader);
uint8_t GetCommandCode(UartBootloaderProtocolDevice_t uart_bootloader);
void SetCommandCode(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t cmd_code);
uint8_t GetPhase(UartBootloaderProtocolDevice_t uart_bootloader);
void SetPhase(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t phase);
uint8_t GetMajorVersion(UartBootloaderProtocolDevice_t uart_bootloader);
void SetMajorVersion(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t major_version);
uint8_t GetMinorVersion(UartBootloaderProtocolDevice_t uart_bootloader);
void SetMinorVersion(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t minor_version);
eFrameStatus GetFrameStatus(UartBootloaderProtocolDevice_t uart_bootloader);
void SetFrameStatus(UartBootloaderProtocolDevice_t* uart_bootloader, eFrameStatus frame_status);

/*------- API -------*/
eFrameStatus ParseFrameFromHost(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t data_buffer[]);
void HandleAckForTransmission(uint8_t* transmitted_data);
void HandleNackForTransmission(uint8_t* transmitted_data);
void HandleDataGetCommandForTransmission(uint8_t* transmitted_data);
void HandleDataGetVersionForTransmission(uint8_t* transmitted_data, uint8_t version_major, uint8_t version_minor);
/*----------------*/

#endif
