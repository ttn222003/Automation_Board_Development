#include "unity.h"
#include "unity_color.h"
#include <string.h>
#include "UartBootloaderProtocolCoresHost.h"

uint8_t transmitt_data[64];

void setUp(void)
{
    memset(transmitt_data, 0, 64);
}

void tearDown(void)
{

}

void HandleHandshakeRequestOfGetCommandTest(void)
{
    uint8_t expected_data[8] = { REQUEST_HANDSHAKE, 0x08, GET_CMD, 0xFF - GET_CMD, 0x8A, 0xC2, 0xA9, 0xFA};
    HandleHandshakeRequestOfGetCommandForTransmission(transmitt_data);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_data, transmitt_data, 8);
}

int main(void)
{
    printf(COL_YELLOW "==== START CORE TEST ====\n" COL_RESET);
    UNITY_BEGIN();
    RUN_TEST(HandleHandshakeRequestOfGetCommandTest);
    printf(COL_YELLOW "==== END CORE TEST ====\n" COL_RESET);
    return UNITY_END();
}