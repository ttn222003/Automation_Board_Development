#include "unity.h"
#include "unity_color.h"
#include "UartBootloaderProtocolDepenedencies.h"

void setUp(void)
{

}

void tearDown(void)
{

}

void TransmittEightDataToHostTest(void)
{
    TransmittDataToHost(8);
}

int main(void)
{
    printf(COL_YELLOW "==== START DEPE TEST ====\n" COL_RESET);
    UNITY_BEGIN();
    printf(COL_YELLOW "==== END DEPE TEST ====\n" COL_RESET);
    return UNITY_END();
}