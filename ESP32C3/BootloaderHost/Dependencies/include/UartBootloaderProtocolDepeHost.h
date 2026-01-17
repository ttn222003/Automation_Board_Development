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
#include "TimerDriver.h"
#include "UartDriver.h"

/*------- Define -------*/
#define MAX_DATA_LEN	64

/*------- Initialize variable -------*/
extern uint8_t TransmittedDataToDevice[MAX_DATA_LEN];
extern uint8_t ReceivedDataBuffer[MAX_DATA_LEN];
extern UartBootloaderProtocolHost_t mUartBootloader;

/* ------- Interface ------- */
void ResetReceivedDataBuffer(void);
void ResetTransmittedDataBuffer(void);
void InitializeDataBuffer(void);
void ResetDataBuffer(void);
void TransmittDataToDevice(UartHandleType uart_handle, uint8_t data_length);
eFrameStatus ReceiveDataAndPutInBuffer(uint8_t received_data);

#endif /* DEPENDENCIES_INCLUDE_UARTBOOTLOADERPROTOCOLDEPEHOST_H_ */
