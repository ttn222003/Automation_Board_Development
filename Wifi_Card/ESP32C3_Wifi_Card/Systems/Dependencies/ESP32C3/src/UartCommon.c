/*
 * UartCommon.c
 *
 *  Created on: Apr 30, 2026
 *      Author: ASUS
 */


#include "UartCommon.h"

typedef struct
{
    uint8_t mDataBuffer[BSP_UART_RX_RING_BUF_SIZE];
    uint16_t mHeadIndex;
    uint16_t mTailIndex;
    uint16_t mCurrentNumberOfBytes;
    BspUartStatus_t mCurrentStatus;
    uint8_t mOverflowFlag;
} BspUart_t;

static BspUart_t sBspUart;

/*======= Internal function =======*/



/*=================================*/

void ResetBspUartState(void)
{
    memset(sBspUart.mDataBuffer, 0, sizeof(sBspUart.mDataBuffer));
    sBspUart.mHeadIndex = 0;
    sBspUart.mTailIndex = 0;
    sBspUart.mCurrentNumberOfBytes = 0;
    sBspUart.mCurrentStatus = BSP_UART_ERR_NOT_INIT;
}

BspUartStatus_t InitBspUart(int status_driver_install, int status_param_config)
{
    if (status_driver_install != ESP_OK || status_param_config != ESP_OK) {
        sBspUart.mCurrentStatus = BSP_UART_ERR_NOT_INIT;
        return BSP_UART_ERR_NOT_INIT;
    }

    memset(sBspUart.mDataBuffer, 0, sizeof(sBspUart.mDataBuffer));
    sBspUart.mHeadIndex = 0;
    sBspUart.mTailIndex = 0;
    sBspUart.mCurrentNumberOfBytes = 0;
    sBspUart.mCurrentStatus = BSP_UART_OK;
    sBspUart.mOverflowFlag = 0;

    return BSP_UART_OK;
}

BspUartStatus_t SendBspUart(const uint8_t *data, uint16_t len)
{
    if(data == NULL) {
		return BSP_UART_ERR_NULL_PTR;
	}
    
    if (sBspUart.mCurrentStatus != BSP_UART_OK) {
        sBspUart.mCurrentStatus = BSP_UART_ERR_NOT_INIT;
        return BSP_UART_ERR_NOT_INIT;
    }

    if (len == 0) {
    	return BSP_UART_ERR_ZERO_LEN;
    }

    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    
    if (txBytes < 0) {
        sBspUart.mCurrentStatus = BSP_UART_ERR_HAL;
    	return BSP_UART_ERR_HAL;
    }

    return BSP_UART_OK;
}

void HandleBspUartIsrRx(uint8_t byte)
{    
    if (sBspUart.mCurrentNumberOfBytes >= BSP_UART_RX_RING_BUF_SIZE) {
        sBspUart.mOverflowFlag = 1;

        return;
    }

    sBspUart.mCurrentNumberOfBytes += 1;

    sBspUart.mDataBuffer[sBspUart.mHeadIndex] = byte;
    sBspUart.mHeadIndex = (sBspUart.mHeadIndex + 1) % BSP_UART_RX_RING_BUF_SIZE;
}

uint16_t GetBspUartAvailable(void)
{
    return sBspUart.mCurrentNumberOfBytes;
}

BspUartStatus_t ReadBspUart(uint8_t *buf, uint16_t len, uint16_t *out_read)
{
    if(buf == NULL) {
        return BSP_UART_ERR_NULL_PTR;
    }

    if (sBspUart.mCurrentNumberOfBytes == 0) {
        *out_read = 0;
        return BSP_UART_ERR_NO_DATA;
    }

    uint16_t the_number_of_bytes_to_push_in_buffer = len;
    *out_read = len;

    if (len > sBspUart.mCurrentNumberOfBytes) {
        the_number_of_bytes_to_push_in_buffer = sBspUart.mCurrentNumberOfBytes;
        *out_read = sBspUart.mCurrentNumberOfBytes;
    }

    uint16_t buffer_index = 0;

    while (the_number_of_bytes_to_push_in_buffer > 0) {
        buf[buffer_index++] = (uint8_t)sBspUart.mDataBuffer[sBspUart.mTailIndex];
        sBspUart.mTailIndex = (sBspUart.mTailIndex + 1) % BSP_UART_RX_RING_BUF_SIZE;
        sBspUart.mCurrentNumberOfBytes--;
        the_number_of_bytes_to_push_in_buffer--;
    }

    return BSP_UART_OK;
}

uint8_t IsBspUartOverflow(void)
{
    if (sBspUart.mOverflowFlag == 1) {
        sBspUart.mOverflowFlag = 0;
        return 1;
    }

    return 0;
}
