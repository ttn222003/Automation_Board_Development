#include "unity.h"
#include "unity_color.h"

extern void ParseFrameHandshakeRequestOfGetCommandTest(void);
extern void ParseFrameHandshakeRequestOfGetCommandButByteDataLengthIsWrongTest(void);
extern void HandleAckTest(void);
extern void HandleNackTest(void);
extern void ParseFrameDataRequestOfGetCommandTest(void);
extern void ParseFrameDataRequestOfGetCommandButNotBytesTest(void);
extern void HandleDataGetCmdTest(void);
extern void ParseFrameEndHandshakeOfGetCommandTest(void);

int main(void)
{
    printf(COL_YELLOW "==== START TEST ====\n" COL_RESET);
    UNITY_BEGIN();
    RUN_TEST(ParseFrameHandshakeRequestOfGetCommandTest);
    RUN_TEST(ParseFrameHandshakeRequestOfGetCommandButByteDataLengthIsWrongTest);
    RUN_TEST(HandleAckTest);
    RUN_TEST(HandleNackTest);
    RUN_TEST(ParseFrameDataRequestOfGetCommandTest);
    RUN_TEST(ParseFrameDataRequestOfGetCommandButNotBytesTest);
    RUN_TEST(HandleDataGetCmdTest);
    RUN_TEST(ParseFrameEndHandshakeOfGetCommandTest);
    printf(COL_YELLOW "==== END TEST ====\n" COL_RESET);
    return UNITY_END();
}
