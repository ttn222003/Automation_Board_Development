#include "UartBootloaderProtocolCore.h"

unsigned char CheckCommandCode(unsigned char received_data[])
{
    return received_data[0];
}


unsigned int CalculateCrc32(unsigned char data[], uint8_t data_length)
{
    unsigned int crc = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | (data[3] << 0);

    for (uint16_t data_index = 4; data_index < data_length; data_index++)
    {
        for(int8_t i = 7; i >= 0; i--)
        {
            if(crc & 0x80000000)
            {
                crc ^= polynomial_crc;
            }
            crc = crc << 1;

            crc |= ((data[data_index] >> i) & 0x01);
        }
    }

    for (uint8_t i = 0; i < 32; i++)
    {
        if(crc & 0x80000000)
        {
            crc ^= polynomial_crc;
        }
        crc = crc << 1;
    }
    
    return crc;
}

void GetCommand(uint8_t* data, uint8_t protocol_version)
{
    data[0] = 0x79;
    if(protocol_version == PROTOCOL_VER_10)
    {
        data[1] = 0x06;
        data[2] = 0x10;
        data[3] = GET_CMD;
        data[4] = GET_VERSION;
        data[5] = GET_ID;
        data[6] = READ_MEM;
        data[7] = GO_CMD;
        data[8] = WRITE_MEM;
        data[9] = GET_CHECKSUM;
    }
    
}