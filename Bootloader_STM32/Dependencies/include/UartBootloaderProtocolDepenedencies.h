/*
 * UartBootloaderProtocolDepenedencies.h
 *
 *  Created on: Nov 9, 2025
 *      Author: TTN
 */

#ifndef _DEPENDENCIES_INCLUDE_UARTBOOTLOADERPROTOCOLDEPENEDENCIES_H_
#define _DEPENDENCIES_INCLUDE_UARTBOOTLOADERPROTOCOLDEPENEDENCIES_H_

// #include "ssd1306.h"
// #include "fonts.h"
#include "UartBootloaderProtocolCore.h"

#ifndef TEST
#include "UartDriver.h"
#else
#include "UartDriverMock.h"
#endif

/*------- Define -------*/
#define MAX_DATA_LEN	64

/*------- Initialize variable -------*/
extern uint8_t ReceivedDataBuffer[MAX_DATA_LEN];
extern uint8_t TransmittedDataToHost[MAX_DATA_LEN];
extern UartBootloaderProtocolDevice_t mUartBootloader;

/*------- Interface -------*/
void TransmittDataToHost(void);
void ResetReceivedDataBuffer(void);
void ResetTransmittedDataBuffer(void);
void InitializeDataBuffer(void);
void ResetDataBuffer(void);
void ReceiveDataAndProcessBuffer(uint8_t received_data);

#endif /* DEPENDENCIES_INCLUDE_UARTBOOTLOADERPROTOCOLDEPENEDENCIES_H_ */
