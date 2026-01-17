#include "unity.h"
#include "unity_color.h"
#include "UartBootloaderProtocolDepeHost.h"
#include "MockTimerDriver.h"
#include "MockUartDriver.h"

UartHandleType uart1;

void setUp(void)
{
    ResetDataBuffer();
}

void tearDown(void)
{

}

/*
TEST 1
*/
void TransmittEightDataToDeviceTest(void)
{
    HandleHandshakeRequestOfGetCommandForTransmission(TransmittedDataToDevice);
    // Expect
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(uart1, REQUEST_HANDSHAKE);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(uart1, 8);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(uart1, GET_CMD);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(uart1, 0xFF);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(uart1, 0x8A);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(uart1, 0xC2);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(uart1, 0xA9);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(uart1, 0xFA);

    TransmittDataToDevice(uart1, 8);
}

/*
TEST 2
*/
void ReceiveResponseHandshakeAndProcessBufferTest(void)
{
    uint8_t expected_data[8] = {RESPONE_HANDSHAKE, 8, ACK, 0x00, 0x69, 0x0D, 0xDF, 0x72 };

    uint8_t received_data;
    uint8_t status;

    received_data = 120;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 8;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = ACK;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0x00;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0x69;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0x0D;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0xDF;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0x72;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_ABLE_TO_PROCESS, status);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_data, ReceivedDataBuffer, 8);
}

/*
TEST 3
*/

void ReceiveResponseDataAndProcessBufferTest(void)
{
    uint8_t expected_data[21] = { RESPONSE_DATA, 21, GET_CMD, GET_VERSION, GET_ID, READ_MEM, GO_CMD, WRITE_MEM, ERASE_MEM, EXT_ERASE_MEM, WRITE_PROTECT, WRITE_UNPROTECT, READOUT_PROTECT, READOUT_UNPROTECT, GET_CHECKSUM, SPECIAL_CMD, EXT_SPECIAL_CMD, 0x4B, 0xA5, 0x3E, 0x9C };

    uint8_t received_data;
    uint8_t status;

    received_data = RESPONSE_DATA;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 21;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = GET_CMD;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = GET_VERSION;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = GET_ID;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = READ_MEM;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = GO_CMD;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = WRITE_MEM;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = ERASE_MEM;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = EXT_ERASE_MEM;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = WRITE_PROTECT;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = WRITE_UNPROTECT;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = READOUT_PROTECT;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = READOUT_UNPROTECT;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = GET_CHECKSUM;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = SPECIAL_CMD;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = EXT_SPECIAL_CMD;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0x4B;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0xA5;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0x3E;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0x9C;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_ABLE_TO_PROCESS, status);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_data, ReceivedDataBuffer, 21);
}

int main(void)
{
    printf(COL_YELLOW "==== START DEPE TEST ====\n" COL_RESET);
    UNITY_BEGIN();
    RUN_TEST(TransmittEightDataToDeviceTest);
    RUN_TEST(ReceiveResponseHandshakeAndProcessBufferTest);
    RUN_TEST(ReceiveResponseDataAndProcessBufferTest);
    printf(COL_YELLOW "==== END DEPE TEST ====\n" COL_RESET);
    return UNITY_END();
}