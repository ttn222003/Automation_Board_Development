#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C" {
    #include "UartBootloaderProtocolDepenedencies.h"
}

TEST_GROUP(BootloaderProtocolDepeTestMock)
{
    void teardown() {
        mock().checkExpectations();
        mock().clear();
    }
};

TEST(BootloaderProtocolDepeTestMock, NotInProcessStatusAndFrameIsNotCorrected)
{
    mock().expectOneCall("HandleNackForTransmission");
    HandleNackForTransmission();
}