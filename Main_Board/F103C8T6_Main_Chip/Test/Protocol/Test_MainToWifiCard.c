/*
 * Test_MainToWifiCard.c
 *
 * Unit tests for the core UART protocol layer – STM32 <-> ESP32.
 * Frame: [SOF][LEN][CMD][PAYLOAD 0-250B][CRC_H][CRC_L][EOF] (6 bytes overhead)
 * No DEV_ID – UART1 is permanently wired to ESP32.
 * Updated: May 08, 2026
 *
 */

#include "unity.h"
#include "UartProtocol.h"

#include <stdint.h>
#include <string.h>

/* ── Fixtures ────────────────────────────────────────────────────────────── */

/*
 * Reference CRC16-MODBUS implementation.
 * Used only to generate golden values; not the code under test.
 */
static uint16_t CalculateReferenceCrc16Modbus(const uint8_t *data, uint16_t len)
{
    uint16_t crc = 0xFFFFu;

    for (uint16_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i];
        for (uint8_t bit = 0; bit < 8u; bit++) {
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
 * Build a valid frame and assert the builder succeeds.
 * Parser tests rely on a known-good input, so any builder failure
 * must terminate the test immediately.
 */
static uint16_t BuildValidFrame(uint8_t cmd,
                                 const uint8_t *payload, uint8_t payloadLen,
                                 uint8_t *out)
{
    uint16_t outLen = 0;
    UartStatus_t ret = BuildUartFrame(cmd, payload, payloadLen, out, &outLen);
    TEST_ASSERT_EQUAL(UART_OK, ret);
    return outLen;
}

/* ── setUp / tearDown ────────────────────────────────────────────────────── */

void setUp(void)
{
}

void tearDown(void)
{
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 1 – BuildUartFrame()
 *
 * Frame layout (byte indices):
 *   [0]      SOF  = 0xAA
 *   [1]      LEN  = payload byte count
 *   [2]      CMD
 *   [3..N]   PAYLOAD
 *   [N+1]    CRC_H
 *   [N+2]    CRC_L
 *   [N+3]    EOF  = 0x55
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 1.1
 * BuildUartFrame() with a 2-byte payload must produce an 8-byte frame
 * with correct SOF, LEN, CMD, payload bytes, CRC, and EOF.
 *
 * Expected frame: [0xAA][0x02][CMD_DATA_PUSH][0x10][0x20][CRC_H][CRC_L][0x55]
 */
void TestBuildFrameWithPayload(void)
{
    uint8_t  payload[] = {0x10, 0x20};
    uint8_t  frame[UART_MAX_FRAME_LEN];
    uint16_t frameLen  = 0;

    UartStatus_t ret = BuildUartFrame(CMD_DATA_PUSH, payload, 2, frame, &frameLen);

    TEST_ASSERT_EQUAL(UART_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(8, frameLen);        /* 2 payload + 6 overhead */
    TEST_ASSERT_EQUAL_HEX8(UART_SOF,       frame[0]);
    TEST_ASSERT_EQUAL_UINT8(2,             frame[1]);   /* LEN */
    TEST_ASSERT_EQUAL_HEX8(CMD_DATA_PUSH,  frame[2]);
    TEST_ASSERT_EQUAL_HEX8(0x10,           frame[3]);
    TEST_ASSERT_EQUAL_HEX8(0x20,           frame[4]);

    uint8_t  crcInput[]  = {0x02, CMD_DATA_PUSH, 0x10, 0x20};
    uint16_t expectedCrc = CalculateReferenceCrc16Modbus(crcInput, sizeof(crcInput));
    uint16_t actualCrc   = ((uint16_t)frame[5] << 8) | frame[6];
    TEST_ASSERT_EQUAL_HEX16(expectedCrc, actualCrc);
    TEST_ASSERT_EQUAL_HEX8(UART_EOF, frame[frameLen - 1]);
}

/*
 * Test case: 1.2
 * BuildUartFrame() with len = 0 and payload = NULL must produce a
 * 6-byte frame with no payload bytes.
 *
 * Expected frame: [0xAA][0x00][CMD][CRC_H][CRC_L][0x55]
 */
void TestBuildFrameWithoutPayload(void)
{
    uint8_t  frame[UART_MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    UartStatus_t ret = BuildUartFrame(CMD_HEARTBEAT_REQ, NULL, 0, frame, &frameLen);

    TEST_ASSERT_EQUAL(UART_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(6, frameLen);        /* 0 payload + 6 overhead */
    TEST_ASSERT_EQUAL_HEX8(UART_SOF,          frame[0]);
    TEST_ASSERT_EQUAL_UINT8(0,                frame[1]);   /* LEN */
    TEST_ASSERT_EQUAL_HEX8(CMD_HEARTBEAT_REQ, frame[2]);
    TEST_ASSERT_EQUAL_HEX8(UART_EOF,          frame[frameLen - 1]);
}

/*
 * Test case: 1.3
 * BuildUartFrame() with UART_MAX_PAYLOAD_LEN bytes must fill exactly
 * UART_MAX_FRAME_LEN bytes.
 */
void TestBuildFrameWithMaxPayload(void)
{
    uint8_t  payload[UART_MAX_PAYLOAD_LEN];
    uint8_t  frame[UART_MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    memset(payload, 0xFF, sizeof(payload));

    UartStatus_t ret = BuildUartFrame(CMD_DATA_PUSH, payload, UART_MAX_PAYLOAD_LEN,
                                       frame, &frameLen);

    TEST_ASSERT_EQUAL(UART_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(UART_MAX_FRAME_LEN,  frameLen);
    TEST_ASSERT_EQUAL_UINT8(UART_MAX_PAYLOAD_LEN, frame[1]);   /* LEN */
    TEST_ASSERT_EQUAL_HEX8(UART_SOF,              frame[0]);
    TEST_ASSERT_EQUAL_HEX8(UART_EOF,              frame[frameLen - 1]);
}

/*
 * Test case: 1.4
 * BuildUartFrame() must reject payloads larger than UART_MAX_PAYLOAD_LEN,
 * reset outLen to 0, and leave the output buffer untouched.
 */
void TestRejectOversizedPayload(void)
{
    uint8_t  payload[UART_MAX_PAYLOAD_LEN + 1];
    uint8_t  frame[UART_MAX_FRAME_LEN + 8];
    uint16_t frameLen = 1234;

    memset(payload, 0xAB, sizeof(payload));
    memset(frame,   0xCC, sizeof(frame));

    UartStatus_t ret = BuildUartFrame(CMD_DATA_PUSH, payload, UART_MAX_PAYLOAD_LEN + 1,
                                       frame, &frameLen);

    TEST_ASSERT_EQUAL(UART_ERR_PAYLOAD_TOO_LARGE, ret);
    TEST_ASSERT_EQUAL_UINT16(0, frameLen);
    TEST_ASSERT_EQUAL_HEX8(0xCC, frame[0]);
    TEST_ASSERT_EQUAL_HEX8(0xCC, frame[sizeof(frame) - 1]);
}

/*
 * Test case: 1.5
 * BuildUartFrame() with a NULL output buffer must return UART_ERR_NULL_PTR.
 */
void TestRejectNullOutputBuffer(void)
{
    uint8_t  payload[] = {0x01};
    uint16_t frameLen  = 0;

    UartStatus_t ret = BuildUartFrame(CMD_DATA_PUSH, payload, 1, NULL, &frameLen);

    TEST_ASSERT_EQUAL(UART_ERR_NULL_PTR, ret);
}

/*
 * Test case: 1.6
 * BuildUartFrame() with a NULL outLen pointer must return UART_ERR_NULL_PTR.
 */
void TestRejectNullOutputLength(void)
{
    uint8_t payload[] = {0x01};
    uint8_t frame[UART_MAX_FRAME_LEN];

    UartStatus_t ret = BuildUartFrame(CMD_DATA_PUSH, payload, 1, frame, NULL);

    TEST_ASSERT_EQUAL(UART_ERR_NULL_PTR, ret);
}

/*
 * Test case: 1.7
 * BuildUartFrame() with payload = NULL and len > 0 must return
 * UART_ERR_NULL_PTR and reset outLen to 0.
 */
void TestRejectNullPayloadWhenLengthIsNonzero(void)
{
    uint8_t  frame[UART_MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    UartStatus_t ret = BuildUartFrame(CMD_DATA_PUSH, NULL, 1, frame, &frameLen);

    TEST_ASSERT_EQUAL(UART_ERR_NULL_PTR, ret);
    TEST_ASSERT_EQUAL_UINT16(0, frameLen);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 2 – ComputeUartCrc16()
 *
 * CRC is computed over [LEN][CMD][PAYLOAD] – no DEV_ID.
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 2.1
 * ComputeUartCrc16() must match the reference for a DATA_PUSH frame header.
 * Input bytes: [LEN=2][CMD_DATA_PUSH][0x10][0x20]
 */
void TestCalculateCrcForKnownProtocolData(void)
{
    uint8_t  data[]   = {0x02, CMD_DATA_PUSH, 0x10, 0x20};
    uint16_t expected = CalculateReferenceCrc16Modbus(data, sizeof(data));
    uint16_t actual   = ComputeUartCrc16(data, sizeof(data));

    TEST_ASSERT_EQUAL_HEX16(expected, actual);
}

/*
 * Test case: 2.2
 * ComputeUartCrc16() must return a different value when one input bit changes.
 */
void TestChangeCrcWhenSingleBitFlips(void)
{
    uint8_t original[] = {0x02, CMD_DATA_PUSH, 0x10, 0x20};
    uint8_t flipped[]  = {0x02, CMD_DATA_PUSH, 0x10, 0x21};

    uint16_t crcOriginal = ComputeUartCrc16(original, sizeof(original));
    uint16_t crcFlipped  = ComputeUartCrc16(flipped,  sizeof(flipped));

    TEST_ASSERT_NOT_EQUAL(crcOriginal, crcFlipped);
}

/*
 * Test case: 2.3
 * ComputeUartCrc16() must handle the protocol bytes of a zero-payload frame.
 * Input bytes: [LEN=0][CMD_HEARTBEAT_REQ]
 */
void TestCalculateCrcForEmptyPayloadFrame(void)
{
    uint8_t  data[]   = {0x00, CMD_HEARTBEAT_REQ};
    uint16_t expected = CalculateReferenceCrc16Modbus(data, sizeof(data));
    uint16_t actual   = ComputeUartCrc16(data, sizeof(data));

    TEST_ASSERT_EQUAL_HEX16(expected, actual);
}

/*
 * Test case: 2.4
 * ComputeUartCrc16() must return the same value for the same input data.
 */
void TestReturnSameCrcForSameData(void)
{
    uint8_t data[] = {0x05, CMD_DATA_PUSH, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE};

    uint16_t crc1 = ComputeUartCrc16(data, sizeof(data));
    uint16_t crc2 = ComputeUartCrc16(data, sizeof(data));

    TEST_ASSERT_EQUAL_HEX16(crc1, crc2);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 3 – ParseUartFrame()
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 3.1
 * ParseUartFrame() must decode a valid frame and return PARSE_OK.
 */
void TestParseValidFrame(void)
{
    uint8_t  payload[] = {0x10, 0x20};
    uint8_t  raw[UART_MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_DATA_PUSH, payload, 2, raw);

    UartFrame_t   frame;
    ParseResult_t result = ParseUartFrame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSE_OK, result);
    TEST_ASSERT_EQUAL_HEX8(CMD_DATA_PUSH, frame.cmd);
    TEST_ASSERT_EQUAL_UINT8(2,            frame.len);
    TEST_ASSERT_EQUAL_HEX8(0x10,          frame.payload[0]);
    TEST_ASSERT_EQUAL_HEX8(0x20,          frame.payload[1]);
}

/*
 * Test case: 3.2
 * ParseUartFrame() must return PARSE_ERR_SOF when the first byte is not 0xAA.
 */
void TestReturnSofErrorWhenSofIsWrong(void)
{
    uint8_t  payload[] = {0x10};
    uint8_t  raw[UART_MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_DATA_PUSH, payload, 1, raw);
    raw[0] = 0xBB;

    UartFrame_t   frame;
    ParseResult_t result = ParseUartFrame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSE_ERR_SOF, result);
}

/*
 * Test case: 3.3
 * ParseUartFrame() must return PARSE_ERR_EOF when the last byte is not 0x55.
 */
void TestReturnEofErrorWhenEofIsWrong(void)
{
    uint8_t  payload[] = {0x10};
    uint8_t  raw[UART_MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_DATA_PUSH, payload, 1, raw);
    raw[rawLen - 1] = 0x44;

    UartFrame_t   frame;
    ParseResult_t result = ParseUartFrame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSE_ERR_EOF, result);
}

/*
 * Test case: 3.4
 * ParseUartFrame() must return PARSE_ERR_CRC when CRC_H is corrupted.
 */
void TestReturnCrcErrorWhenCrcIsWrong(void)
{
    uint8_t  payload[] = {0x10, 0x20};
    uint8_t  raw[UART_MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_DATA_PUSH, payload, 2, raw);
    raw[rawLen - 3] ^= 0xFF;   /* corrupt CRC_H */

    UartFrame_t   frame;
    ParseResult_t result = ParseUartFrame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSE_ERR_CRC, result);
}

/*
 * Test case: 3.5
 * ParseUartFrame() must return PARSE_INCOMPLETE when LEN claims more bytes
 * than the buffer contains.
 */
void TestReturnIncompleteWhenLenClaimsMoreBytesThanAvailable(void)
{
    uint8_t  payload[] = {0x10, 0x20, 0x30};
    uint8_t  raw[UART_MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_DATA_PUSH, payload, 3, raw);
    raw[1] = 10;   /* lie about payload length */

    UartFrame_t   frame;
    ParseResult_t result = ParseUartFrame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSE_INCOMPLETE, result);
}

/*
 * Test case: 3.6
 * ParseUartFrame() must skip noise bytes before SOF and parse the frame.
 */
void TestSkipNoiseBeforeSof(void)
{
    uint8_t  payload[] = {0xAB};
    uint8_t  frameBuf[UART_MAX_FRAME_LEN];
    uint16_t frameLen = BuildValidFrame(CMD_DATA_PUSH, payload, 1, frameBuf);

    uint8_t stream[UART_MAX_FRAME_LEN + 3];
    stream[0] = 0x11;
    stream[1] = 0x22;
    stream[2] = 0x33;
    memcpy(&stream[3], frameBuf, frameLen);

    UartFrame_t   frame;
    ParseResult_t result = ParseUartFrame(stream, frameLen + 3, &frame);

    TEST_ASSERT_EQUAL(PARSE_OK,       result);
    TEST_ASSERT_EQUAL_HEX8(CMD_DATA_PUSH, frame.cmd);
    TEST_ASSERT_EQUAL_UINT8(1,            frame.len);
    TEST_ASSERT_EQUAL_HEX8(0xAB,          frame.payload[0]);
}

/*
 * Test case: 3.7
 * ParseUartFrame() with a NULL output frame must return PARSE_ERR_NULL_PTR.
 */
void TestRejectNullOutputFrame(void)
{
    uint8_t  raw[UART_MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_DATA_PUSH, NULL, 0, raw);

    ParseResult_t result = ParseUartFrame(raw, rawLen, NULL);

    TEST_ASSERT_EQUAL(PARSE_ERR_NULL_PTR, result);
}

/*
 * Test case: 3.8
 * ParseUartFrame() with a NULL input buffer must return PARSE_ERR_NULL_PTR.
 */
void TestRejectNullInputBuffer(void)
{
    UartFrame_t   frame;
    ParseResult_t result = ParseUartFrame(NULL, 0, &frame);

    TEST_ASSERT_EQUAL(PARSE_ERR_NULL_PTR, result);
}

/*
 * Test case: 3.9
 * ParseUartFrame() must return PARSE_INCOMPLETE when the buffer is shorter
 * than the minimum frame length (6 bytes).
 */
void TestReturnIncompleteForShortBuffer(void)
{
    uint8_t raw[] = {UART_SOF, 0x00, CMD_DATA_PUSH};

    UartFrame_t   frame;
    ParseResult_t result = ParseUartFrame(raw, sizeof(raw), &frame);

    TEST_ASSERT_EQUAL(PARSE_INCOMPLETE, result);
}

/*
 * Test case: 3.10
 * ParseUartFrame() must return PARSE_ERR_SOF when the stream has no 0xAA byte.
 */
void TestReturnSofErrorWhenStreamHasNoSof(void)
{
    uint8_t raw[] = {0x11, 0x22, 0x33, 0x44};

    UartFrame_t   frame;
    ParseResult_t result = ParseUartFrame(raw, sizeof(raw), &frame);

    TEST_ASSERT_EQUAL(PARSE_ERR_SOF, result);
}

/*
 * Test case: 3.11
 * ParseUartFrame() must return PARSE_ERR_CRC when the two CRC bytes are
 * swapped.
 */
void TestRejectFrameWhenCrcBytesAreSwapped(void)
{
    uint8_t  payload[] = {0x10, 0x20};
    uint8_t  raw[UART_MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_DATA_PUSH, payload, 2, raw);

    uint8_t tmp      = raw[rawLen - 3];
    raw[rawLen - 3]  = raw[rawLen - 2];
    raw[rawLen - 2]  = tmp;

    UartFrame_t   frame;
    ParseResult_t result = ParseUartFrame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSE_ERR_CRC, result);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 4 – Protocol command frame formats
 *
 * Verifies that each command's payload layout matches the protocol spec.
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 4.1
 * An ACK frame must carry exactly 1 payload byte: the acknowledged command.
 * Frame: [0xAA][0x01][CMD_ACK][acked_cmd][CRC_H][CRC_L][0x55] = 7 bytes
 */
void TestBuildAckFrameFormat(void)
{
    uint8_t  ackPayload[] = {CMD_DATA_PUSH};
    uint8_t  frame[UART_MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    UartStatus_t ret = BuildUartFrame(CMD_ACK, ackPayload, sizeof(ackPayload),
                                       frame, &frameLen);

    TEST_ASSERT_EQUAL(UART_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(7, frameLen);       /* 1 payload + 6 overhead */
    TEST_ASSERT_EQUAL_UINT8(1,            frame[1]);   /* LEN */
    TEST_ASSERT_EQUAL_HEX8(CMD_ACK,      frame[2]);
    TEST_ASSERT_EQUAL_HEX8(CMD_DATA_PUSH, frame[3]);  /* acked command */
}

/*
 * Test case: 4.2
 * A NACK frame must carry exactly 2 payload bytes: [failed_cmd][error_code].
 * Frame: [0xAA][0x02][CMD_NACK][failed_cmd][error][CRC_H][CRC_L][0x55] = 8 bytes
 */
void TestBuildNackFrameFormat(void)
{
    uint8_t  nackPayload[] = {CMD_DATA_PUSH, NACK_ERR_CRC_FAIL};
    uint8_t  frame[UART_MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    UartStatus_t ret = BuildUartFrame(CMD_NACK, nackPayload, sizeof(nackPayload),
                                       frame, &frameLen);

    TEST_ASSERT_EQUAL(UART_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(8, frameLen);            /* 2 payload + 6 overhead */
    TEST_ASSERT_EQUAL_UINT8(2,                 frame[1]);   /* LEN */
    TEST_ASSERT_EQUAL_HEX8(CMD_NACK,           frame[2]);
    TEST_ASSERT_EQUAL_HEX8(CMD_DATA_PUSH,      frame[3]);   /* failed command */
    TEST_ASSERT_EQUAL_HEX8(NACK_ERR_CRC_FAIL,  frame[4]);   /* error code */
}

/*
 * Test case: 4.3
 * ParseUartFrame() must decode a HEARTBEAT_REQ frame with no payload.
 */
void TestParseHeartbeatRequest(void)
{
    uint8_t  raw[UART_MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_HEARTBEAT_REQ, NULL, 0, raw);

    UartFrame_t   frame;
    ParseResult_t result = ParseUartFrame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSE_OK,            result);
    TEST_ASSERT_EQUAL_HEX8(CMD_HEARTBEAT_REQ, frame.cmd);
    TEST_ASSERT_EQUAL_UINT8(0,                frame.len);
}

/*
 * Test case: 4.4
 * BuildUartFrame() must produce a 6-byte HEARTBEAT_RSP frame with no payload.
 */
void TestBuildHeartbeatResponse(void)
{
    uint8_t  frame[UART_MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    UartStatus_t ret = BuildUartFrame(CMD_HEARTBEAT_RSP, NULL, 0, frame, &frameLen);

    TEST_ASSERT_EQUAL(UART_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(6, frameLen);            /* 0 payload + 6 overhead */
    TEST_ASSERT_EQUAL_UINT8(0,                frame[1]);   /* LEN */
    TEST_ASSERT_EQUAL_HEX8(CMD_HEARTBEAT_RSP, frame[2]);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 5 – Stream parsing
 *
 * ParseUartFrame() operates on a caller-owned buffer slice.
 * The caller advances its offset after each successful parse.
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 5.1
 * Three consecutive frames in one byte stream must each be parseable by
 * advancing the offset after each call.
 */
void TestParseThreeConsecutiveFramesByOffset(void)
{
    uint8_t payload1[] = {0x01};
    uint8_t payload2[] = {0x02, 0x03};
    uint8_t payload3[] = {0x04, 0x05, 0x06};

    uint8_t  frame1[UART_MAX_FRAME_LEN];
    uint8_t  frame2[UART_MAX_FRAME_LEN];
    uint8_t  frame3[UART_MAX_FRAME_LEN];

    uint16_t len1 = BuildValidFrame(CMD_DATA_PUSH, payload1, sizeof(payload1), frame1);
    uint16_t len2 = BuildValidFrame(CMD_DATA_PUSH, payload2, sizeof(payload2), frame2);
    uint16_t len3 = BuildValidFrame(CMD_DATA_PUSH, payload3, sizeof(payload3), frame3);

    uint8_t  stream[UART_MAX_FRAME_LEN * 3];
    uint16_t offset = 0;

    memcpy(&stream[offset], frame1, len1); offset += len1;
    memcpy(&stream[offset], frame2, len2); offset += len2;
    memcpy(&stream[offset], frame3, len3); offset += len3;

    UartFrame_t parsed;

    /* Frame 1 */
    TEST_ASSERT_EQUAL(PARSE_OK, ParseUartFrame(stream, offset, &parsed));
    TEST_ASSERT_EQUAL_UINT8(1,    parsed.len);
    TEST_ASSERT_EQUAL_HEX8(0x01, parsed.payload[0]);

    /* Frame 2 */
    TEST_ASSERT_EQUAL(PARSE_OK, ParseUartFrame(&stream[len1], offset - len1, &parsed));
    TEST_ASSERT_EQUAL_UINT8(2,    parsed.len);
    TEST_ASSERT_EQUAL_HEX8(0x02, parsed.payload[0]);
    TEST_ASSERT_EQUAL_HEX8(0x03, parsed.payload[1]);

    /* Frame 3 */
    TEST_ASSERT_EQUAL(PARSE_OK, ParseUartFrame(&stream[len1 + len2], len3, &parsed));
    TEST_ASSERT_EQUAL_UINT8(3,    parsed.len);
    TEST_ASSERT_EQUAL_HEX8(0x04, parsed.payload[0]);
    TEST_ASSERT_EQUAL_HEX8(0x05, parsed.payload[1]);
    TEST_ASSERT_EQUAL_HEX8(0x06, parsed.payload[2]);
}

/*
 * Test case: 5.2
 * A frame with a 0x00 payload byte must be parsed correctly.
 * 0x00 is a valid data byte (e.g. CMD_ACK value, integer zero in a PLC
 * register) and must not be misinterpreted as "no data".
 */
void TestParseFrameContainingZeroByte(void)
{
    uint8_t  payload[] = {0x00, 0x01, 0x00};
    uint8_t  raw[UART_MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_DATA_PUSH, payload, 3, raw);

    UartFrame_t   frame;
    ParseResult_t result = ParseUartFrame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSE_OK,   result);
    TEST_ASSERT_EQUAL_UINT8(3,    frame.len);
    TEST_ASSERT_EQUAL_HEX8(0x00, frame.payload[0]);
    TEST_ASSERT_EQUAL_HEX8(0x01, frame.payload[1]);
    TEST_ASSERT_EQUAL_HEX8(0x00, frame.payload[2]);
}