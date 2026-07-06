/*
 * TestRunner_MainToWifiCard.c
 *
 * Unity test runner for the core UART protocol layer – STM32 <-> ESP32.
 * Updated: May 08, 2026
 *
 */

#include "unity.h"
#include <stdio.h>

/* ── SUITE 1 – BuildUartFrame() ──────────────────────────────────────────── */
extern void TestBuildFrameWithPayload(void);
extern void TestBuildFrameWithoutPayload(void);
extern void TestBuildFrameWithMaxPayload(void);
extern void TestRejectOversizedPayload(void);
extern void TestRejectNullOutputBuffer(void);
extern void TestRejectNullOutputLength(void);
extern void TestRejectNullPayloadWhenLengthIsNonzero(void);

/* ── SUITE 2 – ComputeUartCrc16() ───────────────────────────────────────── */
extern void TestCalculateCrcForKnownProtocolData(void);
extern void TestChangeCrcWhenSingleBitFlips(void);
extern void TestCalculateCrcForEmptyPayloadFrame(void);
extern void TestReturnSameCrcForSameData(void);

/* ── SUITE 3 – ParseUartFrame() ─────────────────────────────────────────── */
extern void TestParseValidFrame(void);
extern void TestParseFrameWithMaxPayload(void);
extern void TestReturnSofErrorWhenSofIsWrong(void);
extern void TestReturnEofErrorWhenEofIsWrong(void);
extern void TestReturnCrcErrorWhenCrcIsWrong(void);
extern void TestReturnIncompleteWhenLenClaimsMoreBytesThanAvailable(void);
extern void TestSkipNoiseBeforeSof(void);
extern void TestParseFrameStoresDecodedFrameForGetFrame(void);
extern void TestRejectNullInputBuffer(void);
extern void TestReturnIncompleteForShortBuffer(void);
extern void TestParseExactSixByteHeartbeatRequestAsComplete(void);
extern void TestReturnSofErrorWhenStreamHasNoSof(void);
extern void TestRejectFrameWhenCrcBytesAreSwapped(void);
extern void TestGetFrameCopiesStoredFrameToCallerBuffer(void);
extern void TestParseErrorDoesNotOverwriteStoredFrame(void);

/* ── SUITE 4 – Protocol command frame formats ────────────────────────────── */
extern void TestBuildAckFrameFormat(void);
extern void TestBuildNackFrameFormat(void);
extern void TestParseHeartbeatRequest(void);
extern void TestBuildHeartbeatResponse(void);
extern void TestParseAckFrameWithNoExtraPayload(void);
extern void TestParseNackFrameWithErrorCode(void);
extern void TestNackErrorCodeValuesAreStable(void);
extern void TestBuildNackFrameForEachErrorCode(void);
extern void TestParseNackFrameForEachErrorCode(void);

/* ── SUITE 5 – Stream parsing ────────────────────────────────────────────── */
extern void TestParseThreeConsecutiveFramesByOffset(void);
extern void TestParseFrameContainingZeroByte(void);
extern void TestSkipNoiseWithStraySofBeforeValidFrame(void);

void setUp(void);
void tearDown(void);

/* ─────────────────────────────────────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    printf("\n--- SUITE 1 - BuildUartFrame() ---\n");
    RUN_TEST(TestBuildFrameWithPayload);
    RUN_TEST(TestBuildFrameWithoutPayload);
    RUN_TEST(TestBuildFrameWithMaxPayload);
    RUN_TEST(TestRejectOversizedPayload);
    RUN_TEST(TestRejectNullOutputBuffer);
    RUN_TEST(TestRejectNullOutputLength);
    RUN_TEST(TestRejectNullPayloadWhenLengthIsNonzero);

    printf("\n--- SUITE 2 - ComputeUartCrc16() ---\n");
    RUN_TEST(TestCalculateCrcForKnownProtocolData);
    RUN_TEST(TestChangeCrcWhenSingleBitFlips);
    RUN_TEST(TestCalculateCrcForEmptyPayloadFrame);
    RUN_TEST(TestReturnSameCrcForSameData);

    printf("\n--- SUITE 3 - ParseUartFrame() ---\n");
    RUN_TEST(TestParseValidFrame);
    RUN_TEST(TestParseFrameWithMaxPayload);
    RUN_TEST(TestReturnSofErrorWhenSofIsWrong);
    RUN_TEST(TestReturnEofErrorWhenEofIsWrong);
    RUN_TEST(TestReturnCrcErrorWhenCrcIsWrong);
    RUN_TEST(TestReturnIncompleteWhenLenClaimsMoreBytesThanAvailable);
    RUN_TEST(TestSkipNoiseBeforeSof);
    RUN_TEST(TestParseFrameStoresDecodedFrameForGetFrame);
    RUN_TEST(TestRejectNullInputBuffer);
    RUN_TEST(TestReturnIncompleteForShortBuffer);
    RUN_TEST(TestParseExactSixByteHeartbeatRequestAsComplete);
    RUN_TEST(TestReturnSofErrorWhenStreamHasNoSof);
    RUN_TEST(TestRejectFrameWhenCrcBytesAreSwapped);
    RUN_TEST(TestGetFrameCopiesStoredFrameToCallerBuffer);
    RUN_TEST(TestParseErrorDoesNotOverwriteStoredFrame);

    printf("\n--- SUITE 4 - Protocol command frame formats ---\n");
    RUN_TEST(TestBuildAckFrameFormat);
    RUN_TEST(TestBuildNackFrameFormat);
    RUN_TEST(TestParseHeartbeatRequest);
    RUN_TEST(TestBuildHeartbeatResponse);
    RUN_TEST(TestParseAckFrameWithNoExtraPayload);
    RUN_TEST(TestParseNackFrameWithErrorCode);
    RUN_TEST(TestNackErrorCodeValuesAreStable);
    RUN_TEST(TestBuildNackFrameForEachErrorCode);
    RUN_TEST(TestParseNackFrameForEachErrorCode);

    printf("\n--- SUITE 5 - Stream parsing ---\n");
    RUN_TEST(TestParseThreeConsecutiveFramesByOffset);
    RUN_TEST(TestParseFrameContainingZeroByte);
    RUN_TEST(TestSkipNoiseWithStraySofBeforeValidFrame);

    return UNITY_END();
}
