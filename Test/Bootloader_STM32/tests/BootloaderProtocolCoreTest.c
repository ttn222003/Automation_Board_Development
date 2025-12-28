#include "unity.h"
#include "unity_color.h"
#include "UartBootloaderProtocolCore.h"
#include <stdio.h>

UartBootloaderProtocolDevice_t mUartBootloaderTest;
uint8_t transmitt_data[64];

void setUp(void)
{
    memset(transmitt_data, 0, 64);
}

void tearDown(void)
{

}

void ParseFrameHandshakeRequestOfGetCommandTest(void)
{
    uint8_t received_data[8] = { REQUEST_HANDSHAKE, 0x08, GET_CMD, 0xFF - GET_CMD, 0x8A, 0xC2, 0xA9, 0xFA };
    ParseFrameHandshakeRequestGetCommandFromHost(&mUartBootloaderTest, received_data);
    TEST_ASSERT_EQUAL(GET_CMD, GetCommandCode(mUartBootloaderTest));
}

void ParseFrameHandshakeRequestOfGetCommandButByteDataLengthIsWrongTest(void)
{
    uint8_t received_data[8] = { REQUEST_HANDSHAKE, 0x07, GET_CMD, 0xFF - GET_CMD, 0x8A, 0xC2, 0xA9, 0xFA };
    ParseFrameHandshakeRequestGetCommandFromHost(&mUartBootloaderTest, received_data);
    TEST_ASSERT_EQUAL(NOT_CODE, GetCommandCode(mUartBootloaderTest));
}

void HandleAckTest(void)
{
    uint8_t expected_data[8] = { RESPONE_HANDSHAKE, 0x08, ACK, 0x00, 0x69,0x0D, 0xDF, 0x72 };
    HandleAckForTransmission(transmitt_data);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_data, transmitt_data, 8);
}

void HandleNackTest(void)
{
    uint8_t expected_data[8] = { RESPONE_HANDSHAKE, 0x08, NACK, 0x00, 0x88, 0x9E, 0x7F, 0x1A };
    HandleNackForTransmission(transmitt_data);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_data, transmitt_data, 8);
}

void ParseFrameDataRequestOfGetCommandTest(void)
{
    uint8_t received_data[8] = { REQUEST_DATA, 0x08, GET_CMD, 0xFF - GET_CMD, 0x6E, 0x2E, 0x0C, 0x64 };
    ParseFrameDataRequestGetCommandFromHost(&mUartBootloaderTest, received_data);
    TEST_ASSERT_EQUAL(GET_CMD, GetCommandCode(mUartBootloaderTest));
}

void ParseFrameDataRequestOfGetCommandButNotBytesTest(void)
{
    uint8_t received_data[8] = { REQUEST_DATA, 0x08, GET_CMD, 0xFF - GET_CMD, 0x6E, 0x2E, 0x0C };
    ParseFrameDataRequestGetCommandFromHost(&mUartBootloaderTest, received_data);
    TEST_ASSERT_EQUAL(NOT_CODE, GetCommandCode(mUartBootloaderTest));
}

void HandleDataGetCmdTest(void)
{
    uint8_t expected_data[21] = { RESPONSE_DATA, 21, GET_CMD, GET_VERSION, GET_ID, READ_MEM, GO_CMD, WRITE_MEM, ERASE_MEM, EXT_ERASE_MEM, WRITE_PROTECT, WRITE_UNPROTECT, READOUT_PROTECT, READOUT_UNPROTECT, GET_CHECKSUM, SPECIAL_CMD, EXT_SPECIAL_CMD, 0x4B, 0xA5, 0x3E, 0x9C };
    HandleDataGetCommandForTransmission(transmitt_data);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_data, transmitt_data, 21);
}

void ParseFrameEndHandshakeOfGetCommandTest(void)
{
    uint8_t received_data[8] = { END_HANDSHAKE, 0x08, GET_CMD, 0xFF - GET_CMD, 0xE6, 0xFE, 0xF5, 0x86 };
    ParseFrameEndHandshakeGetCommandFromHost(&mUartBootloaderTest, received_data);
    TEST_ASSERT_EQUAL(GET_CMD, GetCommandCode(mUartBootloaderTest));
}

int main(void)
{
    printf(COL_YELLOW "==== START CORE TEST ====\n" COL_RESET);
    UNITY_BEGIN();
    RUN_TEST(ParseFrameHandshakeRequestOfGetCommandTest);
    RUN_TEST(ParseFrameHandshakeRequestOfGetCommandButByteDataLengthIsWrongTest);
    RUN_TEST(HandleAckTest);
    RUN_TEST(HandleNackTest);
    RUN_TEST(ParseFrameDataRequestOfGetCommandTest);
    RUN_TEST(ParseFrameDataRequestOfGetCommandButNotBytesTest);
    RUN_TEST(HandleDataGetCmdTest);
    RUN_TEST(ParseFrameEndHandshakeOfGetCommandTest);
    printf(COL_YELLOW "==== END CORE TEST ====\n" COL_RESET);
    return UNITY_END();
}