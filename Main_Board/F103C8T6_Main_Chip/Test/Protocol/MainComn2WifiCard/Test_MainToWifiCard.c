/*
 * Test_MainToWifiCard.c
 *
 * Unit tests for the core UART protocol layer – STM32 <-> ESP32.
 * Frame: [SOF_D][mLength][mCommand][mDataPayload 0-250B][CRC_H][CRC_L][EOF] (6 bytes overhead)
 * No DEV_ID – UART1 is permanently wired to ESP32.
 * Updated: May 08, 2026
 *
 */

#include "unity.h"
#include "MainComn2WifiCard.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* ── Fixtures ────────────────────────────────────────────────────────────── */

/*
 * Reference CRC16-MODBUS implementation.
 * Used only to generate golden values; not the code under test.
 */
static uint16_t CalculateReferenceCrc16Modbus(const uint8_t *data, uint16_t mLength)
{
    uint16_t crc = 0xFFFFu;

    for (uint16_t i = 0; i < mLength; i++) {
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
static uint16_t BuildValidFrame(uint8_t mCommand,
                                 const uint8_t *mDataPayload, uint8_t payloadLen,
                                 uint8_t *out)
{
    uint16_t outLen = 0;
    FrameBuildStatus_t ret = BuildFrame(mCommand, mDataPayload, payloadLen, out, &outLen);
    TEST_ASSERT_EQUAL(BUILT_FRAME_OK, ret);
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
 * SUITE 1 – BuildFrame()
 *
 * Frame layout (byte indices):
 *   [0]      SOF_D  = 0xAA
 *   [1]      mLength  = mDataPayload byte count
 *   [2]      mCommand
 *   [3..N]   mDataPayload
 *   [N+1]    CRC_H
 *   [N+2]    CRC_L
 *   [N+3]    EOF  = 0x55
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 1.1
 * BuildFrame() with a 2-byte mDataPayload must produce an 8-byte frame
 * with correct SOF_D, mLength, mCommand, mDataPayload bytes, CRC, and EOF.
 *
 * Expected frame: [0xAA][0x02][CMD_DATA_PUSH][0x10][0x20][CRC_H][CRC_L][0x55]
 */
void TestBuildFrameWithPayload(void)
{
    uint8_t  mDataPayload[] = {0x10, 0x20};
    uint8_t  frame[MAX_FRAME_LEN];
    uint16_t frameLen  = 0;

    FrameBuildStatus_t ret = BuildFrame(CMD_DATA_PUSH, mDataPayload, 2, frame, &frameLen);

    TEST_ASSERT_EQUAL(BUILT_FRAME_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(8, frameLen);        /* 2 mDataPayload + 6 overhead */
    TEST_ASSERT_EQUAL_HEX8(SOF_D,       frame[0]);
    TEST_ASSERT_EQUAL_UINT8(2,             frame[1]);   /* mLength */
    TEST_ASSERT_EQUAL_HEX8(CMD_DATA_PUSH,  frame[2]);
    TEST_ASSERT_EQUAL_HEX8(0x10,           frame[3]);
    TEST_ASSERT_EQUAL_HEX8(0x20,           frame[4]);

    uint8_t  crcInput[]  = {0x02, CMD_DATA_PUSH, 0x10, 0x20};
    uint16_t expectedCrc = CalculateReferenceCrc16Modbus(crcInput, sizeof(crcInput));
    uint16_t actualCrc   = ((uint16_t)frame[5] << 8) | frame[6];
    TEST_ASSERT_EQUAL_HEX16(expectedCrc, actualCrc);
    TEST_ASSERT_EQUAL_HEX8(EOF_D, frame[frameLen - 1]);
}

/*
 * Test case: 1.2
 * BuildFrame() with mLength = 0 and mDataPayload = NULL must produce a
 * 6-byte frame with no mDataPayload bytes.
 *
 * Expected frame: [0xAA][0x00][mCommand][CRC_H][CRC_L][0x55]
 */
void TestBuildFrameWithoutPayload(void)
{
    uint8_t  frame[MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    FrameBuildStatus_t ret = BuildFrame(CMD_HEARTBEAT_REQ, NULL, 0, frame, &frameLen);

    TEST_ASSERT_EQUAL(BUILT_FRAME_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(6, frameLen);        /* 0 mDataPayload + 6 overhead */
    TEST_ASSERT_EQUAL_HEX8(SOF_D,          frame[0]);
    TEST_ASSERT_EQUAL_UINT8(0,                frame[1]);   /* mLength */
    TEST_ASSERT_EQUAL_HEX8(CMD_HEARTBEAT_REQ, frame[2]);
    TEST_ASSERT_EQUAL_HEX8(EOF_D,          frame[frameLen - 1]);
}

/*
 * Test case: 1.3
 * BuildFrame() with MAX_PAYLOAD_LEN bytes must fill exactly
 * MAX_FRAME_LEN bytes.
 */
void TestBuildFrameWithMaxPayload(void)
{
    uint8_t  mDataPayload[MAX_PAYLOAD_LEN];
    uint8_t  frame[MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    memset(mDataPayload, 0xFF, sizeof(mDataPayload));

    FrameBuildStatus_t ret = BuildFrame(CMD_DATA_PUSH, mDataPayload, MAX_PAYLOAD_LEN,
                                       frame, &frameLen);

    TEST_ASSERT_EQUAL(BUILT_FRAME_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(MAX_FRAME_LEN,  frameLen);
    TEST_ASSERT_EQUAL_UINT8(MAX_PAYLOAD_LEN, frame[1]);   /* mLength */
    TEST_ASSERT_EQUAL_HEX8(SOF_D,            frame[0]);
    TEST_ASSERT_EQUAL_HEX8(EOF_D,            frame[frameLen - 1]);
}

/*
 * Test case: 1.4
 * BuildFrame() must reject payloads larger than MAX_PAYLOAD_LEN,
 * reset outLen to 0, and leave the output buffer untouched.
 */
void TestRejectOversizedPayload(void)
{
    uint8_t  mDataPayload[MAX_PAYLOAD_LEN + 1];
    uint8_t  frame[MAX_FRAME_LEN + 8];
    uint16_t frameLen = 1234;

    memset(mDataPayload, 0xAB, sizeof(mDataPayload));
    memset(frame,   0xCC, sizeof(frame));

    FrameBuildStatus_t ret = BuildFrame(CMD_DATA_PUSH, mDataPayload, MAX_PAYLOAD_LEN + 1,
                                       frame, &frameLen);

    TEST_ASSERT_EQUAL(BUILT_FRAME_ERR_PAYLOAD_TOO_LARGE, ret);
    TEST_ASSERT_EQUAL_UINT16(0, frameLen);
    TEST_ASSERT_EQUAL_HEX8(0xCC, frame[0]);
    TEST_ASSERT_EQUAL_HEX8(0xCC, frame[sizeof(frame) - 1]);
}

/*
 * Test case: 1.5
 * BuildFrame() with a NULL output buffer must return BUILT_FRAME_ERR_NULL_PTR.
 */
void TestRejectNullOutputBuffer(void)
{
    uint8_t  mDataPayload[] = {0x01};
    uint16_t frameLen  = 0;

    FrameBuildStatus_t ret = BuildFrame(CMD_DATA_PUSH, mDataPayload, 1, NULL, &frameLen);

    TEST_ASSERT_EQUAL(BUILT_FRAME_ERR_NULL_PTR, ret);
}

/*
 * Test case: 1.6
 * BuildFrame() with a NULL outLen pointer must return BUILT_FRAME_ERR_NULL_PTR.
 */
void TestRejectNullOutputLength(void)
{
    uint8_t mDataPayload[] = {0x01};
    uint8_t frame[MAX_FRAME_LEN];

    FrameBuildStatus_t ret = BuildFrame(CMD_DATA_PUSH, mDataPayload, 1, frame, NULL);

    TEST_ASSERT_EQUAL(BUILT_FRAME_ERR_NULL_PTR, ret);
}

/*
 * Test case: 1.7
 * BuildFrame() with mDataPayload = NULL and mLength > 0 must return
 * BUILT_FRAME_ERR_NULL_PTR and reset outLen to 0.
 */
void TestRejectNullPayloadWhenLengthIsNonzero(void)
{
    uint8_t  frame[MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    FrameBuildStatus_t ret = BuildFrame(CMD_DATA_PUSH, NULL, 1, frame, &frameLen);

    TEST_ASSERT_EQUAL(BUILT_FRAME_ERR_NULL_PTR, ret);
    TEST_ASSERT_EQUAL_UINT16(0, frameLen);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 2 – ComputeUartCrc16()
 *
 * CRC is computed over [mLength][mCommand][mDataPayload]
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 2.1
 * The CRC embedded in a DATA_PUSH frame must match the reference CRC16-MODBUS
 * computed over [LEN][CMD][payload bytes].
 */
void TestCalculateCrcForKnownProtocolData(void)
{
    uint8_t  payload[]  = {0x10, 0x20};
    uint8_t  frame[MAX_FRAME_LEN];
    uint16_t frameLen   = 0;
 
    BuildFrame(CMD_DATA_PUSH, payload, 2, frame, &frameLen);
 
    /* CRC_H is at frameLen-3, CRC_L at frameLen-2 */
    uint16_t embeddedCrc = ((uint16_t)frame[frameLen - 3] << 8)
                           | frame[frameLen - 2];
 
    uint8_t  crcInput[]  = {0x02, CMD_DATA_PUSH, 0x10, 0x20};
    uint16_t expectedCrc = CalculateReferenceCrc16Modbus(crcInput, sizeof(crcInput));
 
    TEST_ASSERT_EQUAL_HEX16(expectedCrc, embeddedCrc);
}

/*
 * Test case: 2.2
 * The CRC embedded in a DATA_PUSH frame must return a different value when one input bit changes.
 */
void TestChangeCrcWhenSingleBitFlips(void)
{
    uint8_t  payloadA[] = {0x10, 0x20};
    uint8_t  payloadB[] = {0x10, 0x21};   /* one bit different */
    uint8_t  frameA[MAX_FRAME_LEN];
    uint8_t  frameB[MAX_FRAME_LEN];
    uint16_t lenA = 0, lenB = 0;
 
    BuildFrame(CMD_DATA_PUSH, payloadA, 2, frameA, &lenA);
    BuildFrame(CMD_DATA_PUSH, payloadB, 2, frameB, &lenB);
 
    uint16_t crcA = ((uint16_t)frameA[lenA - 3] << 8) | frameA[lenA - 2];
    uint16_t crcB = ((uint16_t)frameB[lenB - 3] << 8) | frameB[lenB - 2];
 
    TEST_ASSERT_NOT_EQUAL(crcA, crcB);
}

/*
 * Test case: 2.3
 * A zero-payload frame must embed a CRC matching the reference computed
 * over [LEN=0][CMD] only.
 */
void TestCalculateCrcForEmptyPayloadFrame(void)
{
    uint8_t  frame[MAX_FRAME_LEN];
    uint16_t frameLen = 0;
 
    BuildFrame(CMD_HEARTBEAT_REQ, NULL, 0, frame, &frameLen);
 
    uint16_t embeddedCrc = ((uint16_t)frame[frameLen - 3] << 8)
                           | frame[frameLen - 2];
 
    uint8_t  crcInput[]  = {0x00, CMD_HEARTBEAT_REQ};
    uint16_t expectedCrc = CalculateReferenceCrc16Modbus(crcInput, sizeof(crcInput));
 
    TEST_ASSERT_EQUAL_HEX16(expectedCrc, embeddedCrc);
}

/*
 * Test case: 2.4
 * Building the same frame twice must produce identical CRC bytes both times.
 */
void TestReturnSameCrcForSameData(void)
{
    uint8_t  payload[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE};
    uint8_t  frameA[MAX_FRAME_LEN];
    uint8_t  frameB[MAX_FRAME_LEN];
    uint16_t lenA = 0, lenB = 0;
 
    BuildFrame(CMD_DATA_PUSH, payload, sizeof(payload), frameA, &lenA);
    BuildFrame(CMD_DATA_PUSH, payload, sizeof(payload), frameB, &lenB);
 
    uint16_t crcA = ((uint16_t)frameA[lenA - 3] << 8) | frameA[lenA - 2];
    uint16_t crcB = ((uint16_t)frameB[lenB - 3] << 8) | frameB[lenB - 2];
 
    TEST_ASSERT_EQUAL_HEX16(crcA, crcB);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 3 – ParseFrame()
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 3.1
 * ParseFrame() must decode a valid frame and return PARSED_FRAME_OK.
 */
void TestParseValidFrame(void)
{
    uint8_t  mDataPayload[] = {0x10, 0x20};
    uint8_t  raw[MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_DATA_PUSH, mDataPayload, 2, raw);

    FrameStructure_t   frame;
    FrameParseStatus_t result = ParseFrame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSED_FRAME_OK, result);
    TEST_ASSERT_EQUAL_HEX8(CMD_DATA_PUSH, frame.mCommand);
    TEST_ASSERT_EQUAL_UINT8(2,            frame.mLength);
    TEST_ASSERT_EQUAL_HEX8(0x10,          frame.mDataPayload[0]);
    TEST_ASSERT_EQUAL_HEX8(0x20,          frame.mDataPayload[1]);
}

/*
 * Test case: 3.2
 * ParseFrame() must return PARSED_FRAME_ERR_SOF_D when the first byte is not 0xAA.
 */
void TestReturnSofErrorWhenSofIsWrong(void)
{
    uint8_t  payload[] = {0x10};
    uint8_t  raw[MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_DATA_PUSH, payload, 1, raw);

    /* Copy sang buffer mới, thay SOF → tránh compiler optimization */
    uint8_t  corrupted[MAX_FRAME_LEN];
    memcpy(corrupted, raw, rawLen);
    corrupted[0] = 0xBB;

    FrameStructure_t   frame;
    FrameParseStatus_t result = ParseFrame(corrupted, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSED_FRAME_ERR_SOF_D, result);
}

/*
 * Test case: 3.3
 * ParseFrame() must return PARSED_FRAME_ERR_EOF_D when the last byte is not 0x55.
 */
void TestReturnEofErrorWhenEofIsWrong(void)
{
    uint8_t  mDataPayload[] = {0x10};
    uint8_t  raw[MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_DATA_PUSH, mDataPayload, 1, raw);

    uint8_t  corrupted[MAX_FRAME_LEN];
    memcpy(corrupted, raw, rawLen);
    corrupted[rawLen - 1] = 0x44;

    FrameStructure_t   frame;
    FrameParseStatus_t result = ParseFrame(corrupted, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSED_FRAME_ERR_EOF_D, result);
}

/*
 * Test case: 3.4
 * ParseFrame() must return PARSED_FRAME_ERR_CRC when CRC_H is corrupted.
 */
void TestReturnCrcErrorWhenCrcIsWrong(void)
{
    uint8_t  mDataPayload[] = {0x10, 0x20};
    uint8_t  raw[MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_DATA_PUSH, mDataPayload, 2, raw);

    uint8_t  corrupted[MAX_FRAME_LEN];
    memcpy(corrupted, raw, rawLen);
    corrupted[rawLen - 3] ^= 0xFF;   /* corrupt CRC_H */

    FrameStructure_t   frame;
    FrameParseStatus_t result = ParseFrame(corrupted, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSED_FRAME_ERR_CRC, result);
}

/*
 * Test case: 3.5
 * ParseFrame() must return PARSED_FRAME_INCOMPLETE when mLength claims more bytes
 * than the buffer contains.
 */
void TestReturnIncompleteWhenLenClaimsMoreBytesThanAvailable(void)
{
    uint8_t  mDataPayload[] = {0x10, 0x20, 0x30};
    uint8_t  raw[MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_DATA_PUSH, mDataPayload, 3, raw);

    uint8_t  corrupted[MAX_FRAME_LEN];
    memcpy(corrupted, raw, rawLen);
    corrupted[1] = 10;   /* lie about mDataPayload length */

    FrameStructure_t   frame;
    FrameParseStatus_t result = ParseFrame(corrupted, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSED_FRAME_INCOMPLETE, result);
}

/*
 * Test case: 3.6
 * ParseFrame() must skip noise bytes before SOF_D and parse the frame.
 * Stream: [noise][noise][noise][SOF_D][mLength][mCommand][mDataPayload][CRC_H][CRC_L][EOF]
 */
void TestSkipNoiseBeforeSof(void)
{
    uint8_t  mDataPayload[] = {0xAB};
    uint8_t  frameBuf[MAX_FRAME_LEN];
    uint16_t frameLen = BuildValidFrame(CMD_DATA_PUSH, mDataPayload, 1, frameBuf);

    uint8_t stream[MAX_FRAME_LEN + 3];
    stream[0] = 0x11;
    stream[1] = 0x22;
    stream[2] = 0x33;
    memcpy(&stream[3], frameBuf, frameLen);

    FrameStructure_t   frame;
    FrameParseStatus_t result = ParseFrame(stream, frameLen + 3, &frame);

    TEST_ASSERT_EQUAL(PARSED_FRAME_OK,       result);
    TEST_ASSERT_EQUAL_HEX8(CMD_DATA_PUSH, frame.mCommand);
    TEST_ASSERT_EQUAL_UINT8(1,            frame.mLength);
    TEST_ASSERT_EQUAL_HEX8(0xAB,          frame.mDataPayload[0]);
}

/*
 * Test case: 3.7
 * ParseFrame() with a NULL output frame must return PARSED_FRAME_NULL_PTR.
 */
void TestRejectNullOutputFrame(void)
{
    uint8_t  raw[MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_DATA_PUSH, NULL, 0, raw);

    FrameParseStatus_t result = ParseFrame(raw, rawLen, NULL);

    TEST_ASSERT_EQUAL(PARSED_FRAME_NULL_PTR, result);
}

/*
 * Test case: 3.8
 * ParseFrame() with a NULL input buffer must return PARSED_FRAME_NULL_PTR.
 */
void TestRejectNullInputBuffer(void)
{
    FrameStructure_t   frame;
    FrameParseStatus_t result = ParseFrame(NULL, 0, &frame);

    TEST_ASSERT_EQUAL(PARSED_FRAME_NULL_PTR, result);
}

/*
 * Test case: 3.9
 * ParseFrame() must return PARSED_FRAME_INCOMPLETE when the buffer is shorter
 * than the minimum frame length (6 bytes).
 */
void TestReturnIncompleteForShortBuffer(void)
{
    uint8_t raw[] = {SOF_D, 0x00, CMD_DATA_PUSH};

    FrameStructure_t   frame;
    FrameParseStatus_t result = ParseFrame(raw, sizeof(raw), &frame);

    TEST_ASSERT_EQUAL(PARSED_FRAME_INCOMPLETE, result);
}

/*
 * Test case: 3.10
 * ParseFrame() must return PARSED_FRAME_ERR_SOF_D when the stream has no 0xAA byte.
 */
void TestReturnSofErrorWhenStreamHasNoSof(void)
{
    uint8_t raw[] = {0x11, 0x22, 0x33, 0x44};

    FrameStructure_t   frame;
    FrameParseStatus_t result = ParseFrame(raw, sizeof(raw), &frame);

    TEST_ASSERT_EQUAL(PARSED_FRAME_ERR_SOF_D, result);
}

/*
 * Test case: 3.11
 * ParseFrame() must return PARSED_FRAME_ERR_CRC when the two CRC bytes are
 * swapped.
 */
void TestRejectFrameWhenCrcBytesAreSwapped(void)
{
    uint8_t  mDataPayload[] = {0x10, 0x20};
    uint8_t  raw[MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_DATA_PUSH, mDataPayload, 2, raw);

    uint8_t  corrupted[MAX_FRAME_LEN];
    memcpy(corrupted, raw, rawLen);
    uint8_t  tmp            = corrupted[rawLen - 3];
    corrupted[rawLen - 3]   = corrupted[rawLen - 2];
    corrupted[rawLen - 2]   = tmp;

    FrameStructure_t   frame;
    FrameParseStatus_t result = ParseFrame(corrupted, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSED_FRAME_ERR_CRC, result);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 4 – Protocol command frame formats
 *
 * Verifies that each command's mDataPayload layout matches the protocol spec.
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 4.1
 * An ACK frame must carry exactly 1 mDataPayload byte: the acknowledged command.
 * Frame: [0xAA][0x01][CMD_ACK][acked_cmd][CRC_H][CRC_L][0x55] = 7 bytes
 */
void TestBuildAckFrameFormat(void)
{
    uint8_t  ackPayload[] = {CMD_DATA_PUSH};
    uint8_t  frame[MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    FrameBuildStatus_t ret = BuildFrame(CMD_ACK, ackPayload, sizeof(ackPayload),
                                       frame, &frameLen);

    TEST_ASSERT_EQUAL(BUILT_FRAME_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(7, frameLen);
    TEST_ASSERT_EQUAL_UINT8(1,             frame[1]);
    TEST_ASSERT_EQUAL_HEX8(CMD_ACK,       frame[2]);
    TEST_ASSERT_EQUAL_HEX8(CMD_DATA_PUSH, frame[3]);
}

/*
 * Test case: 4.2
 * A NACK frame must carry exactly 2 mDataPayload bytes: [failed_cmd][error_code].
 * Frame: [0xAA][0x02][CMD_NACK][failed_cmd][error][CRC_H][CRC_L][0x55] = 8 bytes
 */
void TestBuildNackFrameFormat(void)
{
    uint8_t  nackPayload[] = {CMD_DATA_PUSH, NACK_ERR_CRC_FAIL};
    uint8_t  frame[MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    FrameBuildStatus_t ret = BuildFrame(CMD_NACK, nackPayload, sizeof(nackPayload),
                                       frame, &frameLen);

    TEST_ASSERT_EQUAL(BUILT_FRAME_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(8, frameLen);
    TEST_ASSERT_EQUAL_UINT8(2,                frame[1]);
    TEST_ASSERT_EQUAL_HEX8(CMD_NACK,          frame[2]);
    TEST_ASSERT_EQUAL_HEX8(CMD_DATA_PUSH,     frame[3]);
    TEST_ASSERT_EQUAL_HEX8(NACK_ERR_CRC_FAIL, frame[4]);
}

/*
 * Test case: 4.3
 * ParseFrame() must decode a HEARTBEAT_REQ frame with no mDataPayload.
 */
void TestParseHeartbeatRequest(void)
{
    uint8_t  raw[MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_HEARTBEAT_REQ, NULL, 0, raw);

    FrameStructure_t   frame;
    FrameParseStatus_t result = ParseFrame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSED_FRAME_OK,         result);
    TEST_ASSERT_EQUAL_HEX8(CMD_HEARTBEAT_REQ,  frame.mCommand);
    TEST_ASSERT_EQUAL_UINT8(0,                 frame.mLength);
}

/*
 * Test case: 4.4
 * BuildFrame() must produce a 6-byte HEARTBEAT_RSP frame with no mDataPayload.
 */
void TestBuildHeartbeatResponse(void)
{
    uint8_t  frame[MAX_FRAME_LEN];
    uint16_t frameLen = 0;

    FrameBuildStatus_t ret = BuildFrame(CMD_HEARTBEAT_RSP, NULL, 0, frame, &frameLen);

    TEST_ASSERT_EQUAL(BUILT_FRAME_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(6, frameLen);
    TEST_ASSERT_EQUAL_UINT8(0,                 frame[1]);
    TEST_ASSERT_EQUAL_HEX8(CMD_HEARTBEAT_RSP,  frame[2]);
}

/*
 * Test case: 4.5
 * ParseFrame() must decode a HEARTBEAT_RSP frame with no mDataPayload.
 *
 * CheckParsedFrameIncomplete() whitelists only CMD_HEARTBEAT_REQ for 6-byte
 * frames.  CMD_HEARTBEAT_RSP is also a valid 6-byte frame but is not in the
 * whitelist, so parsing it returns PARSED_FRAME_INCOMPLETE incorrectly.
 * Fix: replace the command-based whitelist with a pure counter check:
 *   if (len < 6) → INCOMPLETE; if ((len - 6) != buffer[1]) → INCOMPLETE.
 */
void TestParseHeartbeatResponse(void)
{
    uint8_t  raw[MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_HEARTBEAT_RSP, NULL, 0, raw);

    FrameStructure_t   frame;
    FrameParseStatus_t result = ParseFrame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSED_FRAME_OK,        result);
    TEST_ASSERT_EQUAL_HEX8(CMD_HEARTBEAT_RSP, frame.mCommand);
    TEST_ASSERT_EQUAL_UINT8(0,                frame.mLength);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 5 – Stream parsing
 *
 * ParseFrame() operates on a caller-owned buffer slice.
 * The caller advances its offset after each successful parse.
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 5.1
 * The caller knows frame boundaries from the mLength field and passes exactly
 * one frame at a time to ParseFrame().  Three frames built into a stream must
 * each parse correctly when sliced to their exact length.
 *
 * Passing the entire remaining stream length fails because CheckParsedFrameIncomplete
 * compares (total_bytes - 6) against frame.mLength – these only match when
 * exactly one frame is in the buffer.
 */
void TestParseThreeConsecutiveFramesByOffset(void)
{
    uint8_t payload1[] = {0x01};
    uint8_t payload2[] = {0x02, 0x03};
    uint8_t payload3[] = {0x04, 0x05, 0x06};

    uint8_t  frame1[MAX_FRAME_LEN];
    uint8_t  frame2[MAX_FRAME_LEN];
    uint8_t  frame3[MAX_FRAME_LEN];

    uint16_t len1 = BuildValidFrame(CMD_DATA_PUSH, payload1, sizeof(payload1), frame1);
    uint16_t len2 = BuildValidFrame(CMD_DATA_PUSH, payload2, sizeof(payload2), frame2);
    uint16_t len3 = BuildValidFrame(CMD_DATA_PUSH, payload3, sizeof(payload3), frame3);

    uint8_t stream[MAX_FRAME_LEN * 3];
    uint16_t offset = 0;
    memcpy(&stream[offset], frame1, len1); offset += len1;
    memcpy(&stream[offset], frame2, len2); offset += len2;
    memcpy(&stream[offset], frame3, len3); offset += len3;

    FrameStructure_t parsed;

    /* Frame 1 – pass exactly len1 bytes */
    TEST_ASSERT_EQUAL(PARSED_FRAME_OK, ParseFrame(stream, len1, &parsed));
    TEST_ASSERT_EQUAL_UINT8(1,    parsed.mLength);
    TEST_ASSERT_EQUAL_HEX8(0x01, parsed.mDataPayload[0]);

    /* Frame 2 – pass exactly len2 bytes starting at offset len1 */
    TEST_ASSERT_EQUAL(PARSED_FRAME_OK, ParseFrame(&stream[len1], len2, &parsed));
    TEST_ASSERT_EQUAL_UINT8(2,    parsed.mLength);
    TEST_ASSERT_EQUAL_HEX8(0x02, parsed.mDataPayload[0]);
    TEST_ASSERT_EQUAL_HEX8(0x03, parsed.mDataPayload[1]);

    /* Frame 3 – pass exactly len3 bytes starting at offset len1+len2 */
    TEST_ASSERT_EQUAL(PARSED_FRAME_OK, ParseFrame(&stream[len1 + len2], len3, &parsed));
    TEST_ASSERT_EQUAL_UINT8(3,    parsed.mLength);
    TEST_ASSERT_EQUAL_HEX8(0x04, parsed.mDataPayload[0]);
    TEST_ASSERT_EQUAL_HEX8(0x05, parsed.mDataPayload[1]);
    TEST_ASSERT_EQUAL_HEX8(0x06, parsed.mDataPayload[2]);
}

/*
 * Test case: 5.2
 * A frame with a 0x00 mDataPayload byte must be parsed correctly.
 * 0x00 is a valid data byte (e.g. CMD_ACK value, integer zero in a PLC
 * register) and must not be misinterpreted as "no data".
 */
void TestParseFrameContainingZeroByte(void)
{
    uint8_t  mDataPayload[] = {0x00, 0x01, 0x00};
    uint8_t  raw[MAX_FRAME_LEN];
    uint16_t rawLen = BuildValidFrame(CMD_DATA_PUSH, mDataPayload, 3, raw);

    FrameStructure_t   frame;
    FrameParseStatus_t result = ParseFrame(raw, rawLen, &frame);

    TEST_ASSERT_EQUAL(PARSED_FRAME_OK,   result);
    TEST_ASSERT_EQUAL_UINT8(3,    frame.mLength);
    TEST_ASSERT_EQUAL_HEX8(0x00, frame.mDataPayload[0]);
    TEST_ASSERT_EQUAL_HEX8(0x01, frame.mDataPayload[1]);
    TEST_ASSERT_EQUAL_HEX8(0x00, frame.mDataPayload[2]);
}