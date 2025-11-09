#include "UartBootloaderProtocolCore.h"

/* Private variable */
static unsigned int polynomial_crc32 = 0x04C11DB7;
static uint8_t polynomial_crc8 = 0xB1;
/*----------------*/

uint8_t CheckCommandCode(uint8_t received_data[])
{
    return received_data[0];
}

static uint32_t CalculateCrc32(uint8_t data[], uint8_t data_length)
{
    unsigned int crc = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | (data[3] << 0);

    for (uint16_t data_index = 4; data_index < data_length; data_index++)
    {
        for(int8_t i = 7; i >= 0; i--)
        {
            if(crc & 0x80000000)
            {
                crc ^= polynomial_crc32;
            }
            crc = crc << 1;

            crc |= ((data[data_index] >> i) & 0x01);
        }
    }

    for (uint8_t i = 0; i < 32; i++)
    {
        if(crc & 0x80000000)
        {
            crc ^= polynomial_crc32;
        }
        crc = crc << 1;
    }
    
    return crc;
}

static uint8_t ChecksumXOR(uint8_t data)
{
    return data ^ 0xFF;
}

void GetCommand(uint8_t* data_cmds, uint8_t protocol_version)
{
    data_cmds[0] = ACK;
    if(protocol_version == PROTOCOL_VER_10)
    {
        data_cmds[1] = 0x07;
        data_cmds[2] = ((protocol_version / 10) << 4) | (protocol_version % 10);
        data_cmds[3] = GET_CMD;
        data_cmds[4] = GET_VERSION;
        data_cmds[5] = GET_ID;
        data_cmds[6] = READ_MEM;
        data_cmds[7] = GO_CMD;
        data_cmds[8] = WRITE_MEM;
        data_cmds[9] = ERASE_MEM;
        data_cmds[10] = GET_CHECKSUM;
    }
}

void GetVersion(uint8_t* data_version, uint8_t protocol_version)
{
    data_version[0] = ACK;
    data_version[1] = ((protocol_version / 10) << 4) | (protocol_version % 10);
    data_version[2] = 0x00;
    data_version[3] = 0x00;
    data_version[4] = ACK;
}

void GetId(uint8_t* data_id, uint8_t id)
{
    data_id[0] = ACK;
    data_id[1] = 0x01;
    data_id[2] = 0x04;
    data_id[3] = id;
    data_id[4] = ACK;
}

uint32_t ProcessAddressAndChecksum(uint8_t* received_data)
{
    uint32_t crc = CalculateCrc32(received_data, 8);
    uint32_t address;

    if(crc == 0x00000000) {
        address = (received_data[0] << 24) | (received_data[1] << 16) | (received_data[2] << 8) | received_data[3];
    }
    else {
        return 0;
    }
    
    if((address < 0x08004400) && (address > 0x08010000)) {
        return 0;
    }

    return address;
}

bool CheckNumberOfByteAndChecksum(uint8_t received_data[])
{
    uint8_t checksum = ChecksumXOR(received_data[1]);
    
    if(received_data[0] == checksum)
    {
        return true;
    }

    return false;
}

void PrepareDataToSendToHost(uint8_t* prepared_data, uint32_t start_address, uint8_t number_of_bytes)
{
   memcpy(prepared_data, (uint8_t*)start_address, number_of_bytes); 
}

void GotoApplication(uint32_t start_address)
{
    void (*application_reset_handler)(void) = (void*)(*((volatile uint32_t*)(start_address + 4U)));
    application_reset_handler();
}