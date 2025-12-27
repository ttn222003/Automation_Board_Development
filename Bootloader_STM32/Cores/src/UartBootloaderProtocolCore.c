#include "UartBootloaderProtocolCore.h"

/* Private variable */
static unsigned int polynomial_crc32 = 0x04C11DB7;
/*----------------*/

/*------- Internal function -------*/
static uint32_t CalculateCrc32(uint8_t data[], uint8_t data_length)
{
    uint32_t crc = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | (data[3] << 0);

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

static void ConvertCrcToArray(uint8_t input_data[], uint8_t data_length, uint8_t* output_data)
{
	uint32_t crc32 = CalculateCrc32(input_data, data_length);
	output_data[4] = (uint8_t)((crc32 >> 24) & 0xFF);
	output_data[5] = (uint8_t)((crc32 >> 16) & 0xFF);
	output_data[6] = (uint8_t)((crc32 >> 8) & 0xFF);
	output_data[7] = (uint8_t)((crc32 >> 0) & 0xFF);
}

/*------- Implement Interface -------*/
void InitializeUartBootloaderProtocol(UartBootloaderProtocolDevice_t* uart_bootloader)
{
	uart_bootloader->CommandCode = NOT_CODE;
	uart_bootloader->HandlingSteps = STEP_1;
	uart_bootloader->ProcessStatus = NOT_IN_PROCESS;
}

void ResetUartBootloaderProtocol(UartBootloaderProtocolDevice_t* uart_bootloader)
{
	uart_bootloader->CommandCode = NOT_CODE;
	uart_bootloader->HandlingSteps = STEP_1;
	uart_bootloader->ProcessStatus = NOT_IN_PROCESS;
}

uint8_t GetCommandCode(UartBootloaderProtocolDevice_t uart_bootloader)
{
	return uart_bootloader.CommandCode;
}

void SetCommandCode(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t cmd_code)
{
	uart_bootloader->CommandCode = cmd_code;
}

uint8_t GetHandlingStep(UartBootloaderProtocolDevice_t uart_bootloader)
{
	return uart_bootloader.HandlingSteps;
}

void SetHandlingStep(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t step)
{
	uart_bootloader->HandlingSteps = step;
}

uint8_t GetProcessStatus(UartBootloaderProtocolDevice_t uart_bootloader)
{
	return uart_bootloader.ProcessStatus;
}

void SetProcessStatus(UartBootloaderProtocolDevice_t* uart_bootloader, ProcessingStatus_t status)
{
	uart_bootloader->ProcessStatus = status;
}

/*------- API -------*/
void ParseFrameHandshakeRequestGetCommandFromHost(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t data_buffer[])
{
	uint32_t crc32_result = CalculateCrc32(data_buffer, 8);

	if(crc32_result == 0)
	{
		SetCommandCode(uart_bootloader, GET_CMD);
		return;
	}
	SetCommandCode(uart_bootloader, NOT_CODE);
}

void ParseFrameDataRequestGetCommandFromHost(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t data_buffer[])
{
	uint32_t crc32_result = CalculateCrc32(data_buffer, 8);

	if(crc32_result == 0)
	{
		SetCommandCode(uart_bootloader, GET_CMD);
		return;
	}
}

void HandleAckForTransmission(uint8_t* transmitted_data)
{
	transmitted_data[0] = RESPONE_HANDSHAKE;
	transmitted_data[1] = 4 + 4;
	transmitted_data[2] = ACK;
	transmitted_data[3] = 0x00;

	ConvertCrcToArray(transmitted_data, 4, transmitted_data);
}

void HandleNackForTransmission(uint8_t* transmitted_data)
{
	transmitted_data[0] = RESPONE_HANDSHAKE;
	transmitted_data[1] = 4 + 4;
	transmitted_data[2] = NACK;
	transmitted_data[3] = 0x00;

	ConvertCrcToArray(transmitted_data, 4, transmitted_data);
}