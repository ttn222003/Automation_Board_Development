/*
 * TestPlcGpio.c
 *
 * Unit tests for the PlcGpio Core layer – STM32F103C8T6
 * Updated: Apr 27, 2026
 *
 */

#include "unity.h"
#include "PlcGpio.h"
#include <string.h>

/* ── Helpers ────────────────────────────────────────────────────────────── */

static void FillOutputArray(uint16_t arr[], uint8_t size, uint16_t value)
{
    for (uint8_t i = 0; i < size; i++) {
        arr[i] = value;
    }
}

static void MaskToOutputArray(uint16_t arr[], uint8_t size, uint16_t mask)
{
    for (uint8_t i = 0; i < size; i++) {
        arr[i] = (mask >> i) & 0x01u;
    }
}

/* ── setUp / tearDown ───────────────────────────────────────────────────── */

void setUp(void)
{
    InitPlcCore();
}

void tearDown(void)
{
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 1 – InitPlcCore()
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 1.1
 * After InitPlcCore() the PLC is in Stop state.
 * ReadOutputImage() returns 0 because the DATA was cleared to 0 by init,
 * NOT because of Stop state blocking reads.
 */
void TestCoreInitSetsStopState(void)
{
    SetPlcState(ePlcStateRun);
    InitPlcCore();

    /* ReadOutputImage always returns data – data is 0 after init */
    TEST_ASSERT_EQUAL_UINT16(0x0000u, ReadOutputImage());
}

/*
 * Test case: 1.2
 * After InitPlcCore() all output image slots are cleared to 0.
 * Verified in Run state to confirm it's the data that is 0.
 */
void TestCoreInitClearsAllOutputs(void)
{
    InitPlcCore();
    SetPlcState(ePlcStateRun);

    TEST_ASSERT_EQUAL_UINT16(0x0000u, ReadOutputImage());
}

/*
 * Test case: 1.3
 * InitPlcCore() after outputs were written resets the output data to 0.
 */
void TestCoreInitResetsOutputsAfterWrite(void)
{
    uint16_t all_on[OUTPUT_NUMBER];
    FillOutputArray(all_on, OUTPUT_NUMBER, 1u);

    SetPlcState(ePlcStateRun);
    UpdatePlcOutputs(all_on);
    TEST_ASSERT_EQUAL_UINT16(0x3Fu, ReadOutputImage());

    InitPlcCore();
    /* Still in Stop state – data is 0 → ReadOutputImage returns 0 */
    TEST_ASSERT_EQUAL_UINT16(0x0000u, ReadOutputImage());
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 2 – SetPlcState()
 *
 * SetPlcState() controls whether UpdatePlcOutputs() is allowed to write.
 * It does NOT affect what ReadOutputImage() returns.
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 2.1
 * In Run state, UpdatePlcOutputs() is allowed to write.
 * ReadOutputImage() returns the updated value.
 */
void TestCoreSetStateRunAllowsOutputUpdate(void)
{
    uint16_t all_on[OUTPUT_NUMBER];
    FillOutputArray(all_on, OUTPUT_NUMBER, 1u);

    SetPlcState(ePlcStateRun);
    UpdatePlcOutputs(all_on);

    TEST_ASSERT_EQUAL_UINT16(0x3Fu, ReadOutputImage());
}

/*
 * Test case: 2.2
 * In Stop state, UpdatePlcOutputs() is blocked.
 * ReadOutputImage() returns the LAST value before Stop – not 0.
 */
void TestCoreSetStateStopBlocksOutputUpdate(void)
{
    uint16_t pattern[OUTPUT_NUMBER];
    MaskToOutputArray(pattern, OUTPUT_NUMBER, 0x2Au);

    /* Write in Run state */
    SetPlcState(ePlcStateRun);
    UpdatePlcOutputs(pattern);
    TEST_ASSERT_EQUAL_UINT16(0x2Au, ReadOutputImage());

    /* Transition to Stop then try to overwrite */
    uint16_t all_on[OUTPUT_NUMBER];
    FillOutputArray(all_on, OUTPUT_NUMBER, 1u);

    SetPlcState(ePlcStateStop);
    UpdatePlcOutputs(all_on);   /* must be blocked */

    /* Output must still be the value written before Stop */
    TEST_ASSERT_EQUAL_UINT16(0x2Au, ReadOutputImage());
}

/*
 * Test case: 2.3
 * ReadOutputImage() returns consistent value across Run → Stop → Run.
 * Value is preserved in Stop and readable throughout.
 */
void TestCoreSetStateRunStopRunCycle(void)
{
    uint16_t pattern[OUTPUT_NUMBER];
    MaskToOutputArray(pattern, OUTPUT_NUMBER, 0x2Au);

    SetPlcState(ePlcStateRun);
    UpdatePlcOutputs(pattern);
    TEST_ASSERT_EQUAL_UINT16(0x2Au, ReadOutputImage());

    /* Stop: update blocked, but read still returns last value */
    SetPlcState(ePlcStateStop);
    TEST_ASSERT_EQUAL_UINT16(0x2Au, ReadOutputImage());

    /* Run again: value still 0x2A (no new write happened) */
    SetPlcState(ePlcStateRun);
    TEST_ASSERT_EQUAL_UINT16(0x2Au, ReadOutputImage());
}

/*
 * Test case: 2.4
 * ReadOutputImage() in Stop state with no prior writes returns 0.
 * (Data is 0 from init – nothing to do with Stop blocking reads.)
 */
void TestCoreReadOutputImageInStopStateWithNoWriteReturnsZero(void)
{
    /* State is Stop (from setUp), no writes at all */
    TEST_ASSERT_EQUAL_UINT16(0x0000u, ReadOutputImage());
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 3 – UpdatePlcOutputs()
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 3.1
 * In Stop state, UpdatePlcOutputs() must NOT change the output image.
 * The previous value (0 from init) must be preserved.
 */
void TestCoreUpdateOutputsInStopStateHasNoEffect(void)
{
    uint16_t all_on[OUTPUT_NUMBER];
    FillOutputArray(all_on, OUTPUT_NUMBER, 1u);

    /* State is Stop – update must be blocked */
    UpdatePlcOutputs(all_on);

    TEST_ASSERT_EQUAL_UINT16(0x0000u, ReadOutputImage());
}

/*
 * Test case: 3.2
 * In Stop state, UpdatePlcOutputs() preserves the LAST written value,
 * not necessarily 0.
 */
void TestCoreUpdateOutputsInStopStatePreservesLastWrittenValue(void)
{
    uint16_t pattern[OUTPUT_NUMBER];
    MaskToOutputArray(pattern, OUTPUT_NUMBER, 0x15u);

    /* Write in Run state */
    SetPlcState(ePlcStateRun);
    UpdatePlcOutputs(pattern);
    TEST_ASSERT_EQUAL_UINT16(0x15u, ReadOutputImage());

    /* Switch to Stop, try to overwrite with all_on */
    uint16_t all_on[OUTPUT_NUMBER];
    FillOutputArray(all_on, OUTPUT_NUMBER, 1u);

    SetPlcState(ePlcStateStop);
    UpdatePlcOutputs(all_on);

    /* Must still be 0x15 */
    TEST_ASSERT_EQUAL_UINT16(0x15u, ReadOutputImage());
}

/*
 * Test case: 3.3
 * In Run state, UpdatePlcOutputs() with all zeros results in bitmask 0x00.
 */
void TestCoreUpdateOutputsAllZerosInRunState(void)
{
    uint16_t all_off[OUTPUT_NUMBER];
    FillOutputArray(all_off, OUTPUT_NUMBER, 0u);

    SetPlcState(ePlcStateRun);
    UpdatePlcOutputs(all_off);

    TEST_ASSERT_EQUAL_UINT16(0x0000u, ReadOutputImage());
}

/*
 * Test case: 3.4
 * In Run state, UpdatePlcOutputs() with all ones results in bitmask 0x3F.
 */
void TestCoreUpdateOutputsAllOnesInRunState(void)
{
    uint16_t all_on[OUTPUT_NUMBER];
    FillOutputArray(all_on, OUTPUT_NUMBER, 1u);

    SetPlcState(ePlcStateRun);
    UpdatePlcOutputs(all_on);

    TEST_ASSERT_EQUAL_UINT16(0x3Fu, ReadOutputImage());
}

/*
 * Test case: 3.5
 * UpdatePlcOutputs() with alternating 0x15 pattern (0,2,4 ON).
 */
void TestCoreUpdateOutputsAlternatingPattern01(void)
{
    uint16_t pattern[OUTPUT_NUMBER];
    MaskToOutputArray(pattern, OUTPUT_NUMBER, 0x15u);

    SetPlcState(ePlcStateRun);
    UpdatePlcOutputs(pattern);

    TEST_ASSERT_EQUAL_UINT16(0x15u, ReadOutputImage());
}

/*
 * Test case: 3.6
 * UpdatePlcOutputs() with alternating 0x2A pattern (1,3,5 ON).
 */
void TestCoreUpdateOutputsAlternatingPattern10(void)
{
    uint16_t pattern[OUTPUT_NUMBER];
    MaskToOutputArray(pattern, OUTPUT_NUMBER, 0x2Au);

    SetPlcState(ePlcStateRun);
    UpdatePlcOutputs(pattern);

    TEST_ASSERT_EQUAL_UINT16(0x2Au, ReadOutputImage());
}

/*
 * Test case: 3.7
 * UpdatePlcOutputs() only uses the LSB of each element.
 * Values > 1 must be treated as 1 (masked by & 0x01 in ReadOutputImage).
 */
void TestCoreUpdateOutputsOnlyLsbIsUsed(void)
{
    uint16_t noisy[OUTPUT_NUMBER];
    FillOutputArray(noisy, OUTPUT_NUMBER, 0xFFu);

    SetPlcState(ePlcStateRun);
    UpdatePlcOutputs(noisy);

    TEST_ASSERT_EQUAL_UINT16(0x3Fu, ReadOutputImage());
}

/*
 * Test case: 3.8
 * A second UpdatePlcOutputs() call overwrites the previous output image.
 */
void TestCoreUpdateOutputsOverwritesPreviousImage(void)
{
    uint16_t first[OUTPUT_NUMBER];
    uint16_t second[OUTPUT_NUMBER];
    MaskToOutputArray(first,  OUTPUT_NUMBER, 0x3Fu);
    MaskToOutputArray(second, OUTPUT_NUMBER, 0x00u);

    SetPlcState(ePlcStateRun);
    UpdatePlcOutputs(first);
    TEST_ASSERT_EQUAL_UINT16(0x3Fu, ReadOutputImage());

    UpdatePlcOutputs(second);
    TEST_ASSERT_EQUAL_UINT16(0x0000u, ReadOutputImage());
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 4 – ReadOutputImage()
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 4.1
 * ReadOutputImage() in Stop state with no prior writes returns 0.
 * (0 is because data was cleared by init, not because Stop blocks reads.)
 */
void TestCoreReadOutputImageReturnsZeroAfterInit(void)
{
    /* Stop state, data = 0 from setUp */
    TEST_ASSERT_EQUAL_UINT16(0x0000u, ReadOutputImage());
}

/*
 * Test case: 4.2
 * ReadOutputImage() in Stop state returns the last written value.
 * Confirms reads are NOT blocked by Stop state.
 */
void TestCoreReadOutputImagePreservesValueInStopState(void)
{
    uint16_t pattern[OUTPUT_NUMBER];
    MaskToOutputArray(pattern, OUTPUT_NUMBER, 0x2Au);

    SetPlcState(ePlcStateRun);
    UpdatePlcOutputs(pattern);

    SetPlcState(ePlcStateStop);

    /* Must return 0x2A, not 0 */
    TEST_ASSERT_EQUAL_UINT16(0x2Au, ReadOutputImage());
}

/*
 * Test case: 4.3
 * In Run state with all outputs 0, returns 0.
 */
void TestCoreReadOutputImageAllZerosInRunState(void)
{
    SetPlcState(ePlcStateRun);
    TEST_ASSERT_EQUAL_UINT16(0x0000u, ReadOutputImage());
}

/*
 * Test case: 4.4
 * In Run state with all outputs 1, returns 0x3F.
 */
void TestCoreReadOutputImageAllOnesInRunState(void)
{
    uint16_t all_on[OUTPUT_NUMBER];
    FillOutputArray(all_on, OUTPUT_NUMBER, 1u);

    SetPlcState(ePlcStateRun);
    UpdatePlcOutputs(all_on);

    TEST_ASSERT_EQUAL_UINT16(0x3Fu, ReadOutputImage());
}

/*
 * Test case: 4.5
 * Walk each output individually: only one bit SET at a time.
 * Verifies correct bit-packing position for each output index.
 */
void TestCoreReadOutputImageSingleOutputOn(void)
{
    for (uint8_t bit = 0; bit < OUTPUT_NUMBER; bit++) {
        InitPlcCore();
        SetPlcState(ePlcStateRun);

        uint16_t pattern[OUTPUT_NUMBER];
        FillOutputArray(pattern, OUTPUT_NUMBER, 0u);
        pattern[bit] = 1u;

        UpdatePlcOutputs(pattern);

        uint16_t expected = (uint16_t)(1u << bit);
        TEST_ASSERT_EQUAL_UINT16(expected, ReadOutputImage());
    }
}

/*
 * Test case: 4.6
 * ReadOutputImage() is idempotent – calling it twice yields the same result.
 */
void TestCoreReadOutputImageIsIdempotent(void)
{
    uint16_t pattern[OUTPUT_NUMBER];
    MaskToOutputArray(pattern, OUTPUT_NUMBER, 0x15u);

    SetPlcState(ePlcStateRun);
    UpdatePlcOutputs(pattern);

    uint16_t first_read  = ReadOutputImage();
    uint16_t second_read = ReadOutputImage();

    TEST_ASSERT_EQUAL_UINT16(first_read, second_read);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 5 – ReadPlcInputs()
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 5.1
 * ReadPlcInputs() is currently a placeholder (TO DO).
 * Verify it can be called without crash.
 */
void TestCoreReadPlcInputsDoesNotCrash(void)
{
    ReadPlcInputs();
    TEST_PASS();
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 6 – GetPlcState()
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 6.1
 * After InitPlcCore(), GetPlcState() must return ePlcStateStop.
 */
void TestCoreGetPlcStateReturnsStopAfterInit(void)
{
    /* setUp already called InitPlcCore() */
    TEST_ASSERT_EQUAL_INT(ePlcStateStop, GetPlcState());
}

/*
 * Test case: 6.2
 * After SetPlcState(Run), GetPlcState() must return ePlcStateRun.
 */
void TestCoreGetPlcStateReturnsRunAfterSetRun(void)
{
    SetPlcState(ePlcStateRun);
    TEST_ASSERT_EQUAL_INT(ePlcStateRun, GetPlcState());
}

/*
 * Test case: 6.3
 * After SetPlcState(Stop), GetPlcState() must return ePlcStateStop.
 */
void TestCoreGetPlcStateReturnsStopAfterSetStop(void)
{
    SetPlcState(ePlcStateRun);
    SetPlcState(ePlcStateStop);
    TEST_ASSERT_EQUAL_INT(ePlcStateStop, GetPlcState());
}

/*
 * Test case: 6.4
 * GetPlcState() tracks every transition correctly across Run → Stop → Run.
 */
void TestCoreGetPlcStateTracksTransitions(void)
{
    TEST_ASSERT_EQUAL_INT(ePlcStateStop, GetPlcState());

    SetPlcState(ePlcStateRun);
    TEST_ASSERT_EQUAL_INT(ePlcStateRun, GetPlcState());

    SetPlcState(ePlcStateStop);
    TEST_ASSERT_EQUAL_INT(ePlcStateStop, GetPlcState());

    SetPlcState(ePlcStateRun);
    TEST_ASSERT_EQUAL_INT(ePlcStateRun, GetPlcState());
}

/*
 * Test case: 6.5
 * GetPlcState() is idempotent – calling it twice returns the same value.
 */
void TestCoreGetPlcStateIsIdempotent(void)
{
    SetPlcState(ePlcStateRun);
    TEST_ASSERT_EQUAL_INT(GetPlcState(), GetPlcState());
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SUITE 7 – Integration: full PLC scan cycle
 * ═══════════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 7.1
 * Mirrors the real PLCScanTask loop:
 *   InitPlcCore() → SetState(Run) → ReadPlcInputs() →
 *   UpdatePlcOutputs() → ReadOutputImage()
 *
 * Stop state now FREEZES outputs at last value (not forces to 0).
 */
void TestCoreFullPlcScanCycleWithStateCheck(void)
{
    /* ── Init phase ──────────────────────────────── */
    InitPlcCore();
    TEST_ASSERT_EQUAL_INT(ePlcStateStop, GetPlcState());

    SetPlcState(ePlcStateRun);
    TEST_ASSERT_EQUAL_INT(ePlcStateRun, GetPlcState());

    /* ── Scan 1: outputs 1,3,5 ON (0x2A) ─────────── */
    ReadPlcInputs();

    uint16_t scan1[OUTPUT_NUMBER];
    MaskToOutputArray(scan1, OUTPUT_NUMBER, 0x2Au);
    UpdatePlcOutputs(scan1);
    TEST_ASSERT_EQUAL_UINT16(0x2Au, ReadOutputImage());

    /* ── Scan 2: outputs 0,2,4 ON (0x15) ─────────── */
    ReadPlcInputs();

    uint16_t scan2[OUTPUT_NUMBER];
    MaskToOutputArray(scan2, OUTPUT_NUMBER, 0x15u);
    UpdatePlcOutputs(scan2);
    TEST_ASSERT_EQUAL_UINT16(0x15u, ReadOutputImage());

    /* ── Scan 3: Stop state – outputs FROZEN at 0x15 ─
     * UpdatePlcOutputs is blocked; ReadOutputImage
     * still returns the last written value.          */
    SetPlcState(ePlcStateStop);
    TEST_ASSERT_EQUAL_INT(ePlcStateStop, GetPlcState());
    TEST_ASSERT_EQUAL_UINT16(0x15u, ReadOutputImage());  /* ← was 0x00 before refactor */
}

/* End of TestPlcGpio.c */