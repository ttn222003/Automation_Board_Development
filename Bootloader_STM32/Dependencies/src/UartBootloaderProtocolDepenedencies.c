/*
 * UartBootloaderProtocolDepenedencies.c
 *
 *  Created on: Nov 9, 2025
 *      Author: ASUS
 */

#include "UartBootloaderProtocolDepenedencies.h"

ProcessingStatus_t IsInProcessCommand(uint8_t received_data[])
{
	if ((received_data[0] == 2) && (received_data[1] != NOT_CODE) & (received_data[2] == 0xFF - received_data[2]))
	{
		return IN_PROCESS;
	}

	return NOT_IN_PROCESS;
}

