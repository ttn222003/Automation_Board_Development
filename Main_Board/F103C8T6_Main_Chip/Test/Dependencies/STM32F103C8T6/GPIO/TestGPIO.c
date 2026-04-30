/*
 * TestGPIO.c
 *
 * Unit tests for the GpioCommon BSP layer – STM32F103C8T6
 */

#include "unity.h"
#include "Mockstm32f1xx_hal_gpio.h"
#include "GpioCommon.h"
#include <string.h>

/* ── GPIOA mock register ───────────────────────────────────────────────────
 * GpioCommon.c accesses GPIOA->ODR directly (BSP_GPIO_Init).
 * We provide the symbol here so the linker resolves it for the host build.
 * CMock compares MEMORY at the pointer, so passing the same GPIOA to both
 * _Expect and the real call always matches – no extra work needed.
 * ─────────────────────────────────────────────────────────────────────── */
static GPIO_TypeDef GPIOA_Mock;
GPIO_TypeDef       *GPIOA = &GPIOA_Mock;

static const uint16_t PIN_MAP[OUTPUT_NUMBER] = {
    GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2,
    GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5
};

/* ── Helper: queue Expects for all 6 outputs given a bitmask ──────────── */
static void ExpectWriteOutputs(uint16_t image)
{
    for (uint8_t i = 0; i < OUTPUT_NUMBER; i++) {
        GPIO_PinState state;
        if ((image >> i) & 0x01u) {
            state = GPIO_PIN_SET;
        } else {
            state = GPIO_PIN_RESET;
        }
        HAL_GPIO_WritePin_Expect(GPIOA, PIN_MAP[i], state);
    }
}

void setUp(void)
{
    Mockstm32f1xx_hal_gpio_Init();
    memset(&GPIOA_Mock, 0, sizeof(GPIOA_Mock));
}

void tearDown(void)
{
    Mockstm32f1xx_hal_gpio_Verify();        /* fail if any Expect was unused */
    Mockstm32f1xx_hal_gpio_Destroy();       /* free CMock memory             */
}

/* ═══════════════════════════════════════════════════════════════════════
 * SUITE 1 – GpioCommon()  Initialisation
 * ═══════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 1.1
 * GpioCommon() does not call any HAL function; it only populates the
 * internal Output struct.  No Expects are queued here.
 */
void TestBspGpioCommonReturnsSuccess(void)
{
    int8_t result = GpioCommon();
    TEST_ASSERT_EQUAL_INT8(1, result);
}

/*
 * Test case: 1.2
 * After GpioCommon() the pin mapping must be correct: a subsequent
 * BSP_WriteOutputs call must address pins 0-5 in order.
 * Pattern 0x2A = 0b101010 → pins 1,3,5 SET, 0,2,4 RESET.
 */
void TestBspGpioCommonInitializesPinMapping(void)
{
    GpioCommon();

    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

    BSP_WriteOutputs(0x2A);
}

/* ═══════════════════════════════════════════════════════════════════════
 * SUITE 2 – BSP_GPIO_Init()
 *
 * BSP_GPIO_Init reads GPIOA->ODR then writes each bit to the matching pin.
 * We set ODR on our mock struct, queue the expected HAL calls, then invoke.
 * ═══════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 2.1
 * ODR = 0b010101 → outputs 0, 2, 4 high
 */
void TestBspGpioInitReadsCurrentODR(void)
{
    GpioCommon();
    GPIOA->ODR = 0x15u;    /* 0b010101 */

    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

    BSP_GPIO_Init();
}

/*
 * Test case: 2.2
 * ODR = 0 → all outputs must be driven low
 */
void TestBspGpioInitAllOutputsOff(void)
{
    GpioCommon();
    GPIOA->ODR = 0x00u;

    ExpectWriteOutputs(0x00);

    BSP_GPIO_Init();
}

/*
 * Test case: 2.3
 * ODR = 0x3F → all 6 outputs must be driven high
 */
void TestBspGpioInitAllOutputsOn(void)
{
    GpioCommon();
    GPIOA->ODR = 0x3Fu;

    ExpectWriteOutputs(0x3Fu);

    BSP_GPIO_Init();
}

/* ═══════════════════════════════════════════════════════════════════════
 * SUITE 3 – BSP_WriteOutputs(uint16_t output_image)
 * ═══════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 3.1
 * 0x00 → every output RESET
 */
void TestBspWriteOutputsAllZeros(void)
{
    GpioCommon();
    ExpectWriteOutputs(0x00);
    BSP_WriteOutputs(0x00);
}

/*
 * Test case: 3.2
 * 0x3F = 0b111111 → every output SET
 */
void TestBspWriteOutputsAllOnes(void)
{
    GpioCommon();
    ExpectWriteOutputs(0x3Fu);
    BSP_WriteOutputs(0x3Fu);
}

/*
 * Test case: 3.3
 * 0x15 = 0b010101 → outputs 0, 2, 4 SET; 1, 3, 5 RESET
 */
void TestBspWriteOutputsAlternatingPattern01(void)
{
    GpioCommon();

    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

    BSP_WriteOutputs(0x15);
}

/*
 * Test case: 3.4
 * 0x2A = 0b101010 → outputs 1, 3, 5 SET; 0, 2, 4 RESET
 */
void TestBspWriteOutputsAlternatingPattern10(void)
{
    GpioCommon();

    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

    BSP_WriteOutputs(0x2A);
}

/*
 * Test case: 3.5
 * Walk each output individually.
 * For each iteration we reset+reinit the mock so each sub-case is isolated.
 * Example: Write OUT1, OUT0 change -> Fail
 */
void TestBspWriteOutputsSingleOutputOn(void)
{
    for (uint8_t bit = 0; bit < OUTPUT_NUMBER; bit++) {
        /* Isolated mock lifecycle per sub-case */
        Mockstm32f1xx_hal_gpio_Verify();
        Mockstm32f1xx_hal_gpio_Destroy();
        Mockstm32f1xx_hal_gpio_Init();
        memset(&GPIOA_Mock, 0, sizeof(GPIOA_Mock));

        GpioCommon();

        uint16_t pattern = (uint16_t)(1u << bit);
        ExpectWriteOutputs(pattern);    /* only pin[bit] SET, rest RESET */

        BSP_WriteOutputs(pattern);
    }
}

/*
 * Test case: 3.6
 * Upper bits (above bit 5) must be masked by the shift logic and must not
 * affect the 6 physical outputs.  0xFF15 has the same lower 6 bits as 0x15.
 */
void TestBspWriteOutputsIgnoresUpperBits(void)
{
    GpioCommon();

    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

    BSP_WriteOutputs((uint16_t)(0xFF00u | 0x15u));
}

/* ═══════════════════════════════════════════════════════════════════════
 * SUITE 4 – Sequential writes  (simulate multiple PLC scan cycles)
 * ═══════════════════════════════════════════════════════════════════════ */

void TestBspSequentialWritesChangeOutputs(void)
{
    GpioCommon();

    /* ── Scan 1: 0x00 – all off ─────────────────────────── */
    ExpectWriteOutputs(0x00);
    BSP_WriteOutputs(0x00);

    /* ── Scan 2: 0x3F – all on ──────────────────────────── */
    ExpectWriteOutputs(0x3Fu);
    BSP_WriteOutputs(0x3Fu);

    /* ── Scan 3: 0x15 – alternating ─────────────────────── */
    ExpectWriteOutputs(0x15u);
    BSP_WriteOutputs(0x15u);
}

/* ═══════════════════════════════════════════════════════════════════════
 * SUITE 5 – BSP_FeedWatchdog()
 * ═══════════════════════════════════════════════════════════════════════ */

/*
 * Currently an empty placeholder.  No HAL calls expected.
 * The test just verifies it can be called without crashing and that CMock
 * raises no "unexpected call" error.
 */
void TestBspFeedWatchdogDoesNotCrash(void)
{
    BSP_FeedWatchdog();
    TEST_PASS();
}

/* ═══════════════════════════════════════════════════════════════════════
 * SUITE 6 – Integration: full PLCScanTask sequence
 * ═══════════════════════════════════════════════════════════════════════ */

/*
 * Mirrors the real task flow:
 *   GpioCommon()  →  BSP_GPIO_Init()  →  BSP_WriteOutputs(ReadOutputImage())
 *
 * ReadOutputImage() is simulated by passing a fixed pattern so the BSP
 * layer remains isolated from the Core layer.
 */
void TestBspTypicalPlcScanSequence(void)
{
    /* ── Initialisation phase ──────────────────────────────────────── */
    int8_t init = GpioCommon();
    TEST_ASSERT_EQUAL_INT8(1, init);

    GPIOA->ODR = 0x00u;
    ExpectWriteOutputs(0x00);
    BSP_GPIO_Init();

    /* ── Scan cycle: Core produced output_image = 0x2A ────────────── */
    uint16_t plc_output_image = 0x2Au;
    ExpectWriteOutputs(plc_output_image);
    BSP_WriteOutputs(plc_output_image);

    BSP_FeedWatchdog();   /* no HAL calls expected */
}

/* ═══════════════════════════════════════════════════════════════════════
 * SUITE 7 – Boundary conditions
 * ═══════════════════════════════════════════════════════════════════════ */

/*
 * Test case: 7.1
 * Maximum valid 6-bit image (0x3F = 0b111111)
 */
void TestBspBoundaryMaximumOutputValue(void)
{
    GpioCommon();
    ExpectWriteOutputs(0x3Fu);
    BSP_WriteOutputs(0x3Fu);
}

/*
 * Test case: 7.2
 * Minimum image: all outputs off
 */
void TestBspBoundaryMinimumOutputValue(void)
{
    GpioCommon();
    ExpectWriteOutputs(0x00);
    BSP_WriteOutputs(0x00);
}

/* End of TestGPIO.c */