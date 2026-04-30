/*
 * TestRunnerPlcExecuteLogic.c
 *
 * Unity test runner for PlcMainExecuteLogic.
 */

#include "unity.h"
#include <stdio.h>

/* ── Suite 1: InitializeVariable() ─────────────────────────────────── */
extern void TestExecuteInitVariableReturnsSuccess(void);
extern void TestExecuteInitVariableClearsOutArray(void);
extern void TestExecuteInitVariableIsIdempotent(void);

/* ── Suite 2: output_image population ──────────────────────────────── */
extern void TestExecuteLogicWritesAllOutputSlots(void);
extern void TestExecuteLogicOutputValuesAreBinary(void);
extern void TestExecuteLogicDoesNotWriteBeyondOutputArray(void);

/* ── Suite 3: OUT[] stateful behavior ──────────────────────────────── */
extern void TestExecuteLogicOutIsStatefulBetweenScans(void);
extern void TestExecuteLogicInitClearsResidualStateBeforeFirstScan(void);

/* ── Suite 4: User logic output values ─────────────────────────────── */
extern void TestExecuteLogicCurrentPatternIs0x15(void);
extern void TestExecuteLogicOutputMapsToExpectedBitmask(void);

/* ── Suite 5: Integration ───────────────────────────────────────────── */
extern void TestExecuteLogicOutputIsValidForUpdatePlcOutputs(void);
extern void TestExecuteLogicConsistentAcrossMultipleScans(void);

/* ─────────────────────────────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    printf("\n--- Suite 1: InitializeVariable() ---\n");
    RUN_TEST(TestExecuteInitVariableReturnsSuccess);
    RUN_TEST(TestExecuteInitVariableClearsOutArray);
    RUN_TEST(TestExecuteInitVariableIsIdempotent);

    printf("\n--- Suite 2: output_image Population ---\n");
    RUN_TEST(TestExecuteLogicWritesAllOutputSlots);
    RUN_TEST(TestExecuteLogicOutputValuesAreBinary);
    RUN_TEST(TestExecuteLogicDoesNotWriteBeyondOutputArray);

    printf("\n--- Suite 3: OUT[] Stateful Behavior ---\n");
    RUN_TEST(TestExecuteLogicOutIsStatefulBetweenScans);
    RUN_TEST(TestExecuteLogicInitClearsResidualStateBeforeFirstScan);

    printf("\n--- Suite 4: User Logic Output Values ---\n");
    RUN_TEST(TestExecuteLogicCurrentPatternIs0x15);
    RUN_TEST(TestExecuteLogicOutputMapsToExpectedBitmask);

    printf("\n--- Suite 5: Integration ---\n");
    RUN_TEST(TestExecuteLogicOutputIsValidForUpdatePlcOutputs);
    RUN_TEST(TestExecuteLogicConsistentAcrossMultipleScans);

    return UNITY_END();
}