#include "UartBootloaderProtocolCore.h"

unsigned char CheckCommandCode(unsigned char received_data[])
{
    return received_data[0];
}