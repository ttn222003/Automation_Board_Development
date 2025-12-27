#include "unity.h"
#include "UartBootloaderProtocolCore.h"
#include <stdio.h>

UartBootloaderProtocolDevice_t mUartBootloaderTest;

void setUp(void)
{
    
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
    uint8_t transmitt_data[8] = { 0 };
    HandleAckForTransmission(transmitt_data);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_data, transmitt_data, 8);
}

void HandleNackTest(void)
{
    uint8_t expected_data[8] = { RESPONE_HANDSHAKE, 0x08, NACK, 0x00, 0x88, 0x9E, 0x7F, 0x1A };
    uint8_t transmitt_data[8] = { 0 };
    HandleNackForTransmission(transmitt_data);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_data, transmitt_data, 8);
}

void ParseFrameDataRequestOfGetCommandTest(void)
{
    uint8_t received_data[8] = { REQUEST_DATA, 0x08, GET_CMD, 0xFF - GET_CMD, 0x6E, 0x2E, 0x0C, 0x64 };
    ParseFrameDataRequestGetCommandFromHost(&mUartBootloaderTest, received_data);
    TEST_ASSERT_EQUAL(GET_CMD, GetCommandCode(mUartBootloaderTest));
}