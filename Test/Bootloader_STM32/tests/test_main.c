#include "unity.h"

extern void TestUnity(void);

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(TestUnity);
    return UNITY_END();
}
