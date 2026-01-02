/*
 * UartBootloaderProtocolCoresHost.c
 *
 *  Created on: Nov 20, 2025
 *      Author: TTN
 */

#include "UartBootloaderProtocolCoresHost.h"
#include "UartBootloaderProtocolState.h"
#include <stdint.h>
/* Private variable */
static unsigned int polynomial_crc32 = 0x04C11DB7;

/*------- Support function -------*/
static uint32_t CalculateCrc32(uint8_t data[], uint8_t data_length)
{
    unsigned int crc = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | (data[3] << 0);

    for (uint16_t data_index = 4; data_index < data_length; data_index++)
    {
        for(int8_t i = 7; i >= 0; i--)
        {
            if(crc & 0x80000000)
            {
                crc ^= polynomial_crc32;
            }
            crc = crc << 1;

            crc |= ((data[data_index] >> i) & 0x01);
        }
    }

    for (uint8_t i = 0; i < 32; i++)
    {
        if(crc & 0x80000000)
        {
            crc ^= polynomial_crc32;
        }
        crc = crc << 1;
    }
    
    return crc;
}

/*------- Implement Interface -------*/
void SetPreviousCommandCode(UartBootloaderProtocolHost_t* uart_bootloader, uint8_t cmd_code)
{
	uart_bootloader->PreviousCommandCode = cmd_code;
}

uint8_t GetPreviousCommandCode(UartBootloaderProtocolHost_t uart_bootloader)
{
	return uart_bootloader.PreviousCommandCode;
}

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

void SetProcessStatus(UartBootloaderProtocolHost_t* uart_bootloader, uint8_t status)
{
	uart_bootloader->ProcessStatus = status;
}

ProcessingStatus_t GetProcessStatus(UartBootloaderProtocolHost_t uart_bootloader)
{
	return uart_bootloader.ProcessStatus;
}

void InitializeUartBootloaderProtocol(UartBootloaderProtocolHost_t* uart_bootloader)
{
	SetPreviousCommandCode(uart_bootloader, NOT_CODE);
	SetCommandCode(uart_bootloader, GET_CMD);
	SetHandlingStep(uart_bootloader, STEP_1);
	SetProcessStatus(uart_bootloader, NOT_IN_PROCESS);
}

void HandleBeginingProcessData(UartBootloaderProtocolHost_t uart_bootloader, uint8_t* transmitted_data)
{
	transmitted_data[0] = 0x77;
	transmitted_data[1] = 4 + 4;
	transmitted_data[2] = uart_bootloader.CommandCode;
	transmitted_data[3] = 0xFF - uart_bootloader.CommandCode;
	
	uint8_t data_to_calculate_crc[4];
	data_to_calculate_crc[0] = 0x77;
	data_to_calculate_crc[1] = 0x04 + 0x04;
	data_to_calculate_crc[2] = uart_bootloader.CommandCode;
	data_to_calculate_crc[3] = 0xFF - uart_bootloader.CommandCode;
	
	uint32_t crc32 = CalculateCrc32(data_to_calculate_crc, 4);
	
	transmitted_data[4] = (uint8_t)((crc32 << 24) & 0xFF);
	transmitted_data[5] = (uint8_t)((crc32 << 16) & 0xFF);
	
	transmitted_data[6] = (uint8_t)((crc32 << 8) & 0xFF);
	transmitted_data[7] = (uint8_t)((crc32 << 0) & 0xFF);
}

void HandleRequestData(UartBootloaderProtocolHost_t uart_bootloader, uint8_t* transmitted_data)
{
	transmitted_data[0] = 0x77;
	transmitted_data[1] = 4 + 4;
	transmitted_data[2] = uart_bootloader.CommandCode;
	transmitted_data[3] = 0xFF - uart_bootloader.CommandCode;
	
	uint8_t data_to_calculate_crc[4];
	data_to_calculate_crc[0] = 0x77;
	data_to_calculate_crc[1] = 0x04 + 0x04;
	data_to_calculate_crc[2] = uart_bootloader.CommandCode;
	data_to_calculate_crc[3] = 0xFF - uart_bootloader.CommandCode;
	
	uint32_t crc32 = CalculateCrc32(data_to_calculate_crc, 4);
	
	transmitted_data[4] = (uint8_t)((crc32 << 24) & 0xFF);
	transmitted_data[5] = (uint8_t)((crc32 << 16) & 0xFF);
	
	transmitted_data[6] = (uint8_t)((crc32 << 8) & 0xFF);
	transmitted_data[7] = (uint8_t)((crc32 << 0) & 0xFF);
}