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

// Add test for parse frame here
void ParseFrameTest(void)
{   
    uint8_t received_data[8] = { REQUEST_HANDSHAKE, 0x08, GET_CMD, 0xFF - GET_CMD, 0x8A, 0xC2, 0xA9, 0xFA };
    uint8_t status = ParseFrameFromHost(&mUartBootloaderTest, received_data);

    TEST_ASSERT_EQUAL(GET_CMD, GetCommandCode(mUartBootloaderTest));
    TEST_ASSERT_EQUAL(FRAME_OK, status);
}

void ParseFrameButByteDataLengthIsWrongTest(void)
{
    uint8_t received_data[8] = { REQUEST_HANDSHAKE, 0x07, GET_CMD, 0xFF - GET_CMD, 0x8A, 0xC2, 0xA9, 0xFA };
    uint8_t status = ParseFrameFromHost(&mUartBootloaderTest, received_data);

    TEST_ASSERT_EQUAL(NOT_CODE, GetCommandCode(mUartBootloaderTest));
    TEST_ASSERT_EQUAL(FRAME_ERROR, status);
}

void ParseFrameButNotEnoughBytesTest(void)
{
    uint8_t received_data[8] = { REQUEST_DATA, 0x08, GET_CMD, 0xFF - GET_CMD, 0x6E, 0x2E, 0x0C };
    uint8_t status = ParseFrameFromHost(&mUartBootloaderTest, received_data);

    TEST_ASSERT_EQUAL(NOT_CODE, GetCommandCode(mUartBootloaderTest));
    TEST_ASSERT_EQUAL(FRAME_ERROR, status);
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

void HandleDataGetCmdTest(void)
{
    uint8_t expected_data[21] = { RESPONSE_DATA, 21, GET_CMD, GET_VERSION, GET_ID, READ_MEM, GO_CMD, WRITE_MEM, ERASE_MEM, EXT_ERASE_MEM, WRITE_PROTECT, WRITE_UNPROTECT, READOUT_PROTECT, READOUT_UNPROTECT, GET_CHECKSUM, SPECIAL_CMD, EXT_SPECIAL_CMD, 0x4B, 0xA5, 0x3E, 0x9C };
    HandleDataGetCommandForTransmission(transmitt_data);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_data, transmitt_data, 21);
}

void HandleDataGetVersionTest(void)
{
    uint8_t expected_data[9] = { RESPONSE_DATA, 0x08, 0x10, 0x00, 0x93, 0x01, 0xBE, 0xF2 };
    HandleDataGetVersionForTransmission(transmitt_data, 0x10, 0x00);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_data, transmitt_data, 8);
}

int main(void)
{
    printf(COL_YELLOW "==== START CORE TEST ====\n" COL_RESET);
    UNITY_BEGIN();
    RUN_TEST(ParseFrameTest);
    RUN_TEST(ParseFrameButByteDataLengthIsWrongTest);
    RUN_TEST(ParseFrameButNotEnoughBytesTest);
    RUN_TEST(HandleAckTest);
    RUN_TEST(HandleNackTest);
    RUN_TEST(HandleDataGetCmdTest);
    RUN_TEST(HandleDataGetVersionTest);
    printf(COL_YELLOW "==== END CORE TEST ====\n" COL_RESET);
    return UNITY_END();
}