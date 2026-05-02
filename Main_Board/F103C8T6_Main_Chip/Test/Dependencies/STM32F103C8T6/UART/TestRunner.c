/*
 * TestRunner.c
 *
 * Unity test runner for the BspUart layer – STM32 <-> ESP32C3
 * Updated: Apr 30, 2026
 *
 */

#include "unity.h"
#include <stdio.h>

/* ── SUITE 1 – InitBspUart() ─────────────────────────────────────────────── */
extern void TestInitBspUartWithValidHandleReturnsOk(void);
extern void TestInitBspUartNullHandleReturnsError(void);
extern void TestSendBspUartBeforeInitReturnsNotInit(void);
extern void TestSendBspUartAfterResetReturnsNotInit(void);

/* ── SUITE 2 – SendBspUart() ─────────────────────────────────────────────── */
extern void TestSendBspUartCallsHalTransmit(void);
extern void TestSendBspUartHalErrorReturnsError(void);
extern void TestSendBspUartNullPointerReturnsError(void);
extern void TestSendBspUartZeroLenReturnsError(void);
extern void TestSendBspUartHalBusyReturnsError(void);

/* ── SUITE 3 – HandleBspUartIsrRx() → ring buffer ───────────────────────── */
extern void TestHandleIsrRxOneByteIsAvailable(void);
extern void TestHandleIsrRxMultipleBytesAvailable(void);
extern void TestHandleIsrRxPreservesFifoOrder(void);
extern void TestHandleIsrRxZeroBytePayloadIsReadable(void);
extern void TestHandleIsrRxOverflowSetsFlag(void);
extern void TestHandleIsrRxOverflowFlagClearsAfterCheck(void);
extern void TestHandleIsrRxRingBufferWrapsAround(void);

/* ── SUITE 4 – ReadBspUart() ─────────────────────────────────────────────── */
extern void TestReadBspUartEmptyBufferReturnsNoData(void);
extern void TestReadBspUartReadsOnlyAvailableBytes(void);
extern void TestReadBspUartNullBufferReturnsError(void);
extern void TestReadBspUartDecreasesAvailableCount(void);
extern void TestReadBspUartPartialReadReportsActualCount(void);

/* ── setUp / tearDown declared in TestUART.c ─────────────────────────────── */
void setUp(void);
void tearDown(void);

/* ─────────────────────────────────────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    printf("\n--- SUITE 1 - InitBspUart() ---\n");
    RUN_TEST(TestInitBspUartWithValidHandleReturnsOk);
    RUN_TEST(TestInitBspUartNullHandleReturnsError);
    RUN_TEST(TestSendBspUartBeforeInitReturnsNotInit);
    RUN_TEST(TestSendBspUartAfterResetReturnsNotInit);

    printf("\n--- SUITE 2 - SendBspUart() ---\n");
    RUN_TEST(TestSendBspUartCallsHalTransmit);
    RUN_TEST(TestSendBspUartHalErrorReturnsError);
    RUN_TEST(TestSendBspUartNullPointerReturnsError);
    RUN_TEST(TestSendBspUartZeroLenReturnsError);
    RUN_TEST(TestSendBspUartHalBusyReturnsError);

    printf("\n--- SUITE 3 - HandleBspUartIsrRx() -> ring buffer ---\n");
    RUN_TEST(TestHandleIsrRxOneByteIsAvailable);
    RUN_TEST(TestHandleIsrRxMultipleBytesAvailable);
    RUN_TEST(TestHandleIsrRxPreservesFifoOrder);
    RUN_TEST(TestHandleIsrRxZeroBytePayloadIsReadable);
    RUN_TEST(TestHandleIsrRxOverflowSetsFlag);
    RUN_TEST(TestHandleIsrRxOverflowFlagClearsAfterCheck);
    RUN_TEST(TestHandleIsrRxRingBufferWrapsAround);

    printf("\n--- SUITE 4 - ReadBspUart() ---\n");
    RUN_TEST(TestReadBspUartEmptyBufferReturnsNoData);
    RUN_TEST(TestReadBspUartReadsOnlyAvailableBytes);
    RUN_TEST(TestReadBspUartNullBufferReturnsError);
    RUN_TEST(TestReadBspUartDecreasesAvailableCount);
    RUN_TEST(TestReadBspUartPartialReadReportsActualCount);

    return UNITY_END();
}