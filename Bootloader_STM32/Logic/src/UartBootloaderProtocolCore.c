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