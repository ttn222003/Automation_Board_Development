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