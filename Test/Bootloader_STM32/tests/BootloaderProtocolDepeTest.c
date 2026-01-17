#include "unity.h"
#include "unity_color.h"
#include "UartBootloaderProtocolDepenedencies.h"
#include "MockUartDriver.h"
#include "MockTimerDriver.h"

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
void TransmittEightDataToHostTest(void)
{
    HandleAckForTransmission(TransmittedDataToHost);
    // Expect
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(RESPONE_HANDSHAKE);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(8);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(ACK);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(0);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(105);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(13);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(223);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(114);

    TransmittDataToHost(8);
}

/*
TEST 2
*/
void TransmittTwentyOneDataToHostTest(void)
{
    HandleDataGetCommandForTransmission(TransmittedDataToHost);

    // Expect
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(RESPONSE_DATA);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(21);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(GET_CMD);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(GET_VERSION);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(GET_ID);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(READ_MEM);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(GO_CMD);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(WRITE_MEM);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(ERASE_MEM);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(EXT_ERASE_MEM);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(WRITE_PROTECT);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(WRITE_UNPROTECT);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(READOUT_PROTECT);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(READOUT_UNPROTECT);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(GET_CHECKSUM);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(SPECIAL_CMD);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(EXT_SPECIAL_CMD);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(75);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(165);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(62);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(156);

    TransmittDataToHost(TransmittedDataToHost[1]);
}

/*
TEST 3
*/
void ReceiveRequestHandshakeAndProcessBufferTest(void)
{
    uint8_t expected_data[8] = {0x77, 8, 0, 0xFF, 0x8A, 0xC2, 0xA9, 0xFA };

    uint8_t received_data;
    uint8_t status;

    received_data = 0x77;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 8;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0xFF;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0x8A;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0xC2;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0xA9;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0xFA;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_ABLE_TO_PROCESS, status);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_data, ReceivedDataBuffer, 8);
}

/*
TEST 4
*/
void ReceiveRequestHandshakeButFirstByteIsWrongAndProcessBufferTest(void)
{
    uint8_t expected_data[8] = { 0 };
    memset(expected_data, 0, 8);

    uint8_t received_data;
    uint8_t status;

    received_data = 0x76;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 8;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);
    
    received_data = 0;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0xFF;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0x8A;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0xC2;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0xA9;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0xFA;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_data, ReceivedDataBuffer, 8);
}

/*
TEST 5
*/
void ReceiveRequestDataAndProcessBufferTest(void)
{
    uint8_t expected_data[8] = {0xFE, 8, 0, 0xFF, 0x6E, 0x2E, 0x0C, 0x64 };

    uint8_t received_data;
    uint8_t status;

    received_data = 0xFE;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 8;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0xFF;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0x6E;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0x2E;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0x0C;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_UNABLE_TO_PROCESS, status);

    received_data = 0x64;
    status = ReceiveDataAndPutInBuffer(received_data);
    TEST_ASSERT_EQUAL(FRAME_ABLE_TO_PROCESS, status);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_data, ReceivedDataBuffer, 8);
}

// /*
// TEST 6: This test is tested for condition
// CommandCode == GET_CMD && Phase == REQUEST_HANDSHAKE
// */

// void ReceivedDataIsGetCmdAndPhaseIsRequestHandshake(void)
// {
//     HandleAckForTransmission(TransmittedDataToHost);

//     // Expect
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(RESPONE_HANDSHAKE);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(8);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(ACK);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(0);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(105);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(13);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(223);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(114);

//     TransmittDataToHost(TransmittedDataToHost[1]);
// }

// /*
// TEST 7: This test is tested for condition
// CommandCode == GET_CMD && Phase == REQUEST_DATAw
// */
// void ReceivedDataIsGetCmdAndPhaseIsRequestData(void)
// {
//     HandleDataGetCommandForTransmission(TransmittedDataToHost);

//     // Expect
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(RESPONSE_DATA);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(21);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(GET_CMD);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(GET_VERSION);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(GET_ID);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(READ_MEM);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(GO_CMD);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(WRITE_MEM);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(ERASE_MEM);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(EXT_ERASE_MEM);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(WRITE_PROTECT);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(WRITE_UNPROTECT);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(READOUT_PROTECT);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(READOUT_UNPROTECT);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(GET_CHECKSUM);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(SPECIAL_CMD);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(EXT_SPECIAL_CMD);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(75);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(165);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(62);
//     DelayMs_Expect(1);
//     UartTransmittOneByteData_Expect(156);

//     TransmittDataToHost(TransmittedDataToHost[1]);
// }

int main(void)
{
    printf(COL_YELLOW "==== START DEPE TEST ====\n" COL_RESET);
    UNITY_BEGIN();
    RUN_TEST(TransmittEightDataToHostTest);
    RUN_TEST(TransmittTwentyOneDataToHostTest);
    RUN_TEST(ReceiveRequestHandshakeAndProcessBufferTest);
    RUN_TEST(ReceiveRequestHandshakeButFirstByteIsWrongAndProcessBufferTest);
    RUN_TEST(ReceiveRequestDataAndProcessBufferTest);
    // RUN_TEST(ReceivedDataIsGetCmdAndPhaseIsRequestHandshake);
    // RUN_TEST(ReceivedDataIsGetCmdAndPhaseIsRequestData);
    printf(COL_YELLOW "==== END DEPE TEST ====\n" COL_RESET);
    return UNITY_END();
}