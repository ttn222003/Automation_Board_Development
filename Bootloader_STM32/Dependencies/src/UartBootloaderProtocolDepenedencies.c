/*
 * UartBootloaderProtocolDepenedencies.c
 *
 *  Created on: Nov 9, 2025
 *      Author: TTN
 */

#include "UartBootloaderProtocolDepenedencies.h"

/*------- Declare variable -------*/
uint8_t ReceivedDataFromHost[MAX_DATA_LEN];
uint8_t TransmittedDataToHost[MAX_DATA_LEN];
UartBootloaderProtocolDevice_t mUartBootloader;

/*------- Implement Interface -------*/
void ResetReceivedDataBuffer(void)
{
	memset(ReceivedDataFromHost, 0, 64);
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
    return ReceivedDataFromHost[1];
}

/*
@ brief:

*/
ProcessingStatus_t IsInProcessCommand(void)
{
	if ((ReceivedDataFromHost[0] == 2) && (ReceivedDataFromHost[1] != NOT_CODE) && (ReceivedDataFromHost[2] == 0xFF - ReceivedDataFromHost[1]))
	{
		return IN_PROCESS;
	}

	return NOT_IN_PROCESS;
}

