#include "CppUTest/TestHarness.h"
extern "C" {
    #include "UartBootloaderProtocolCore.h"
}

TEST_GROUP(BootloaderProtocolCoreGetCmd)
{
    
};

TEST(BootloaderProtocolCoreGetCmd, CalculateCRC32DataAllZero)
{
    unsigned char data[] = { 0x00, 0x00, 0x00, 0x00 };
    uint32_t crc32 = CalculateCrc32(data, 4);
    CHECK_EQUAL(0x00000000, crc32);
}

TEST(BootloaderProtocolCoreGetCmd, CalculateCRC32LastByteOne)
{
    unsigned char data[] = { 0x00, 0x00, 0x00, 0x01 };
    uint32_t crc32 = CalculateCrc32(data, 4);
    CHECK_EQUAL(0x9823B6E, crc32);
}

TEST(BootloaderProtocolCoreGetCmd, CalculateCRC32Random4ByteData)
{
    unsigned char data[] = { 0x12, 0x34, 0x56, 0x78 };
    uint32_t crc32 = CalculateCrc32(data, 4);
    CHECK_EQUAL(0xD0202C9A, crc32);
}

TEST(BootloaderProtocolCoreGetCmd, CalculateCRC32Random5ByteData)
{
    unsigned char data[] = { 0x12, 0x34, 0x56, 0x78, 0x00 };
    uint32_t crc32 = CalculateCrc32(data, 5);
    CHECK_EQUAL(0x2C90DB04, crc32);
}