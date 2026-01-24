#include "UartBootloaderProtocolCore.h"

/* Private variable */
static unsigned int polynomial_crc32 = 0x04C11DB7;
/*----------------*/

/*------- Implement Private Functions -------*/
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
	output_data[data_length + 0] = (uint8_t)((crc32 >> 24) & 0xFF);
	output_data[data_length + 1] = (uint8_t)((crc32 >> 16) & 0xFF);
	output_data[data_length + 2] = (uint8_t)((crc32 >> 8) & 0xFF);
	output_data[data_length + 3] = (uint8_t)((crc32 >> 0) & 0xFF);
}

/*------- Implement API -------*/
void InitializeUartBootloaderProtocol(UartBootloaderProtocolDevice_t* uart_bootloader)
{
	uart_bootloader->CommandCode = NOT_CODE;
	uart_bootloader->FrameStatus = FRAME_UNABLE_TO_PROCESS;
}

void ResetUartBootloaderProtocol(UartBootloaderProtocolDevice_t* uart_bootloader)
{
	uart_bootloader->CommandCode = NOT_CODE;
	uart_bootloader->FrameStatus = FRAME_UNABLE_TO_PROCESS;
}

uint8_t GetCommandCode(UartBootloaderProtocolDevice_t uart_bootloader)
{
	return uart_bootloader.CommandCode;
}

void SetCommandCode(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t cmd_code)
{
	uart_bootloader->CommandCode = cmd_code;
}

uint8_t GetPhase(UartBootloaderProtocolDevice_t uart_bootloader)
{
	return uart_bootloader.Phase;
}

void SetPhase(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t phase)
{
	uart_bootloader->Phase = phase;
}

uint8_t GetMajorVersion(UartBootloaderProtocolDevice_t uart_bootloader)
{
	return uart_bootloader.MajorVersion;
}

void SetMajorVersion(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t major_version)
{
	uart_bootloader->MajorVersion = major_version;
}

uint8_t GetMinorVersion(UartBootloaderProtocolDevice_t uart_bootloader)
{
	return uart_bootloader.MinorVersion;
}

void SetMinorVersion(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t minor_version)
{
	uart_bootloader->MinorVersion = minor_version;
}

eFrameStatus GetFrameStatus(UartBootloaderProtocolDevice_t uart_bootloader)
{
	return uart_bootloader.FrameStatus;
}

void SetFrameStatus(UartBootloaderProtocolDevice_t* uart_bootloader, eFrameStatus frame_status)
{
	uart_bootloader->FrameStatus = frame_status;
}

eFrameStatus ParseFrameFromHost(UartBootloaderProtocolDevice_t* uart_bootloader, uint8_t data_buffer[])
{
	uint32_t crc32_result = CalculateCrc32(data_buffer, 8);

	if(crc32_result == 0)
	{
		SetCommandCode(uart_bootloader, data_buffer[2]);
		SetPhase(uart_bootloader, data_buffer[0]);
		return FRAME_OK;
	}

	SetCommandCode(uart_bootloader, NOT_CODE);
	// Should have IDLE phase here
	return FRAME_ERROR;
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

void HandleDataGetCommandForTransmission(uint8_t* transmitted_data)
{
	transmitted_data[0] = RESPONSE_DATA;
	transmitted_data[1] = 21;
	transmitted_data[2] = GET_CMD;
	transmitted_data[3] = GET_VERSION;
	transmitted_data[4] = GET_ID;
	transmitted_data[5] = READ_MEM;
	transmitted_data[6] = GO_CMD;
	transmitted_data[7] = WRITE_MEM;
	transmitted_data[8] = ERASE_MEM;
	transmitted_data[9] = EXT_ERASE_MEM;
	transmitted_data[10] = WRITE_PROTECT;
	transmitted_data[11] = WRITE_UNPROTECT;
	transmitted_data[12] = READOUT_PROTECT;
	transmitted_data[13] = READOUT_UNPROTECT;
	transmitted_data[14] = GET_CHECKSUM;
	transmitted_data[15] = SPECIAL_CMD;
	transmitted_data[16] = EXT_SPECIAL_CMD;

	ConvertCrcToArray(transmitted_data, 17, transmitted_data);
}

void HandleDataGetVersionForTransmission(uint8_t* transmitted_data, uint8_t version_major, uint8_t version_minor)
{
	transmitted_data[0] = RESPONSE_DATA;
	transmitted_data[1] = 8;
	transmitted_data[2] = version_major;
	transmitted_data[3] = version_minor;

	ConvertCrcToArray(transmitted_data, 4, transmitted_data);
}