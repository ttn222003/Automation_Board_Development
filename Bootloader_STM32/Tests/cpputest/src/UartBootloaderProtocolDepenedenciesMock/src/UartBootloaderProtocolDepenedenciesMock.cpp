extern "C" {
#include "UartBootloaderProtocolDepenedencies.h"
}
#include "CppUTestExt/MockSupport.h"

extern "C" void HandleNackForTransmission(void)
{
    mock().actualCall("HandleNackForTransmission");
}