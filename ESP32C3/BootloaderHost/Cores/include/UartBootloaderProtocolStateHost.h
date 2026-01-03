/*
 * UartBootloaderProtocolState.h
 *
 *  Created on: Nov 20, 2025
 *      Author: ASUS
 */

#ifndef CORES_INCLUDE_UARTBOOTLOADERPROTOCOLSTATE_H_
#define CORES_INCLUDE_UARTBOOTLOADERPROTOCOLSTATE_H_

/* Private define */
#define MAX_DATA_LEN            64
#define PROTOCOL_VER_10         10
/*----------------*/

/* Enum define */
enum CommandCode
{
    GET_CMD             = 0x00,
    GET_VERSION         = 0x01,
    GET_ID              = 0x02,
    READ_MEM            = 0x11,
    GO_CMD              = 0x21,
    WRITE_MEM           = 0x31,
    ERASE_MEM           = 0x43,
    EXT_ERASE_MEM       = 0x44,
    WRITE_PROTECT       = 0x63,
    WRITE_UNPROTECT     = 0x73,
    READOUT_PROTECT     = 0x82,
    READOUT_UNPROTECT   = 0x92,
    GET_CHECKSUM        = 0xA1,
    SPECIAL_CMD         = 0x50,
    EXT_SPECIAL_CMD     = 0x51,
	NOT_CODE			= 0xFF,
};

enum CommunicationCodes
{
    REQUEST_HANDSHAKE   = 0x77,
    RESPONE_HANDSHAKE   = 0x78,
    END_HANDSHAKE       = 0x80,
    REQUEST_DATA        = 0xFE,
    RESPONSE_DATA       = 0xFD,
    ACK                 = 0x79,
    NACK                = 0X1F
};

typedef enum
{
    FRAME_ERROR             = 0,
    FRAME_OK                = 1,
    FRAME_ABLE_TO_PROCESS   = 2,
    FRAME_UNABLE_TO_PROCESS = 3
} eFrameStatus;
/*----------------*/

#endif /* CORES_INCLUDE_UARTBOOTLOADERPROTOCOLSTATE_H_ */
