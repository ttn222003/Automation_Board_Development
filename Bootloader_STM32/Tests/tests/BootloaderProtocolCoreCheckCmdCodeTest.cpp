#include "CppUTest/TestHarness.h"
extern "C" {
    #include "UartBootloaderProtocolCore.h"
}

TEST_GROUP(BootloaderProtocolCoreCheckCmdCode)
{
    
};

TEST(BootloaderProtocolCoreCheckCmdCode, CheckCmdCodeGet)
{
    unsigned char data[2] = {0x00, 0xFF};
    unsigned char command_code = CheckCommandCode(data);
    CHECK_EQUAL(GET_CMD, command_code);
}

TEST(BootloaderProtocolCoreCheckCmdCode, CheckCmdCodeGetVersion)
{
    unsigned char data[2] = {0x01, 0xFE};
    unsigned char command_code = CheckCommandCode(data);
    CHECK_EQUAL(GET_VERSION, command_code);
}

TEST(BootloaderProtocolCoreCheckCmdCode, CheckCmdCodeGetID)
{
    unsigned char data[2] = {0x02, 0xFD};
    unsigned char command_code = CheckCommandCode(data);
    CHECK_EQUAL(GET_ID, command_code);
}

TEST(BootloaderProtocolCoreCheckCmdCode, CheckCmdCodeReadMemory)
{
    unsigned char data[2] = {0x11, 0xEE};
    unsigned char command_code = CheckCommandCode(data);
    CHECK_EQUAL(READ_MEM, command_code);
}

TEST(BootloaderProtocolCoreCheckCmdCode, CheckCmdCodeGo)
{
    unsigned char data[2] = {0x21, 0xDE};
    unsigned char command_code = CheckCommandCode(data);
    CHECK_EQUAL(GO_CMD, command_code);
}

TEST(BootloaderProtocolCoreCheckCmdCode, CheckCmdCodeWriteMemory)
{
    unsigned char data[2] = {0x31, 0xCE};
    unsigned char command_code = CheckCommandCode(data);
    CHECK_EQUAL(WRITE_MEM, command_code);
}

TEST(BootloaderProtocolCoreCheckCmdCode, CheckCmdCodeEraseMemory)
{
    unsigned char data[2] = {0x43, 0xBC};
    unsigned char command_code = CheckCommandCode(data);
    CHECK_EQUAL(ERASE_MEM, command_code);
}

TEST(BootloaderProtocolCoreCheckCmdCode, CheckCmdCodeExtendEraseMemory)
{
    unsigned char data[2] = {0x44, 0xBB};
    unsigned char command_code = CheckCommandCode(data);
    CHECK_EQUAL(EXT_ERASE_MEM, command_code);
}

TEST(BootloaderProtocolCoreCheckCmdCode, CheckCmdCodeWriteProtect)
{
    unsigned char data[2] = {0x63, 0x9C};
    unsigned char command_code = CheckCommandCode(data);
    CHECK_EQUAL(WRITE_PROTECT, command_code);
}

TEST(BootloaderProtocolCoreCheckCmdCode, CheckCmdCodeWriteUnprotect)
{
    unsigned char data[2] = {0x73, 0x8C};
    unsigned char command_code = CheckCommandCode(data);
    CHECK_EQUAL(WRITE_UNPROTECT, command_code);
}

TEST(BootloaderProtocolCoreCheckCmdCode, CheckCmdCodeReadoutProtect)
{
    unsigned char data[2] = {0x82, 0x7D};
    unsigned char command_code = CheckCommandCode(data);
    CHECK_EQUAL(READOUT_PROTECT, command_code);
}

TEST(BootloaderProtocolCoreCheckCmdCode, CheckCmdCodeReadoutUnprotect)
{
    unsigned char data[2] = {0x92, 0x6D};
    unsigned char command_code = CheckCommandCode(data);
    CHECK_EQUAL(READOUT_UNPROTECT, command_code);
}

TEST(BootloaderProtocolCoreCheckCmdCode, CheckCmdCodeGetChecksum)
{
    unsigned char data[2] = {0xA1, 0x5E};
    unsigned char command_code = CheckCommandCode(data);
    CHECK_EQUAL(GET_CHECKSUM, command_code);
}

TEST(BootloaderProtocolCoreCheckCmdCode, CheckCmdCodeSpecialCmd)
{
    unsigned char data[2] = {0x50, 0xAF};
    unsigned char command_code = CheckCommandCode(data);
    CHECK_EQUAL(SPECIAL_CMD, command_code);
}

TEST(BootloaderProtocolCoreCheckCmdCode, CheckCmdCodeExtendSpecialCmd)
{
    unsigned char data[2] = {0x51, 0xAE};
    unsigned char command_code = CheckCommandCode(data);
    CHECK_EQUAL(EXT_SPECIAL_CMD, command_code);
}