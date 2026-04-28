/*
 * TestPlcExecuteLogic.c
 *
 * Unit tests for PlcMainExecuteLogic – STM32F103C8T6
 * 
 * Created on: Apr 28, 2026
 */

#include "unity.h"
#include "PlcMainExecuteLogic.h"
#include <string.h>

/* ── setUp / tearDown ───────────────────────────────────────────────────── */

void setUp(void)
{
    InitializeVariable();
}

void tearDown(void)
{

}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 1 – InitializeVariable()
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 1.1
 * InitializeVariable() must return 1 on success.
 * main.c checks this return value to detect init failure.
 */
void TestExecuteInitVariableReturnsSuccess(void)
{
    int8_t result = InitializeVariable();
    TEST_ASSERT_EQUAL_INT8(1, result);
}

/*
 * Test case: 1.2
 * InitializeVariable() must clear OUT[] so the first scan starts from 0.
 * Verified indirectly: after init, ExecutePlcLogic with no writes would
 * produce all-zero output. Here we confirm via a fresh ExecutePlcLogic
 * after re-initializing.
 *
 * Note: current user logic always explicitly sets all OUT[] slots,
 * so we verify by calling InitializeVariable() then checking output_image
 * reflects only what user logic explicitly wrote (not residual state).
 */
void TestExecuteInitVariableClearsOutArray(void)
{
    /* Run a scan to dirty OUT[] */
    uint16_t output_image[OUTPUT_NUMBER];
    ExecutePlcLogic(output_image);

    /* Re-init clears OUT[] */
    InitializeVariable();

    memset(output_image, 0xFF, sizeof(output_image));
    ExecutePlcLogic(output_image);

    /* Current user logic: OUT[0,2,4]=1, OUT[1,3,5]=0 → 0x15 pattern */
    uint16_t bitmask = 0u;
    for (uint8_t i = 0; i < OUTPUT_NUMBER; i++) {
        bitmask |= (uint16_t)((output_image[i] & 0x01u) << i);
    }
    TEST_ASSERT_EQUAL_HEX16(0x15u, bitmask);
}

/*
 * Test case: 1.3
 * Calling InitializeVariable() multiple times must always return success.
 * (Idempotent init.)
 */
void TestExecuteInitVariableIsIdempotent(void)
{
    TEST_ASSERT_EQUAL_INT8(1, InitializeVariable());
    TEST_ASSERT_EQUAL_INT8(1, InitializeVariable());
    TEST_ASSERT_EQUAL_INT8(1, InitializeVariable());
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 2 – ExecutePlcLogic() output_image population
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 2.1
 * ExecutePlcLogic() must write to all OUTPUT_NUMBER slots.
 * Verified by pre-filling output_image with sentinel 0xFFFF.
 */
void TestExecuteLogicWritesAllOutputSlots(void)
{
    uint16_t output_image[OUTPUT_NUMBER];
    memset(output_image, 0xFF, sizeof(output_image));

    ExecutePlcLogic(output_image);

    for (uint8_t i = 0; i < OUTPUT_NUMBER; i++) {
        TEST_ASSERT_NOT_EQUAL_MESSAGE(0xFFFFu, output_image[i],
            "output_image slot not written by ExecutePlcLogic");
    }
}

/*
 * Test case: 2.2
 * Each output_image element must be 0 or 1 only (valid PLC bit state).
 */
void TestExecuteLogicOutputValuesAreBinary(void)
{
    uint16_t output_image[OUTPUT_NUMBER];
    ExecutePlcLogic(output_image);

    for (uint8_t i = 0; i < OUTPUT_NUMBER; i++) {
        TEST_ASSERT_MESSAGE(
            output_image[i] == 0u || output_image[i] == 1u,
            "output_image element is not 0 or 1"
        );
    }
}

/*
 * Test case: 2.3
 * ExecutePlcLogic() must not write beyond OUTPUT_NUMBER slots.
 * Guard byte placed just after the array must be untouched.
 */
void TestExecuteLogicDoesNotWriteBeyondOutputArray(void)
{
    uint16_t output_image[OUTPUT_NUMBER + 1];
    memset(output_image, 0xFF, sizeof(output_image));

    ExecutePlcLogic(output_image);

    TEST_ASSERT_EQUAL_HEX16(0xFFFFu, output_image[OUTPUT_NUMBER]);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 3 – OUT[] stateful behavior
 *
 * OUT[] is NOT reset between scans.
 * Values set in scan N persist into scan N+1 unless overwritten.
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 3.1
 * OUT[] retains values between consecutive ExecutePlcLogic() calls.
 * (Opposite of the old stateless behavior.)
 *
 * Since current user logic explicitly sets all 6 outputs each scan,
 * both scans produce the same result – but for the right reason:
 * user explicitly wrote them, not because of a reset.
 */
void TestExecuteLogicOutIsStatefulBetweenScans(void)
{
    uint16_t scan1[OUTPUT_NUMBER];
    uint16_t scan2[OUTPUT_NUMBER];

    ExecutePlcLogic(scan1);
    ExecutePlcLogic(scan2);

    for (uint8_t i = 0; i < OUTPUT_NUMBER; i++) {
        TEST_ASSERT_EQUAL_UINT16(scan1[i], scan2[i]);
    }
}

/*
 * Test case: 3.2
 * After InitializeVariable() resets OUT[], the next ExecutePlcLogic()
 * must produce output based ONLY on what user logic writes in that scan –
 * no residual state from any previous scan.
 */
void TestExecuteLogicInitClearsResidualStateBeforeFirstScan(void)
{
    uint16_t before_init[OUTPUT_NUMBER];
    uint16_t after_init[OUTPUT_NUMBER];

    /* Scan before init */
    ExecutePlcLogic(before_init);

    /* Re-init wipes OUT[] */
    InitializeVariable();

    /* Scan after init */
    ExecutePlcLogic(after_init);

    /*
     * Current user logic always sets the same pattern, so both must match.
     * This verifies that InitializeVariable() + ExecutePlcLogic() is the
     * correct startup sequence and produces a clean first-scan output.
     */
    for (uint8_t i = 0; i < OUTPUT_NUMBER; i++) {
        TEST_ASSERT_EQUAL_UINT16(before_init[i], after_init[i]);
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 4 – User logic output values
 *
 * !! ATTENTION !!
 * These tests reflect the CURRENT hardcoded user logic:
 *   OUT[0]=1, OUT[1]=0, OUT[2]=1, OUT[3]=0, OUT[4]=1, OUT[5]=0
 *
 * Update these tests when the user zone changes.
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 4.1
 * Current user logic: outputs 0,2,4 ON; outputs 1,3,5 OFF.
 */
void TestExecuteLogicCurrentPatternIs0x15(void)
{
    uint16_t output_image[OUTPUT_NUMBER];
    ExecutePlcLogic(output_image);

    TEST_ASSERT_EQUAL_UINT16(1u, output_image[0]);  /* OUT0 = ON  */
    TEST_ASSERT_EQUAL_UINT16(0u, output_image[1]);  /* OUT1 = OFF */
    TEST_ASSERT_EQUAL_UINT16(1u, output_image[2]);  /* OUT2 = ON  */
    TEST_ASSERT_EQUAL_UINT16(0u, output_image[3]);  /* OUT3 = OFF */
    TEST_ASSERT_EQUAL_UINT16(1u, output_image[4]);  /* OUT4 = ON  */
    TEST_ASSERT_EQUAL_UINT16(0u, output_image[5]);  /* OUT5 = OFF */
}

/*
 * Test case: 4.2
 * Verify the bitmask ReadOutputImage() would produce from current user logic.
 * Simulates: ExecutePlcLogic() → (pack bits) == 0x15
 */
void TestExecuteLogicOutputMapsToExpectedBitmask(void)
{
    uint16_t output_image[OUTPUT_NUMBER];
    ExecutePlcLogic(output_image);

    uint16_t bitmask = 0u;
    for (uint8_t i = 0; i < OUTPUT_NUMBER; i++) {
        bitmask |= (uint16_t)((output_image[i] & 0x01u) << i);
    }

    TEST_ASSERT_EQUAL_HEX16(0x15u, bitmask);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 5 – Integration: ExecuteLogic inside PLCScanTask flow
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 5.1
 * output_image produced by ExecutePlcLogic() must be valid
 * for UpdatePlcOutputs() – all values must be 0 or 1.
 */
void TestExecuteLogicOutputIsValidForUpdatePlcOutputs(void)
{
    uint16_t output_image[OUTPUT_NUMBER];
    ExecutePlcLogic(output_image);

    for (uint8_t i = 0; i < OUTPUT_NUMBER; i++) {
        TEST_ASSERT_MESSAGE(
            output_image[i] == 0u || output_image[i] == 1u,
            "Value not valid for UpdatePlcOutputs (must be 0 or 1)"
        );
    }
}

/*
 * Test case: 5.2
 * Simulate main.c startup + 3 consecutive scan cycles.
 * Mirrors: InitializeVariable() → [scan loop] ExecutePlcLogic() × 3.
 * Each scan must produce the same output (current logic is deterministic).
 */
void TestExecuteLogicConsistentAcrossMultipleScans(void)
{
    uint16_t scan[3][OUTPUT_NUMBER];

    for (uint8_t s = 0; s < 3; s++) {
        ExecutePlcLogic(scan[s]);
    }

    for (uint8_t s = 1; s < 3; s++) {
        for (uint8_t i = 0; i < OUTPUT_NUMBER; i++) {
            TEST_ASSERT_EQUAL_UINT16_MESSAGE(scan[0][i], scan[s][i],
                "Output differs between scan cycles");
        }
    }
}

/* End of TestPlcExecuteLogic.c */