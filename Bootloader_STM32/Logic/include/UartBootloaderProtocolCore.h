#include "stdio.h"

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
*/

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

unsigned char CheckCommandCode(unsigned char received_data[]);