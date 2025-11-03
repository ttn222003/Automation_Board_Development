#include "CppUTest/TestHarness.h"
extern "C" {
    #include "UartBootloaderProtocolCore.h"
}

TEST_GROUP(BootloaderProtocolCoreGetCmd)
{
    
};

IGNORE_TEST(BootloaderProtocolCoreGetCmd, CalculateCRC32DataAllZero)
{
    unsigned char data[] = { 0x00, 0x00, 0x00, 0x00 };
    uint32_t crc32 = CalculateCrc32(data, 4);
    CHECK_EQUAL(0x00000000, crc32);
}

IGNORE_TEST(BootloaderProtocolCoreGetCmd, CalculateCRC32LastByteOne)
{
    unsigned char data[] = { 0x00, 0x00, 0x00, 0x01 };
    uint32_t crc32 = CalculateCrc32(data, 4);
    CHECK_EQUAL(0x9823B6E, crc32);
}

IGNORE_TEST(BootloaderProtocolCoreGetCmd, CalculateCRC32Random4ByteData)
{
    unsigned char data[] = { 0x12, 0x34, 0x56, 0x78 };
    uint32_t crc32 = CalculateCrc32(data, 4);
    CHECK_EQUAL(0xD0202C9A, crc32);
}

IGNORE_TEST(BootloaderProtocolCoreGetCmd, CalculateCRC32Random5ByteData)
{
    unsigned char data[] = { 0x12, 0x34, 0x56, 0x78, 0x00 };
    uint32_t crc32 = CalculateCrc32(data, 5);
    CHECK_EQUAL(0x2C90DB04, crc32);
}

TEST(BootloaderProtocolCoreGetCmd, GetCommandFollowVersion10)
{
    uint8_t data[MAX_DATA_LEN] = { 0 };
    uint8_t expected_data[MAX_DATA_LEN] = { 0x79, 0x07, 0x10, 0x00, 0x01, 0x02, 0x11, 0x21, 0x31, 0x43, 0xA1 };
    GetCommand(data, 10);
    MEMCMP_EQUAL(expected_data, data, 11);
}

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

