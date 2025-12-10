#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C" {
    #include "UartBootloaderProtocolDepenedencies.h"
}

TEST_GROUP(BootloaderProtocolDepeTest)
{
    
};

IGNORE_TEST(BootloaderProtocolDepeTest, HandleNACK)
{
    //Read byte NACK => Is not in process
    uint8_t fake_received_data[2] = {0x00, 0xEF};
    uint8_t expect_transmitted_data[64] = {0x01, NACK};
    memcpy(ReceivedDataFromHost, fake_received_data, 2);
    CHECK_EQUAL(NOT_IN_PROCESS, IsInProcessCommand());
    
    HandleNackForTransmission();
    MEMCMP_EQUAL(expect_transmitted_data, TransmittedDataToHost, 2);
}

