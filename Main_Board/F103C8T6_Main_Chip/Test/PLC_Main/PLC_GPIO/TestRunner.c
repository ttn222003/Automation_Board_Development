/*
 * TestRunnerPlcGpio.c
 *
 * Unity test runner for the PlcGpio Core layer.
 */

#include "unity.h"
#include <stdio.h>

/* ── Suite 1: InitPlcCore() ─────────────────────────────────────────── */
extern void TestCoreInitSetsStopState(void);
extern void TestCoreInitClearsAllOutputs(void);
extern void TestCoreInitResetsOutputsAfterWrite(void);

/* ── Suite 2: SetPlcState() ─────────────────────────────────────────── */
extern void TestCoreSetStateRunAllowsOutputUpdate(void);
extern void TestCoreSetStateStopBlocksOutputUpdate(void);
extern void TestCoreSetStateRunStopRunCycle(void);
extern void TestCoreReadOutputImageInStopStateWithNoWriteReturnsZero(void);

/* ── Suite 3: UpdatePlcOutputs() ────────────────────────────────────── */
extern void TestCoreUpdateOutputsInStopStateHasNoEffect(void);
extern void TestCoreUpdateOutputsInStopStatePreservesLastWrittenValue(void);
extern void TestCoreUpdateOutputsAllZerosInRunState(void);
extern void TestCoreUpdateOutputsAllOnesInRunState(void);
extern void TestCoreUpdateOutputsAlternatingPattern01(void);
extern void TestCoreUpdateOutputsAlternatingPattern10(void);
extern void TestCoreUpdateOutputsOnlyLsbIsUsed(void);
extern void TestCoreUpdateOutputsOverwritesPreviousImage(void);

/* ── Suite 4: ReadOutputImage() ─────────────────────────────────────── */
extern void TestCoreReadOutputImageReturnsZeroAfterInit(void);
extern void TestCoreReadOutputImagePreservesValueInStopState(void);
extern void TestCoreReadOutputImageAllZerosInRunState(void);
extern void TestCoreReadOutputImageAllOnesInRunState(void);
extern void TestCoreReadOutputImageSingleOutputOn(void);
extern void TestCoreReadOutputImageIsIdempotent(void);

/* ── Suite 5: ReadPlcInputs() ───────────────────────────────────────── */
extern void TestCoreReadPlcInputsDoesNotCrash(void);

/* ── Suite 6: GetPlcState() ───────────────────────────────────────────── */
extern void TestCoreGetPlcStateReturnsStopAfterInit(void);
extern void TestCoreGetPlcStateReturnsRunAfterSetRun(void);
extern void TestCoreGetPlcStateReturnsStopAfterSetStop(void);
extern void TestCoreGetPlcStateTracksTransitions(void);
extern void TestCoreGetPlcStateIsIdempotent(void);

/* ── Suite 7: Integration: full PLC scan cycle ───────────────────────────── */
extern void TestCoreFullPlcScanCycleWithStateCheck(void);

/* ──────────────────────────────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    printf("\n--- Suite 1: InitPlcCore() ---\n");
    RUN_TEST(TestCoreInitSetsStopState);
    RUN_TEST(TestCoreInitClearsAllOutputs);
    RUN_TEST(TestCoreInitResetsOutputsAfterWrite);

    printf("\n--- Suite 2: SetPlcState() ---\n");
    RUN_TEST(TestCoreSetStateRunAllowsOutputUpdate);
    RUN_TEST(TestCoreSetStateStopBlocksOutputUpdate);
    RUN_TEST(TestCoreSetStateRunStopRunCycle);
    RUN_TEST(TestCoreReadOutputImageInStopStateWithNoWriteReturnsZero);

    printf("\n--- Suite 3: UpdatePlcOutputs() ---\n");
    RUN_TEST(TestCoreUpdateOutputsInStopStateHasNoEffect);
    RUN_TEST(TestCoreUpdateOutputsInStopStatePreservesLastWrittenValue);
    RUN_TEST(TestCoreUpdateOutputsAllZerosInRunState);
    RUN_TEST(TestCoreUpdateOutputsAllOnesInRunState);
    RUN_TEST(TestCoreUpdateOutputsAlternatingPattern01);
    RUN_TEST(TestCoreUpdateOutputsAlternatingPattern10);
    RUN_TEST(TestCoreUpdateOutputsOnlyLsbIsUsed);
    RUN_TEST(TestCoreUpdateOutputsOverwritesPreviousImage);

    printf("\n--- Suite 4: ReadOutputImage() ---\n");
    RUN_TEST(TestCoreReadOutputImageReturnsZeroAfterInit);
    RUN_TEST(TestCoreReadOutputImagePreservesValueInStopState);
    RUN_TEST(TestCoreReadOutputImageAllZerosInRunState);
    RUN_TEST(TestCoreReadOutputImageAllOnesInRunState);
    RUN_TEST(TestCoreReadOutputImageSingleOutputOn);
    RUN_TEST(TestCoreReadOutputImageIsIdempotent);

    printf("\n--- Suite 5: ReadPlcInputs() ---\n");
    RUN_TEST(TestCoreReadPlcInputsDoesNotCrash);

    printf("\n--- Suite 6: GetPlcState() ---\n");
    RUN_TEST(TestCoreGetPlcStateReturnsStopAfterInit);
    RUN_TEST(TestCoreGetPlcStateReturnsRunAfterSetRun);
    RUN_TEST(TestCoreGetPlcStateReturnsStopAfterSetStop);
    RUN_TEST(TestCoreGetPlcStateTracksTransitions);
    RUN_TEST(TestCoreGetPlcStateIsIdempotent);

    printf("\n--- Suite 7: Integration: full PLC scan cycle ---\n");
    RUN_TEST(TestCoreFullPlcScanCycleWithStateCheck);

    return UNITY_END();
}