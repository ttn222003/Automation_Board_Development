#include "CppUTest/TestHarness.h"
extern "C" {
    #include "UartBootloaderProtocolCore.h"
}

// #define DISABLE_TEST_GROUP  1

#ifdef DISABLE_TEST_GROUP
TEST_GROUP(BootloaderProtocolCoreGetCmd)
{
    
};

TEST(BootloaderProtocolCoreGetCmd, GetVersionFollowVersion10)
{
    uint8_t data[MAX_DATA_LEN] = { 0 };
    uint8_t expected_data[MAX_DATA_LEN] = { 0x79, 0x10, 0x00, 0x00, 0x79 };
    GetVersion(data, 10);
    MEMCMP_EQUAL(expected_data, data, 5);
}

TEST(BootloaderProtocolCoreGetCmd, GetIdFollowVersion10)
{
    uint8_t data[MAX_DATA_LEN] = { 0 };
    uint8_t expected_data[MAX_DATA_LEN] = { 0x79, 0x01, 0x04, 0x01, 0x79 };
    GetId(data, 1);
    MEMCMP_EQUAL(expected_data, data, 5);
}

TEST(BootloaderProtocolCoreGetCmd, ProcessAddress)
{
    uint8_t data[MAX_DATA_LEN] = { 0x08, 0x01, 0x55, 0x55, 0x89, 0xA1, 0x69, 0x60 };
    uint32_t expected_data = 0x08015555;
    uint32_t output_data = ProcessAddressAndChecksum(data);
    CHECK_EQUAL(expected_data, output_data);
}

TEST(BootloaderProtocolCoreGetCmd, ReceiveNumberOfBytesAndChecksumForReadMem)
{
    uint8_t data[2] = { 0x04, 0xFB };
    uint32_t pass_checksum = CheckNumberOfByteAndChecksum(data);
    CHECK_EQUAL(1, pass_checksum);
}

IGNORE_TEST(BootloaderProtocolCoreGetCmd, PreparedDataSendToHost) // Try with mock later
{
    uint32_t start_address = 0x08015555;
    uint8_t number_of_bytes = 4;
    uint8_t transmitted_data[MAX_DATA_LEN] = { 0 };
    uint8_t expected_data[MAX_DATA_LEN] = { 0x12, 0x34, 0x56, 0x78 };
    *(uint32_t*)0x08015555 = 0x12345678;
    PrepareDataToSendToHost(transmitted_data, start_address, number_of_bytes);
    MEMCMP_EQUAL(expected_data, transmitted_data, 4);
}
#endif