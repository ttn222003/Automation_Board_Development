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
    GET_CMD     = 0x00,
    GET_VERSION = 0x01,
    GET_ID      = 0x02,
};

unsigned char CheckCommandCode(unsigned char received_data[]);