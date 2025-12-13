/*
 * UartBootloaderProtocolDepenedencies.c
 *
 *  Created on: Nov 9, 2025
 *      Author: TTN
 */

#include "UartBootloaderProtocolDepenedencies.h"

/*------- Declare variable -------*/
uint8_t ReceivedDataBuffer[MAX_DATA_LEN];
uint8_t TransmittedDataToHost[MAX_DATA_LEN];
UartBootloaderProtocolDevice_t mUartBootloader;

/*------- Implement Interface -------*/
void ResetReceivedDataBuffer(void)
{
	memset(ReceivedDataBuffer, 0, 64);
}

void ResetTransmittedDataBuffer(void)
{
	memset(TransmittedDataToHost, 0, 64);
}

void InitializeDataBuffer(void)
{
	ResetReceivedDataBuffer();
	ResetTransmittedDataBuffer();
}

void ResetDataBuffer(void)
{
	ResetReceivedDataBuffer();
	ResetTransmittedDataBuffer();
}

void HandleAckForTransmission(void)
{
	TransmittedDataToHost[0] = 1;
	TransmittedDataToHost[1] = ACK;
}

void HandleGetCommandForTransmission(uint8_t protocol_version)
{
	TransmittedDataToHost[0] = 10;
	GetCommand(protocol_version);
}

void HandleNackForTransmission(void)
{
	TransmittedDataToHost[0] = 1;
	TransmittedDataToHost[1] = NACK;
}

FrameStatus_t ReceiveDataAndProcessBuffer(uint8_t received_data)
{
	static uint8_t rx_state = 0;
	static uint8_t rx_buffer_index = 0;

	switch(rx_state)
	{
	case 0:
		if(received_data == 0x77)
		{
			ReceivedDataBuffer[rx_buffer_index++] = received_data;
			rx_state = 1;
		}
		break;

	case 1:
		if(received_data == 8)
		{
			ReceivedDataBuffer[rx_buffer_index++] = received_data;
			rx_state = 2;
		}
		break;

	case 2:
		ReceivedDataBuffer[rx_buffer_index++] = received_data;
		rx_state = 3;

		break;

	case 3:
		ReceivedDataBuffer[rx_buffer_index++] = received_data;
		rx_state = 4;

		break;

	case 4:
		ReceivedDataBuffer[rx_buffer_index++] = received_data;
		rx_state = 5;

		break;

	case 5:
		ReceivedDataBuffer[rx_buffer_index++] = received_data;
		rx_state = 6;

		break;

	case 6:
		ReceivedDataBuffer[rx_buffer_index++] = received_data;
		rx_state = 7;

		break;

	case 7:
		ReceivedDataBuffer[rx_buffer_index++] = received_data;

		if(!IsFrameCorrect(ReceivedDataBuffer, 4))
		{
			// HandleNackForTransmission();
			ResetDataBuffer();

			break;
		}

		ParseFrame(&mUartBootloader, ReceivedDataBuffer);
		SetHandlingStep(&mUartBootloader, STEP_1);
		SetProcessStatus(&mUartBootloader, IN_PROCESS);

		rx_state = 0;
		rx_buffer_index = 0;
		break;

	default:
		break;
	}
}

/*------- Implement Protocol Dependencies -------*/
void GetCommand(uint8_t protocol_version)
{
	if(protocol_version == PROTOCOL_VER_10)
	{
		TransmittedDataToHost[1] = 0x07;
		TransmittedDataToHost[2] = ((protocol_version / 10) << 4) | (protocol_version % 10);
		TransmittedDataToHost[3] = GET_CMD;
		TransmittedDataToHost[4] = GET_VERSION;
		TransmittedDataToHost[5] = GET_ID;
		TransmittedDataToHost[6] = READ_MEM;
		TransmittedDataToHost[7] = GO_CMD;
		TransmittedDataToHost[8] = WRITE_MEM;
		TransmittedDataToHost[9] = ERASE_MEM;
		TransmittedDataToHost[10] = GET_CHECKSUM;
	}
}

uint8_t CheckCommandCode(void)
{
    return ReceivedDataBuffer[1];
}

/*
@ brief:

*/
ProcessingStatus_t IsInProcessCommand(void)
{
	if ((ReceivedDataBuffer[0] == 2) && (ReceivedDataBuffer[1] != NOT_CODE) && (ReceivedDataBuffer[2] == 0xFF - ReceivedDataBuffer[1]))
	{
		return IN_PROCESS;
	}

	return NOT_IN_PROCESS;
}

