/*
 * UartBootloaderProtocolDepeHost.h
 *
 *  Created on: Nov 20, 2025
 *      Author: ASUS
 */

#ifndef DEPENDENCIES_INCLUDE_UARTBOOTLOADERPROTOCOLDEPEHOST_H_
#define DEPENDENCIES_INCLUDE_UARTBOOTLOADERPROTOCOLDEPEHOST_H_

#include <stdint.h>
#include <string.h>
#include "UartBootloaderProtocolCoresHost.h"

/*------- Define -------*/
#define MAX_DATA_LEN	64

/*------- Initialize variable -------*/
extern uint8_t TransmittedDataToDevice[MAX_DATA_LEN];
extern uint8_t ReceivedDataFromDevice[MAX_DATA_LEN];
extern UartBootloaderProtocolHost_t mUartBootloader;

/* ------- Interface ------- */
void InitializeDataBuffer(void);
void ResetReceivedDataBuffer(void);
void DelayMs(uint32_t delay_time);


#endif /* DEPENDENCIES_INCLUDE_UARTBOOTLOADERPROTOCOLDEPEHOST_H_ */
