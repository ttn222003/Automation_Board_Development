/*
 * UartBootloaderProtocolState.h
 *
 *  Created on: Nov 1, 2025
 *      Author: ASUS
 */

#ifndef CORES_INCLUDE_UARTBOOTLOADERPROTOCOLCORES_H_
#define CORES_INCLUDE_UARTBOOTLOADERPROTOCOLCORES_H_

#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "UartBootloaderProtocolState.h"

/*------- Interface -------*/
uint8_t CheckCommandCode(uint8_t received_data[]);
void GetCommand(uint8_t* data_cmds, uint8_t protocol_version);
void GetVersion(uint8_t* data_version, uint8_t protocol_version);
void GetId(uint8_t* data_id, uint8_t id);
uint32_t ProcessAddressAndChecksum(uint8_t* received_data);
bool CheckNumberOfByteAndChecksum(uint8_t received_data[]);
void PrepareDataToSendToHost(uint8_t* prepared_data, uint32_t start_address, uint8_t number_of_bytes);
void GotoApplication(uint32_t start_address);
/*----------------*/

#endif
