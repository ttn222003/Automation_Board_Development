#include "UartBootloaderProtocolCore.h"

/* Private variable */
static unsigned int polynomial_crc32 = 0x04C11DB7;
/*----------------*/

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

bool IsFrameCorrect(uint8_t data_buffer[], uint8_t data_length)
{
	uint32_t crc32_result = CalculateCrc32(data_buffer, 4);

	if(crc32_result == 0)
	{
		return true;
	}

	return false;
}

void ParseFrame(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t data_buffer[])
{
	uart_bootloader->CommandCode = data_buffer[2];
}

void HandleAckForTransmission(uint8_t* transmitted_data)
{
	transmitted_data[0] = 0x77;
	transmitted_data[1] = 4 + 4;
	transmitted_data[2] = ACK;
	transmitted_data[3] = ACK;

	uint8_t data_to_calculate_crc[4];
	data_to_calculate_crc[0] = 0x77;
	data_to_calculate_crc[1] = 0x04 + 0x04;
	data_to_calculate_crc[2] = ACK;
	data_to_calculate_crc[3] = ACK;

	uint32_t crc32 = CalculateCrc32(data_to_calculate_crc, 4);

	transmitted_data[4] = (uint8_t)((crc32 << 24) & 0xFF);
	transmitted_data[5] = (uint8_t)((crc32 << 16) & 0xFF);

	transmitted_data[6] = (uint8_t)((crc32 << 8) & 0xFF);
	transmitted_data[7] = (uint8_t)((crc32 << 0) & 0xFF);
}
