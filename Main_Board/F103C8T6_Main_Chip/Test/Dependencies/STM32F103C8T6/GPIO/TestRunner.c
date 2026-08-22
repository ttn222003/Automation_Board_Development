/*
 * TestRunner.c
 *
 * Unity test runner for the GpioCommon BSP layer.
 * All test functions are declared extern and registered with RUN_TEST().
 */

#include "unity.h"
#include <stdio.h>

/* ── Suite 1: GpioCommon() ─────────────────────────────────────────── */
extern void TestBspGpioCommonReturnsSuccess(void);
extern void TestBspGpioCommonInitializesPinMapping(void);

/* ── Suite 2: BSP_GPIO_Init() ──────────────────────────────────────── */
extern void TestBspGpioInitReadsCurrentODR(void);
extern void TestBspGpioInitAllOutputsOff(void);
extern void TestBspGpioInitAllOutputsOn(void);

/* ── Suite 3: BSP_WriteOutputs() ──────────────────────────────────── */
extern void TestBspWriteOutputsAllZeros(void);
extern void TestBspWriteOutputsAllOnes(void);
extern void TestBspWriteOutputsAlternatingPattern01(void);
extern void TestBspWriteOutputsAlternatingPattern10(void);
extern void TestBspWriteOutputsSingleOutputOn(void);
extern void TestBspWriteOutputsIgnoresUpperBits(void);

/* ── Suite 4: Sequential writes ────────────────────────────────────── */
extern void TestBspSequentialWritesChangeOutputs(void);

/* ── Suite 5: BSP_FeedWatchdog() ──────────────────────────────────── */
extern void TestBspFeedWatchdogDoesNotCrash(void);

/* ── Suite 6: Integration ──────────────────────────────────────────── */
extern void TestBspTypicalPlcScanSequence(void);

/* ── Suite 7: Boundary conditions ──────────────────────────────────── */
extern void TestBspBoundaryMaximumOutputValue(void);
extern void TestBspBoundaryMinimumOutputValue(void);
extern void TestBspBoundaryRejectOutOfRangeOutputImage(void);

/* ── Suite 8 – State Transitions between Functions ──────────────────────────────────── */
extern void TestBspStateTransitionInitBeforeGpioCommon(void);
extern void TestBspStateTransitionWriteOutputsBeforeGpioCommon(void);
extern void TestBspStateTransitionNormalSequence(void);

/* ─────────────────────────────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    printf("\n--- Suite 1: BSP_GpioCommon() Initialisation ---\n");
    RUN_TEST(TestBspGpioCommonReturnsSuccess);
    RUN_TEST(TestBspGpioCommonInitializesPinMapping);

    printf("\n--- Suite 2: BSP_GPIO_Init() ---\n");
    RUN_TEST(TestBspGpioInitReadsCurrentODR);
    RUN_TEST(TestBspGpioInitAllOutputsOff);
    RUN_TEST(TestBspGpioInitAllOutputsOn);

    printf("\n--- Suite 3: BSP_WriteOutputs() ---\n");
    RUN_TEST(TestBspWriteOutputsAllZeros);
    RUN_TEST(TestBspWriteOutputsAllOnes);
    RUN_TEST(TestBspWriteOutputsAlternatingPattern01);
    RUN_TEST(TestBspWriteOutputsAlternatingPattern10);
    RUN_TEST(TestBspWriteOutputsSingleOutputOn);
    RUN_TEST(TestBspWriteOutputsIgnoresUpperBits);

    printf("\n--- Suite 4: BSP_SequentialWrites() ---\n");
    RUN_TEST(TestBspSequentialWritesChangeOutputs);

    printf("\n--- Suite 5: BSP_FeedWatchdog() ---\n");
    RUN_TEST(TestBspFeedWatchdogDoesNotCrash);

    printf("\n--- Suite 6: PLC Integration ---\n");
    RUN_TEST(TestBspTypicalPlcScanSequence);

    printf("\n--- Suite 7: Boundary Conditions ---\n");
    RUN_TEST(TestBspBoundaryMaximumOutputValue);
    RUN_TEST(TestBspBoundaryMinimumOutputValue);
    RUN_TEST(TestBspBoundaryRejectOutOfRangeOutputImage);

    printf("\n--- Suite 8: State Transitions ---\n");
    RUN_TEST(TestBspStateTransitionInitBeforeGpioCommon);
    RUN_TEST(TestBspStateTransitionWriteOutputsBeforeGpioCommon);
    RUN_TEST(TestBspStateTransitionNormalSequence);

    return UNITY_END();
}