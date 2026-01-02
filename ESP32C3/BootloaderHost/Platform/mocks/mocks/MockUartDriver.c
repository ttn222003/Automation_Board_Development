#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "cmock.h"
#include "MockUartDriver.h"


static struct MockUartDriverInstance
{
  unsigned char placeHolder;
} Mock;


void MockUartDriver_Verify(void)
{
}

void MockUartDriver_Init(void)
{
  MockUartDriver_Destroy();
}

void MockUartDriver_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

