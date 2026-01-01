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
void TransmittDataToHost(uint8_t data_length)
{
	uint8_t transmitted_data_to_host = 0x00;

	for (uint8_t transmitted_data_index = 0; transmitted_data_index < data_length; transmitted_data_index++)
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

eFrameStatus ReceiveDataAndPutInBuffer(uint8_t received_data)
{
	static uint8_t rx_state = 0;
	static uint8_t rx_buffer_index = 0;

	switch(rx_state)
	{
	case 0:
		if((received_data == REQUEST_HANDSHAKE) || (received_data == REQUEST_DATA))
		{
			ReceivedDataBuffer[rx_buffer_index++] = received_data;
			rx_state = 1;
		}

		return FRAME_UNABLE_TO_PROCESS;

	case 1:
		ReceivedDataBuffer[rx_buffer_index++] = received_data;
		rx_state = 2;

		return FRAME_UNABLE_TO_PROCESS;

	case 7:
		ReceivedDataBuffer[rx_buffer_index++] = received_data;

		rx_state = 0;
		rx_buffer_index = 0;

		return FRAME_ABLE_TO_PROCESS;

	default:
		ReceivedDataBuffer[rx_buffer_index++] = received_data;
		rx_state++;

		return FRAME_UNABLE_TO_PROCESS;
	}
}
