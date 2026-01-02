/*
 * UartBootloaderProtocolDepeHost.c
 *
 *  Created on: Nov 20, 2025
 *      Author: TTN
 */

#include "UartBootloaderProtocolDepeHost.h"

/*------- Declare variable -------*/
uint8_t TransmittedDataToDevice[MAX_DATA_LEN];
uint8_t ReceivedDataFromDevice[MAX_DATA_LEN];
UartBootloaderProtocolHost_t mUartBootloader;

/*------- Implement Interface -------*/
void InitializeDataBuffer(void)
{
	memset(TransmittedDataToDevice, 0, 64);
	memset(ReceivedDataFromDevice, 0, 64);
}

void ResetReceivedDataBuffer(void)
{
	memset(ReceivedDataFromDevice, 0, 64);
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
			ReceivedDataFromDevice[rx_buffer_index++] = received_data;
			rx_state = 1;
		}
		break;

	case 1:
		if(received_data == 8)
		{
			ReceivedDataFromDevice[rx_buffer_index++] = received_data;
			rx_state = 2;
		}
		break;

	case 2:
		ReceivedDataFromDevice[rx_buffer_index++] = received_data;
		rx_state = 3;

		break;

	case 3:
		ReceivedDataFromDevice[rx_buffer_index++] = received_data;
		rx_state = 4;

		break;

	case 4:
		ReceivedDataFromDevice[rx_buffer_index++] = received_data;
		rx_state = 5;

		break;

	case 5:
		ReceivedDataFromDevice[rx_buffer_index++] = received_data;
		rx_state = 6;

		break;

	case 6:
		ReceivedDataFromDevice[rx_buffer_index++] = received_data;
		rx_state = 7;

		break;

	case 7:
		ReceivedDataFromDevice[rx_buffer_index++] = received_data;
		
		rx_state = 0;
		rx_buffer_index = 0;
		break;

	default:
		break;
	}
}