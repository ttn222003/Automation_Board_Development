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

/*------- Private Functions -------*/
static void HandleRequestDataPhase(void)
{
	uint8_t command_code = GetCommandCode(mUartBootloader);
	
	switch (command_code)
	{
	case GET_CMD:
		HandleDataGetCommandForTransmission(TransmittedDataToHost);
		break;
	
	case GET_VERSION:
	{
		uint8_t major_version = GetMajorVersion(mUartBootloader);
		uint8_t minor_version = GetMinorVersion(mUartBootloader);
		HandleDataGetVersionForTransmission(TransmittedDataToHost, major_version, minor_version);
		break;
	}
		
	default:
		break;
	}
}

static void HandleValidFrame(void)
{
	uint8_t phase = GetPhase(mUartBootloader);

	if((phase == REQUEST_HANDSHAKE) || (phase == END_HANDSHAKE))
	{
		HandleAckForTransmission(TransmittedDataToHost);
	}
	else if(phase == REQUEST_DATA)
	{
		HandleRequestDataPhase();
	}
}

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
		if((received_data == REQUEST_HANDSHAKE) || (received_data == REQUEST_DATA) || (received_data == END_HANDSHAKE))
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

void HandleUartBootloaderFrame(void)
{
	if (GetFrameStatus(mUartBootloader) == FRAME_UNABLE_TO_PROCESS)
	{
		return;
	}

	eFrameStatus frame_status = ParseFrameFromHost(&mUartBootloader, ReceivedDataBuffer);

	if(frame_status == FRAME_OK)
	{
		HandleValidFrame();
	}
	else if(frame_status == FRAME_ERROR)
	{
		HandleNackForTransmission(TransmittedDataToHost);
	}

	TransmittDataToHost(TransmittedDataToHost[1]);

	ResetDataBuffer();
	SetFrameStatus(&mUartBootloader, FRAME_UNABLE_TO_PROCESS);
}