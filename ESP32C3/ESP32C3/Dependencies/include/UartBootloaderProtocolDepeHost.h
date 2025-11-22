/*
 * UartBootloaderProtocolDepeHost.h
 *
 *  Created on: Nov 20, 2025
 *      Author: ASUS
 */

#ifndef DEPENDENCIES_INCLUDE_UARTBOOTLOADERPROTOCOLDEPEHOST_H_
#define DEPENDENCIES_INCLUDE_UARTBOOTLOADERPROTOCOLDEPEHOST_H_

#include <stdint.h>
#include "UartBootloaderProtocolCoresHost.h"

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
	uint8_t BootloaderCommandType;
	uint8_t CurrentStep;
	GetCommandParameter_t get_command_parameter;
} UartBootloaderProtocolDepeHost_t;

/* ------- Interface ------- */
StatusResult ParseGetCommand(UartBootloaderProtocolDepeHost_t* uart_bootloader_prtcl_depe_host, uint8_t received_data[]);

#endif /* DEPENDENCIES_INCLUDE_UARTBOOTLOADERPROTOCOLDEPEHOST_H_ */
