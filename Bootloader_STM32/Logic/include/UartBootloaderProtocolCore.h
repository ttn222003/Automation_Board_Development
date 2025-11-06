#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
/*
-------------------------------
--------- Test list -----------
-------------------------------
1. Get all command
2. Calculate Check Sum
2.1. Parity check
2.2. CRC32
2.3. XOR check
3. Check command code
4. Check CRC32
*/

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
    EXT_SPECIAL_CMD     = 0x51
};

enum OtherCodes
{
    ACK                 = 0x79,
    NACK                = 0X1F
};
/*----------------*/

/*Function*/
uint8_t CheckCommandCode(uint8_t received_data[]);
void GetCommand(uint8_t* data_cmds, uint8_t protocol_version);
void GetVersion(uint8_t* data_version, uint8_t protocol_version);
void GetId(uint8_t* data_id, uint8_t id);
uint32_t ProcessAddressAndChecksum(uint8_t* received_data);
bool CheckNumberOfByteAndChecksumForReadMem(uint8_t received_data[]);
void PrepareDataToSendToHost(uint8_t* prepared_data, uint32_t start_address, uint8_t number_of_bytes);
/*----------------*/