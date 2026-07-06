/*
 * Test_MainComn2Wifi_and_UartCommon.c
 *
 * Integration tests for the protocol core and UART dependency layer.
 * Boundary under test:
 *   MainComn2WifiCard BuildFrame/ParseFrame/GetFrame <-> UartCommon Send/Read/ISR buffer.
 */

#include "unity.h"
#include "MainComn2WifiCard.h"
#include "UartCommon.h"
#include "Mockstm32f1xx_hal_uart.h"

#include <stdint.h>
#include <string.h>

static UART_HandleTypeDef sDummyHandle;

void setUp(void)
{
    Mockstm32f1xx_hal_uart_Init();
    ResetBspUartState();
    TEST_ASSERT_EQUAL(BSP_UART_OK, InitBspUart(&sDummyHandle));
}

void tearDown(void)
{
    Mockstm32f1xx_hal_uart_Verify();
    Mockstm32f1xx_hal_uart_Destroy();
}

static void PushBytesIntoUartRx(const uint8_t *data, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++) {
        HandleBspUartIsrRx(data[i]);
    }
}

static uint16_t BuildValidFrame(CommandType_t cmd,
                                const uint8_t *payload,
                                uint8_t payloadLen,
                                uint8_t *frame)
{
    uint16_t frameLen = 0;
    FrameBuildStatus_t buildStatus = BuildFrame(cmd, payload, payloadLen, frame, &frameLen);

    TEST_ASSERT_EQUAL(BUILT_FRAME_OK, buildStatus);
    TEST_ASSERT_EQUAL_UINT16((uint16_t)(payloadLen + 6U), frameLen);

    return frameLen;
}

static void ReadAvailableUartBytes(uint8_t *outFrame, uint16_t expectedLen)
{
    uint16_t readLen = 0;
    BspUartStatus_t readStatus = ReadBspUart(outFrame, MAX_FRAME_LEN, &readLen);

    TEST_ASSERT_EQUAL(BSP_UART_OK, readStatus);
    TEST_ASSERT_EQUAL_UINT16(expectedLen, readLen);
    TEST_ASSERT_EQUAL_UINT16(0, GetBspUartAvailable());
}

static void ExpectUartTransmitOk(uint8_t *frame, uint16_t frameLen)
{
    HAL_UART_Transmit_ExpectAndReturn(
        &sDummyHandle, frame, frameLen, BSP_UART_TX_TIMEOUT_MS, HAL_OK);
}

/*
 * Update reason: ParseFrame() now stores decoded data internally. Integration
 * tests must call GetFrame() after PARSED_FRAME_OK to verify the frame that
 * other RTOS tasks will consume through the public protocol API.
 */
static FrameParseStatus_t ParseAndGetFrame(const uint8_t *raw,
                                           uint16_t rawLen,
                                           FrameStructure_t *frame)
{
    FrameParseStatus_t result = ParseFrame(raw, rawLen);

    if ((result == PARSED_FRAME_OK) && (frame != NULL)) {
        GetFrame(frame);
    }

    return result;
}

/*
 * SUITE 1 - Protocol TX path into UartCommon.
 */

/*
 * Test case: 1.1
 * Add reason: Cover the STM32 heartbeat response TX path. The communication
 * task will need to build CMD_HEARTBEAT_RSP with no payload and pass exactly
 * that 6-byte frame to UartCommon/HAL.
 */
void TestBuildHeartbeatResponseThenSendOverUart(void)
{
    uint8_t frame[MAX_FRAME_LEN];
    uint16_t frameLen = BuildValidFrame(CMD_HEARTBEAT_RSP, NULL, 0, frame);

    TEST_ASSERT_EQUAL_UINT8(SOF_D, frame[0]);
    TEST_ASSERT_EQUAL_UINT8(0, frame[1]);
    TEST_ASSERT_EQUAL_UINT8(CMD_HEARTBEAT_RSP, frame[2]);
    TEST_ASSERT_EQUAL_UINT8(EOF_D, frame[frameLen - 1U]);

    ExpectUartTransmitOk(frame, frameLen);
    TEST_ASSERT_EQUAL(BSP_UART_OK, SendBspUart(&sDummyHandle, frame, frameLen));
}

/*
 * Test case: 1.2
 * Add reason: Cover NACK TX format at the integration boundary. A protocol
 * error response must be sent as CMD_NACK and its payload must identify the
 * failed command plus the NACK error code.
 */
void TestBuildNackForCrcFailThenSendOverUart(void)
{
    uint8_t nackPayload[] = {
        (uint8_t)CMD_DATA_PUSH,
        (uint8_t)NACK_ERR_CRC_FAIL,
    };
    uint8_t frame[MAX_FRAME_LEN];
    uint16_t frameLen = BuildValidFrame(CMD_NACK,
                                        nackPayload,
                                        (uint8_t)sizeof(nackPayload),
                                        frame);

    FrameStructure_t parsedFrame;
    TEST_ASSERT_EQUAL(PARSED_FRAME_OK, ParseAndGetFrame(frame, frameLen, &parsedFrame));
    TEST_ASSERT_EQUAL_UINT8(CMD_NACK, parsedFrame.mCommand);
    TEST_ASSERT_EQUAL_UINT16(sizeof(nackPayload), parsedFrame.mLength);
    TEST_ASSERT_EQUAL_UINT8(CMD_DATA_PUSH, parsedFrame.mDataPayload[0]);
    TEST_ASSERT_EQUAL_UINT8(NACK_ERR_CRC_FAIL, parsedFrame.mDataPayload[1]);

    ExpectUartTransmitOk(frame, frameLen);
    TEST_ASSERT_EQUAL(BSP_UART_OK, SendBspUart(&sDummyHandle, frame, frameLen));
}

/*
 * SUITE 2 - UartCommon RX path into protocol parser.
 */

/*
 * Test case: 2.1
 * Add reason: Cover the RX path for CMD_HEARTBEAT_REQ. This is the exact
 * command that main communication logic should consume from UART and answer
 * with CMD_HEARTBEAT_RSP later.
 */
void TestReceiveHeartbeatRequestFromUartThenParse(void)
{
    uint8_t txFrame[MAX_FRAME_LEN];
    uint16_t txFrameLen = BuildValidFrame(CMD_HEARTBEAT_REQ, NULL, 0, txFrame);

    PushBytesIntoUartRx(txFrame, txFrameLen);
    TEST_ASSERT_EQUAL_UINT16(txFrameLen, GetBspUartAvailable());

    uint8_t rxFrame[MAX_FRAME_LEN];
    ReadAvailableUartBytes(rxFrame, txFrameLen);

    FrameStructure_t parsedFrame;
    TEST_ASSERT_EQUAL(PARSED_FRAME_OK, ParseAndGetFrame(rxFrame, txFrameLen, &parsedFrame));
    TEST_ASSERT_EQUAL_UINT8(CMD_HEARTBEAT_REQ, parsedFrame.mCommand);
    TEST_ASSERT_EQUAL_UINT16(0, parsedFrame.mLength);
}

/*
 * Test case: 2.2
 * Add reason: Cover the RX path for a normal CMD_DATA_PUSH frame, including
 * a 0x00 payload byte. This verifies UART buffering does not corrupt protocol
 * payload before ParseFrame consumes it.
 */
void TestReceiveDataPushFromUartThenParsePayload(void)
{
    uint8_t payload[] = {0x11, 0x22, 0x00, 0x33};
    uint8_t txFrame[MAX_FRAME_LEN];
    uint16_t txFrameLen = BuildValidFrame(CMD_DATA_PUSH,
                                          payload,
                                          (uint8_t)sizeof(payload),
                                          txFrame);

    PushBytesIntoUartRx(txFrame, txFrameLen);

    uint8_t rxFrame[MAX_FRAME_LEN];
    ReadAvailableUartBytes(rxFrame, txFrameLen);

    FrameStructure_t parsedFrame;
    TEST_ASSERT_EQUAL(PARSED_FRAME_OK, ParseAndGetFrame(rxFrame, txFrameLen, &parsedFrame));
    TEST_ASSERT_EQUAL_UINT8(CMD_DATA_PUSH, parsedFrame.mCommand);
    TEST_ASSERT_EQUAL_UINT16(sizeof(payload), parsedFrame.mLength);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(payload, parsedFrame.mDataPayload, sizeof(payload));
}

/*
 * SUITE 3 - Error feedback across protocol and UART.
 */

/*
 * Test case: 3.1
 * Add reason: Cover the expected integration behavior after UART receives a
 * frame with bad CRC: ParseFrame must report CRC error, then the caller can
 * build and send CMD_NACK with NACK_ERR_CRC_FAIL for the failed command.
 */
void TestCrcErrorFromUartRxBuildsAndSendsNack(void)
{
    uint8_t payload[] = {0x44, 0x55};
    uint8_t badFrame[MAX_FRAME_LEN];
    uint16_t badFrameLen = BuildValidFrame(CMD_DATA_PUSH,
                                           payload,
                                           (uint8_t)sizeof(payload),
                                           badFrame);

    badFrame[badFrameLen - 3U] ^= 0x01U;

    PushBytesIntoUartRx(badFrame, badFrameLen);

    uint8_t rxFrame[MAX_FRAME_LEN];
    ReadAvailableUartBytes(rxFrame, badFrameLen);

    TEST_ASSERT_EQUAL(PARSED_FRAME_ERR_CRC, ParseFrame(rxFrame, badFrameLen));

    uint8_t nackPayload[] = {
        (uint8_t)CMD_DATA_PUSH,
        (uint8_t)NACK_ERR_CRC_FAIL,
    };
    uint8_t nackFrame[MAX_FRAME_LEN];
    uint16_t nackFrameLen = BuildValidFrame(CMD_NACK,
                                            nackPayload,
                                            (uint8_t)sizeof(nackPayload),
                                            nackFrame);

    ExpectUartTransmitOk(nackFrame, nackFrameLen);
    TEST_ASSERT_EQUAL(BSP_UART_OK, SendBspUart(&sDummyHandle, nackFrame, nackFrameLen));
}
