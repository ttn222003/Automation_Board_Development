/*
 * UartBootloaderProtocolCoresHost.c
 *
 *  Created on: Nov 20, 2025
 *      Author: TTN
 */

#include "UartBootloaderProtocolCoresHost.h"
#include "UartBootloaderProtocolState.h"

/*------- Implement Interface -------*/
void SetCommandCode(UartBootloaderProtocolHost_t* uart_bootloader, uint8_t cmd_code)
{
	uart_bootloader->CommandCode = cmd_code;
}

uint8_t GetCommandCode(UartBootloaderProtocolHost_t uart_bootloader)
{
	return uart_bootloader.CommandCode;
}

void SetHandlingStep(UartBootloaderProtocolHost_t* uart_bootloader, uint8_t step)
{
	uart_bootloader->HandlingSteps = step;
}

uint8_t GetHandlingStep(UartBootloaderProtocolHost_t uart_bootloader)
{
	return uart_bootloader.HandlingSteps;
}

void InitializeUartBootloaderProtocol(UartBootloaderProtocolHost_t* uart_bootloader)
{
	SetCommandCode(uart_bootloader, GET_CMD);
	SetHandlingStep(uart_bootloader, STEP_1);
}

void HandleBeginingProcessData(uint8_t* transmitted_data, UartBootloaderProtocolHost_t uart_bootloader)
{
	transmitted_data[0] = 2;
	transmitted_data[1] = uart_bootloader.CommandCode;
	transmitted_data[2] = 0xFF - uart_bootloader.CommandCode;
}

StatusResult ParseGetCommand(UartBootloaderProtocolHost_t* uart_bootloader, uint8_t received_data[])
{
	if(received_data[1] == 0x07)
	{
		uart_bootloader->get_command_parameter.SupportVersion = received_data[4];
		uart_bootloader->get_command_parameter.SupportId = received_data[5];
		uart_bootloader->get_command_parameter.SupportReadMem = received_data[6];
		uart_bootloader->get_command_parameter.SupportGo = received_data[7];
		uart_bootloader->get_command_parameter.SupportWriteMem = received_data[8];
		uart_bootloader->get_command_parameter.SupportEraseMem = received_data[9];
		uart_bootloader->get_command_parameter.SupportGetChecksum = received_data[10];
		
		return STATUS_OK;
	}
	
	return STATUS_FAIL;
}