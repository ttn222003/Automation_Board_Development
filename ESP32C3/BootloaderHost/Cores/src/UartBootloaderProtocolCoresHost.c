/*
 * UartBootloaderProtocolCoresHost.c
 *
 *  Created on: Nov 20, 2025
 *      Author: TTN
 */

#include "UartBootloaderProtocolCoresHost.h"
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

void InitializeUartBootloaderProtocol(UartBootloaderProtocolHost_t* uart_bootloader)
{
	SetPreviousCommandCode(uart_bootloader, NOT_CODE);
	SetCommandCode(uart_bootloader, GET_CMD);
}

void HandleHandshakeRequestOfGetCommandForTransmission(uint8_t* transmitted_data)
{
	transmitted_data[0] = REQUEST_HANDSHAKE;
	transmitted_data[1] = 4 + 4;
	transmitted_data[2] = GET_CMD;
	transmitted_data[3] = 0xFF - GET_CMD;

	uint32_t crc32_result = CalculateCrc32(transmitted_data, 4);

	transmitted_data[4] = (uint8_t)((crc32_result >> 24) & 0xFF);
	transmitted_data[5] = (uint8_t)((crc32_result >> 16) & 0xFF);
	transmitted_data[6] = (uint8_t)((crc32_result >> 8) & 0xFF);
	transmitted_data[7] = (uint8_t)((crc32_result >> 0) & 0xFF);
}