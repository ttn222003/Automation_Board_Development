#include "stdio.h"
#include "stdint.h"
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
#define MAX_DATA_LEN        64
#define PROTOCOL_VER_10     1
/*----------------*/

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

static unsigned int polynomial_crc = 0x04C11DB7;

uint8_t CheckCommandCode(uint8_t received_data[]);
uint32_t CalculateCrc32(uint8_t data[], uint8_t data_length);
void GetCommand(uint8_t* data, uint8_t protocol_version);