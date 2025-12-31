#include "unity.h"
#include "unity_color.h"
#include "UartBootloaderProtocolDepenedencies.h"
#include "MockUartDriver.h"
#include "MockTimerDriver.h"

void setUp(void)
{

}

void tearDown(void)
{

}

void TransmittEightDataToHostTest(void)
{
    HandleAckForTransmission(TransmittedDataToHost);
    // Expect
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(RESPONE_HANDSHAKE);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(8);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(ACK);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(0);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(105);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(13);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(223);
    DelayMs_Expect(1);
    UartTransmittOneByteData_Expect(114);

    TransmittDataToHost(8);
}

int main(void)
{
    printf(COL_YELLOW "==== START DEPE TEST ====\n" COL_RESET);
    UNITY_BEGIN();
    RUN_TEST(TransmittEightDataToHostTest);
    printf(COL_YELLOW "==== END DEPE TEST ====\n" COL_RESET);
    return UNITY_END();
}