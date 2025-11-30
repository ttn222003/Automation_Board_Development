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

void DelayMs(uint32_t delay_time)
{
	vTaskDelay(delay_time / portTICK_PERIOD_MS);
}

