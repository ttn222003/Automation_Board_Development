/*
 * Test_Uart2Esp.c
 *
 * Unit tests for the core UART protocol layer - STM32 <-> ESP32.
 * Updated: May 08, 2026
 *
 */

#include "unity.h"
#include "uart_protocol.h"

#include <stdint.h>
#include <string.h>

/* ---- Fixtures ---------------------------------------------------------- */

/*
 * Calculate CRC16-MODBUS directly in the test to create golden values.
 * This is a reference implementation, not the code under test.
 */
static uint16_t CalculateReferenceCrc16Modbus(const uint8_t *data, uint16_t len)
{
    uint16_t crc = 0xFFFFu;

    for (uint16_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i];

        for (uint8_t bit = 0; bit < 8; bit++) {
            if ((crc & 0x0001u) != 0u) {
                crc = (uint16_t)((crc >> 1) ^ 0xA001u);
            } else {
                crc >>= 1;
            }
        }
    }

    return crc;
}

/*
 * Build a valid frame for parser tests. The helper asserts immediately if
 * the builder fails, because parser tests need a known-good input frame.
 */
static uint16_t BuildValidFrame(uint8_t devId, uint8_t cmd, const uint8_t *payload, uint8_t payloadLen, uint8_t *out)
{
    uint16_t outLen = 0;
    UartStatus_t ret = uart_build_frame(devId, cmd, payload, payloadLen, out, &outLen);

    TEST_ASSERT_EQUAL(UART_OK, ret);
    return outLen;
}

/* ---- setUp / tearDown -------------------------------------------------- */

void setUp(void)
{
}

void tearDown(void)
{
}

/* =========================================================================
 * SUITE 1 - uart_build_frame()
 * ========================================================================= */

/*
 * Test case: 1.1
 * uart_build_frame() with a 2-byte payload must generate a valid frame.
 */
void TestBuildFrameWithPayload(void)
{
    uint8_t payload[] = {0x10, 0x20};
    uint8_t frame[UART_MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    UartStatus_t ret = uart_build_frame(DEV_ID_ESP32, CMD_DATA_PUSH, payload, 2, frame, &frameLen);

    TEST_ASSERT_EQUAL(UART_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(9, frameLen);
    TEST_ASSERT_EQUAL_HEX8(UART_SOF, frame[0]);
    TEST_ASSERT_EQUAL_UINT8(2, frame[1]);
    TEST_ASSERT_EQUAL_HEX8(DEV_ID_ESP32, frame[2]);
    TEST_ASSERT_EQUAL_HEX8(CMD_DATA_PUSH, frame[3]);
    TEST_ASSERT_EQUAL_HEX8(0x10, frame[4]);
    TEST_ASSERT_EQUAL_HEX8(0x20, frame[5]);

    uint8_t crcInput[] = {2, DEV_ID_ESP32, CMD_DATA_PUSH, 0x10, 0x20};
    uint16_t expectedCrc = CalculateReferenceCrc16Modbus(crcInput, sizeof(crcInput));
    uint16_t actualCrc = ((uint16_t)frame[6] << 8) | frame[7];
    TEST_ASSERT_EQUAL_HEX16(expectedCrc, actualCrc);
    TEST_ASSERT_EQUAL_HEX8(UART_EOF, frame[frameLen - 1]);
}

/*
 * Test case: 1.2
 * uart_build_frame() with len = 0 and payload = NULL must generate a
 * 7-byte frame with no payload.
 */
void TestBuildFrameWithoutPayload(void)
{
    uint8_t frame[UART_MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    UartStatus_t ret = uart_build_frame(DEV_ID_STM32, CMD_HEARTBEAT_REQ, NULL, 0, frame, &frameLen);

    TEST_ASSERT_EQUAL(UART_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(7, frameLen);
    TEST_ASSERT_EQUAL_HEX8(UART_SOF, frame[0]);
    TEST_ASSERT_EQUAL_UINT8(0, frame[1]);
    TEST_ASSERT_EQUAL_HEX8(DEV_ID_STM32, frame[2]);
    TEST_ASSERT_EQUAL_HEX8(CMD_HEARTBEAT_REQ, frame[3]);
    TEST_ASSERT_EQUAL_HEX8(UART_EOF, frame[frameLen - 1]);
}

/*
 * Test case: 1.3
 * uart_build_frame() with UART_MAX_PAYLOAD_LEN bytes must fill exactly
 * UART_MAX_FRAME_LEN bytes.
 */
void TestBuildFrameWithMaxPayload(void)
{
    uint8_t payload[UART_MAX_PAYLOAD_LEN];
    uint8_t frame[UART_MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    memset(payload, 0xFF, sizeof(payload));

    UartStatus_t ret = uart_build_frame(DEV_ID_ESP32, CMD_DATA_PUSH, payload, UART_MAX_PAYLOAD_LEN, frame, &frameLen);

    TEST_ASSERT_EQUAL(UART_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(UART_MAX_FRAME_LEN, frameLen);
    TEST_ASSERT_EQUAL_UINT8(UART_MAX_PAYLOAD_LEN, frame[1]);
    TEST_ASSERT_EQUAL_HEX8(UART_SOF, frame[0]);
    TEST_ASSERT_EQUAL_HEX8(UART_EOF, frame[frameLen - 1]);
}

/*
 * Test case: 1.4
 * uart_build_frame() must reject payloads larger than UART_MAX_PAYLOAD_LEN
 * and leave the output buffer untouched.
 */
void TestRejectOversizedPayload(void)
{
    uint8_t payload[UART_MAX_PAYLOAD_LEN + 1];
    uint8_t frame[UART_MAX_FRAME_LEN + 8];
    uint16_t frameLen = 1234;

    memset(payload, 0xAB, sizeof(payload));
    memset(frame, 0xCC, sizeof(frame));

    UartStatus_t ret = uart_build_frame(DEV_ID_ESP32, CMD_DATA_PUSH, payload, UART_MAX_PAYLOAD_LEN + 1, frame, &frameLen);

    TEST_ASSERT_EQUAL(UART_ERR_PAYLOAD_TOO_LARGE, ret);
    TEST_ASSERT_EQUAL_UINT16(0, frameLen);
    TEST_ASSERT_EQUAL_HEX8(0xCC, frame[0]);
    TEST_ASSERT_EQUAL_HEX8(0xCC, frame[sizeof(frame) - 1]);
}

/*
 * Test case: 1.5
 * uart_build_frame() with a NULL output buffer must return
 * UART_ERR_NULL_PTR.
 */
void TestRejectNullOutputBuffer(void)
{
    uint8_t payload[] = {0x01};
    uint16_t frameLen = 0;

    UartStatus_t ret = uart_build_frame(DEV_ID_ESP32, CMD_DATA_PUSH, payload, 1, NULL, &frameLen);

    TEST_ASSERT_EQUAL(UART_ERR_NULL_PTR, ret);
}

/*
 * Test case: 1.6
 * uart_build_frame() with a NULL output length pointer must return
 * UART_ERR_NULL_PTR.
 */
void TestRejectNullOutputLength(void)
{
    uint8_t payload[] = {0x01};
    uint8_t frame[UART_MAX_FRAME_LEN];

    UartStatus_t ret = uart_build_frame(DEV_ID_ESP32, CMD_DATA_PUSH, payload, 1, frame, NULL);

    TEST_ASSERT_EQUAL(UART_ERR_NULL_PTR, ret);
}

/*
 * Test case: 1.7
 * uart_build_frame() with payload = NULL and len > 0 must return
 * UART_ERR_NULL_PTR.
 */
void TestRejectNullPayloadWhenLengthIsNonzero(void)
{
    uint8_t frame[UART_MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    UartStatus_t ret = uart_build_frame(DEV_ID_ESP32, CMD_DATA_PUSH, NULL, 1, frame, &frameLen);

    TEST_ASSERT_EQUAL(UART_ERR_NULL_PTR, ret);
    TEST_ASSERT_EQUAL_UINT16(0, frameLen);
}

/* =========================================================================
 * SUITE 2 - uart_crc16()
 * ========================================================================= */

/*
 * Test case: 2.1
 * uart_crc16() must match the reference CRC for a DATA_PUSH payload.
 */
void TestCalculateCrcForKnownProtocolData(void)
{
    uint8_t data[] = {0x02, DEV_ID_ESP32, CMD_DATA_PUSH, 0x10, 0x20};
    uint16_t expected = CalculateReferenceCrc16Modbus(data, sizeof(data));
    uint16_t actual = uart_crc16(data, sizeof(data));

    TEST_ASSERT_EQUAL_HEX16(expected, actual);
}

/*
 * Test case: 2.2
 * uart_crc16() must change when one input bit changes.
 */
void TestChangeCrcWhenSingleBitFlips(void)
{
    uint8_t original[] = {0x02, DEV_ID_ESP32, CMD_DATA_PUSH, 0x10, 0x20};
    uint8_t flipped[] = {0x02, DEV_ID_ESP32, CMD_DATA_PUSH, 0x10, 0x21};

    uint16_t crcOriginal = uart_crc16(original, sizeof(original));
    uint16_t crcFlipped = uart_crc16(flipped, sizeof(flipped));

    TEST_ASSERT_NOT_EQUAL(crcOriginal, crcFlipped);
}

/*
 * Test case: 2.3
 * uart_crc16() must handle the protocol bytes of a zero-payload frame.
 */
void TestCalculateCrcForEmptyPayloadFrame(void)
{
    uint8_t data[] = {0x00, DEV_ID_STM32, CMD_HEARTBEAT_REQ};
    uint16_t expected = CalculateReferenceCrc16Modbus(data, sizeof(data));
    uint16_t actual = uart_crc16(data, sizeof(data));

    TEST_ASSERT_EQUAL_HEX16(expected, actual);
}

/*
 * Test case: 2.4
 * uart_crc16() must return the same value for the same data.
 */
void TestReturnSameCrcForSameData(void)
{
    uint8_t data[] = {0x05, DEV_ID_ESP32, CMD_DATA_PUSH, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE};

    uint16_t crc1 = uart_crc16(data, sizeof(data));
    uint16_t crc2 = uart_crc16(data, sizeof(data));

    TEST_ASSERT_EQUAL_HEX16(crc1, crc2);
}

/*
 * Test case: 2.5
 * uart_crc16() must match the standard MODBUS check value for "123456789".
 */
void TestCalculateStandardModbusCheckValue(void)
{
    const uint8_t data[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

    TEST_ASSERT_EQUAL_HEX16(0x4B37, uart_crc16(data, sizeof(data)));
}

/* =========================================================================
 * SUITE 3 - uart_parse_frame()
 * ========================================================================= */

/*
 * Test case: 3.1
 * uart_parse_frame() must decode a structurally valid DATA_PUSH frame.
 */
void TestParseValidFrame(void)
{
    uint8_t payload[] = {0x10, 0x20};
    uint8_t raw[UART_MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(DEV_ID_ESP32, CMD_DATA_PUSH, payload, 2, raw);

    UartFrame_t frame;
    ParseResult_t result = uart_parse_frame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSE_OK, result);
    TEST_ASSERT_EQUAL_HEX8(DEV_ID_ESP32, frame.dev_id);
    TEST_ASSERT_EQUAL_HEX8(CMD_DATA_PUSH, frame.cmd);
    TEST_ASSERT_EQUAL_UINT8(2, frame.len);
    TEST_ASSERT_EQUAL_HEX8(0x10, frame.payload[0]);
    TEST_ASSERT_EQUAL_HEX8(0x20, frame.payload[1]);
}

/*
 * Test case: 3.2
 * uart_parse_frame() must reject a frame with a wrong SOF byte.
 */
void TestRejectFrameWithWrongSof(void)
{
    uint8_t payload[] = {0x10};
    uint8_t raw[UART_MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(DEV_ID_ESP32, CMD_DATA_PUSH, payload, 1, raw);
    raw[0] = 0xBB;

    UartFrame_t frame;
    ParseResult_t result = uart_parse_frame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSE_ERR_SOF, result);
}

/*
 * Test case: 3.3
 * uart_parse_frame() must reject a frame with a wrong EOF byte.
 */
void TestRejectFrameWithWrongEof(void)
{
    uint8_t payload[] = {0x10};
    uint8_t raw[UART_MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(DEV_ID_ESP32, CMD_DATA_PUSH, payload, 1, raw);
    raw[rawLen - 1] = 0x44;

    UartFrame_t frame;
    ParseResult_t result = uart_parse_frame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSE_ERR_EOF, result);
}

/*
 * Test case: 3.4
 * uart_parse_frame() must reject a frame with a CRC mismatch.
 */
void TestRejectFrameWithCrcMismatch(void)
{
    uint8_t payload[] = {0x10, 0x20};
    uint8_t raw[UART_MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(DEV_ID_ESP32, CMD_DATA_PUSH, payload, 2, raw);

    raw[rawLen - 3] ^= 0xFF;

    UartFrame_t frame;
    ParseResult_t result = uart_parse_frame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSE_ERR_CRC, result);
}

/*
 * Test case: 3.5
 * uart_parse_frame() must return PARSE_INCOMPLETE when LEN claims more
 * payload bytes than the buffer contains.
 */
void TestReturnIncompleteWhenLenClaimsMoreBytesThanAvailable(void)
{
    uint8_t payload[] = {0x10, 0x20, 0x30};
    uint8_t raw[UART_MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(DEV_ID_ESP32, CMD_DATA_PUSH, payload, 3, raw);
    raw[1] = 10;

    UartFrame_t frame;
    ParseResult_t result = uart_parse_frame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSE_INCOMPLETE, result);
}

/*
 * Test case: 3.6
 * uart_parse_frame() must skip noise bytes before SOF and parse the first
 * valid frame.
 */
void TestSkipNoiseBeforeSof(void)
{
    uint8_t payload[] = {0xAB};
    uint8_t frameBuf[UART_MAX_FRAME_LEN];
    uint16_t frameLen = BuildValidFrame(DEV_ID_ESP32, CMD_DATA_PUSH, payload, 1, frameBuf);

    uint8_t stream[UART_MAX_FRAME_LEN + 3];
    stream[0] = 0x11;
    stream[1] = 0x22;
    stream[2] = 0x33;
    memcpy(&stream[3], frameBuf, frameLen);

    UartFrame_t frame;
    ParseResult_t result = uart_parse_frame(stream, frameLen + 3, &frame);

    TEST_ASSERT_EQUAL(PARSE_OK, result);
    TEST_ASSERT_EQUAL_HEX8(DEV_ID_ESP32, frame.dev_id);
    TEST_ASSERT_EQUAL_HEX8(CMD_DATA_PUSH, frame.cmd);
    TEST_ASSERT_EQUAL_UINT8(1, frame.len);
    TEST_ASSERT_EQUAL_HEX8(0xAB, frame.payload[0]);
}

/*
 * Test case: 3.7
 * uart_parse_frame() with a NULL output frame must not return PARSE_OK.
 */
void TestRejectNullOutputFrame(void)
{
    uint8_t raw[UART_MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(DEV_ID_ESP32, CMD_DATA_PUSH, NULL, 0, raw);

    ParseResult_t result = uart_parse_frame(raw, rawLen, NULL);

    TEST_ASSERT_NOT_EQUAL(PARSE_OK, result);
}

/*
 * Test case: 3.8
 * uart_parse_frame() with a NULL input buffer must not return PARSE_OK.
 */
void TestRejectNullInputBuffer(void)
{
    UartFrame_t frame;
    ParseResult_t result = uart_parse_frame(NULL, 0, &frame);

    TEST_ASSERT_NOT_EQUAL(PARSE_OK, result);
}

/*
 * Test case: 3.9
 * uart_parse_frame() must return PARSE_INCOMPLETE for a buffer shorter than
 * the minimum frame length.
 */
void TestReturnIncompleteForShortBuffer(void)
{
    uint8_t raw[] = {UART_SOF, 0x00, DEV_ID_ESP32};
    UartFrame_t frame;
    ParseResult_t result = uart_parse_frame(raw, sizeof(raw), &frame);

    TEST_ASSERT_EQUAL(PARSE_INCOMPLETE, result);
}

/*
 * Test case: 3.10
 * uart_parse_frame() must return PARSE_ERR_SOF when no SOF exists in the
 * stream.
 */
void TestReturnSofErrorWhenStreamHasNoSof(void)
{
    uint8_t raw[] = {0x11, 0x22, 0x33, 0x44};
    UartFrame_t frame;
    ParseResult_t result = uart_parse_frame(raw, sizeof(raw), &frame);

    TEST_ASSERT_EQUAL(PARSE_ERR_SOF, result);
}

/*
 * Test case: 3.11
 * uart_parse_frame() must reject a frame when CRC bytes are swapped.
 */
void TestRejectFrameWhenCrcBytesAreSwapped(void)
{
    uint8_t payload[] = {0x10, 0x20};
    uint8_t raw[UART_MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(DEV_ID_ESP32, CMD_DATA_PUSH, payload, 2, raw);

    uint8_t tmp = raw[rawLen - 3];
    raw[rawLen - 3] = raw[rawLen - 2];
    raw[rawLen - 2] = tmp;

    UartFrame_t frame;
    ParseResult_t result = uart_parse_frame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSE_ERR_CRC, result);
}

/* =========================================================================
 * SUITE 4 - Protocol command frame formats
 * ========================================================================= */

/*
 * Test case: 4.1
 * uart_build_frame() must encode an ACK frame as a 1-byte payload containing
 * the acknowledged command.
 */
void TestBuildAckFrameFormat(void)
{
    uint8_t ackPayload[] = {CMD_DATA_PUSH};
    uint8_t frame[UART_MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    UartStatus_t ret = uart_build_frame(DEV_ID_STM32, CMD_ACK, ackPayload, sizeof(ackPayload), frame, &frameLen);

    TEST_ASSERT_EQUAL(UART_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(8, frameLen);
    TEST_ASSERT_EQUAL_UINT8(1, frame[1]);
    TEST_ASSERT_EQUAL_HEX8(DEV_ID_STM32, frame[2]);
    TEST_ASSERT_EQUAL_HEX8(CMD_ACK, frame[3]);
    TEST_ASSERT_EQUAL_HEX8(CMD_DATA_PUSH, frame[4]);
}

/*
 * Test case: 4.2
 * uart_build_frame() must encode a NACK frame as [failed command, error].
 */
void TestBuildNackFrameFormat(void)
{
    uint8_t nackPayload[] = {CMD_DATA_PUSH, NACK_ERR_CRC_FAIL};
    uint8_t frame[UART_MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    UartStatus_t ret = uart_build_frame(DEV_ID_STM32, CMD_NACK, nackPayload, sizeof(nackPayload), frame, &frameLen);

    TEST_ASSERT_EQUAL(UART_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(9, frameLen);
    TEST_ASSERT_EQUAL_UINT8(2, frame[1]);
    TEST_ASSERT_EQUAL_HEX8(DEV_ID_STM32, frame[2]);
    TEST_ASSERT_EQUAL_HEX8(CMD_NACK, frame[3]);
    TEST_ASSERT_EQUAL_HEX8(CMD_DATA_PUSH, frame[4]);
    TEST_ASSERT_EQUAL_HEX8(NACK_ERR_CRC_FAIL, frame[5]);
}

/*
 * Test case: 4.3
 * uart_parse_frame() must decode a HEARTBEAT_REQ frame from ESP32.
 */
void TestParseHeartbeatRequestFromEsp32(void)
{
    uint8_t raw[UART_MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(DEV_ID_ESP32, CMD_HEARTBEAT_REQ, NULL, 0, raw);

    UartFrame_t frame;
    ParseResult_t result = uart_parse_frame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSE_OK, result);
    TEST_ASSERT_EQUAL_HEX8(DEV_ID_ESP32, frame.dev_id);
    TEST_ASSERT_EQUAL_HEX8(CMD_HEARTBEAT_REQ, frame.cmd);
    TEST_ASSERT_EQUAL_UINT8(0, frame.len);
}

/*
 * Test case: 4.4
 * uart_build_frame() must encode a zero-payload HEARTBEAT_RSP frame from
 * STM32.
 */
void TestBuildHeartbeatResponseFromStm32(void)
{
    uint8_t frame[UART_MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    UartStatus_t ret = uart_build_frame(DEV_ID_STM32, CMD_HEARTBEAT_RSP, NULL, 0, frame, &frameLen);

    TEST_ASSERT_EQUAL(UART_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(7, frameLen);
    TEST_ASSERT_EQUAL_UINT8(0, frame[1]);
    TEST_ASSERT_EQUAL_HEX8(DEV_ID_STM32, frame[2]);
    TEST_ASSERT_EQUAL_HEX8(CMD_HEARTBEAT_RSP, frame[3]);
}

/* =========================================================================
 * SUITE 5 - Stream parsing without FreeRTOS
 * ========================================================================= */

/*
 * Test case: 5.1
 * Consecutive frames in one byte stream must be parseable by advancing the
 * caller-owned offset.
 */
void TestParseThreeConsecutiveFramesByOffset(void)
{
    uint8_t payload1[] = {0x01};
    uint8_t payload2[] = {0x02, 0x03};
    uint8_t payload3[] = {0x04, 0x05, 0x06};

    uint8_t frame1[UART_MAX_FRAME_LEN];
    uint8_t frame2[UART_MAX_FRAME_LEN];
    uint8_t frame3[UART_MAX_FRAME_LEN];

    uint16_t len1 = BuildValidFrame(DEV_ID_ESP32, CMD_DATA_PUSH, payload1, sizeof(payload1), frame1);
    uint16_t len2 = BuildValidFrame(DEV_ID_ESP32, CMD_DATA_PUSH, payload2, sizeof(payload2), frame2);
    uint16_t len3 = BuildValidFrame(DEV_ID_ESP32, CMD_DATA_PUSH, payload3, sizeof(payload3), frame3);

    uint8_t stream[UART_MAX_FRAME_LEN * 3];
    uint16_t offset = 0;

    memcpy(&stream[offset], frame1, len1);
    offset += len1;
    memcpy(&stream[offset], frame2, len2);
    offset += len2;
    memcpy(&stream[offset], frame3, len3);
    offset += len3;

    UartFrame_t parsed;

    TEST_ASSERT_EQUAL(PARSE_OK, uart_parse_frame(stream, offset, &parsed));
    TEST_ASSERT_EQUAL_UINT8(1, parsed.len);
    TEST_ASSERT_EQUAL_HEX8(0x01, parsed.payload[0]);

    TEST_ASSERT_EQUAL(PARSE_OK, uart_parse_frame(&stream[len1], offset - len1, &parsed));
    TEST_ASSERT_EQUAL_UINT8(2, parsed.len);
    TEST_ASSERT_EQUAL_HEX8(0x02, parsed.payload[0]);
    TEST_ASSERT_EQUAL_HEX8(0x03, parsed.payload[1]);

    TEST_ASSERT_EQUAL(PARSE_OK, uart_parse_frame(&stream[len1 + len2], len3, &parsed));
    TEST_ASSERT_EQUAL_UINT8(3, parsed.len);
    TEST_ASSERT_EQUAL_HEX8(0x04, parsed.payload[0]);
    TEST_ASSERT_EQUAL_HEX8(0x05, parsed.payload[1]);
    TEST_ASSERT_EQUAL_HEX8(0x06, parsed.payload[2]);
}
