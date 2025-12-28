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
void TransmittDataToHost(void)
{
	uint8_t transmitted_data_to_host = 0x00;

	for (uint8_t transmitted_data_index = 0; transmitted_data_index < TransmittedDataToHost[1]; transmitted_data_index++)
	{
		transmitted_data_to_host = TransmittedDataToHost[transmitted_data_index];
		DelayMs(1);
		UartTransmittOneByteData(transmitted_data_to_host);
	}
}

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

void ReceiveDataAndProcessBuffer(uint8_t received_data)
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

		// if(!IsFrameCorrect(ReceivedDataBuffer, 4))
		// {
		// 	// HandleNackForTransmission();
		// 	ResetDataBuffer();

		// 	break;
		// }

		// ParseFrame(&mUartBootloader, ReceivedDataBuffer);
		SetProcessStatus(&mUartBootloader, IN_PROCESS);

		rx_state = 0;
		rx_buffer_index = 0;
		break;

	default:
		break;
	}
}
