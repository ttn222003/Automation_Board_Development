#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "cmock.h"
#include "MockTimerDriver.h"


static struct MockTimerDriverInstance
{
  unsigned char placeHolder;
} Mock;


void MockTimerDriver_Verify(void)
{
}

void MockTimerDriver_Init(void)
{
  MockTimerDriver_Destroy();
}

void MockTimerDriver_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

