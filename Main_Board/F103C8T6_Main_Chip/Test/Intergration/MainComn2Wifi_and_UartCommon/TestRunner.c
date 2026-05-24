/*
 * TestRunner.c
 *
 * Unity runner for MainComn2WifiCard and UartCommon integration tests.
 */

#include "unity.h"

#include <stdio.h>

extern void TestBuildHeartbeatResponseThenSendOverUart(void);
extern void TestBuildNackForCrcFailThenSendOverUart(void);
extern void TestReceiveHeartbeatRequestFromUartThenParse(void);
extern void TestReceiveDataPushFromUartThenParsePayload(void);
extern void TestCrcErrorFromUartRxBuildsAndSendsNack(void);

void setUp(void);
void tearDown(void);

int main(void)
{
    UNITY_BEGIN();

    printf("\n--- SUITE 1 - Protocol TX path into UartCommon ---\n");
    RUN_TEST(TestBuildHeartbeatResponseThenSendOverUart);
    RUN_TEST(TestBuildNackForCrcFailThenSendOverUart);

    printf("\n--- SUITE 2 - UartCommon RX path into protocol parser ---\n");
    RUN_TEST(TestReceiveHeartbeatRequestFromUartThenParse);
    RUN_TEST(TestReceiveDataPushFromUartThenParsePayload);

    printf("\n--- SUITE 3 - Error feedback across protocol and UART ---\n");
    RUN_TEST(TestCrcErrorFromUartRxBuildsAndSendsNack);

    return UNITY_END();
}
