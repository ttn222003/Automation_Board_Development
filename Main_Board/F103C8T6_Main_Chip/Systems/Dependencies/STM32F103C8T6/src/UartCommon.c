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

BspUartStatus_t InitBspUart(UART_HandleTypeDef *huart)
{
    if (huart == NULL) {
        sBspUart.mCurrentStatus = BSP_UART_ERR_NULL_PTR;
        return BSP_UART_ERR_NULL_PTR;
    }

    memset(sBspUart.mDataBuffer, 0, sizeof(sBspUart.mDataBuffer));
    sBspUart.mHeadIndex = 0;
    sBspUart.mTailIndex = 0;
    sBspUart.mCurrentNumberOfBytes = 0;
    sBspUart.mCurrentStatus = BSP_UART_OK;
    sBspUart.mOverflowFlag = 0;

    return BSP_UART_OK;
}

BspUartStatus_t SendBspUart(UART_HandleTypeDef *huart, const uint8_t *data, uint16_t len)
{
	if(data == NULL) {
		return BSP_UART_ERR_NULL_PTR;
	}

    // Same as bug found at test case 2.6? Check again all same bug for the src
    if (sBspUart.mCurrentStatus != BSP_UART_OK) {
        sBspUart.mCurrentStatus = BSP_UART_ERR_NOT_INIT;
        return BSP_UART_ERR_NOT_INIT;
    }

    if (len == 0) {
    	return BSP_UART_ERR_ZERO_LEN;
    }

    HAL_StatusTypeDef uart_status = HAL_UART_Transmit(huart, data, len, BSP_UART_TX_TIMEOUT_MS);
    if ((uart_status == HAL_ERROR) || (uart_status == HAL_BUSY)) {
    	return BSP_UART_ERR_HAL;
    }

    return BSP_UART_OK;
}

void HandleBspUartIsrRx(uint8_t byte)
{
    // If use ring buffer, then does check overflow neccesary?
    sBspUart.mCurrentNumberOfBytes += 1;

    if (sBspUart.mCurrentNumberOfBytes > BSP_UART_RX_RING_BUF_SIZE) {
        sBspUart.mOverflowFlag = 1;

        return;
    }

    sBspUart.mDataBuffer[sBspUart.mHeadIndex] = byte;
    sBspUart.mHeadIndex = (sBspUart.mHeadIndex + 1) % BSP_UART_RX_RING_BUF_SIZE;
}

uint16_t GetBspUartAvailable(void)
{
    // If use ring buffer, then does check overflow neccesary?
    if (sBspUart.mCurrentNumberOfBytes > BSP_UART_RX_RING_BUF_SIZE) {
        return BSP_UART_RX_RING_BUF_SIZE;
    }

    return sBspUart.mCurrentNumberOfBytes;
}

BspUartStatus_t ReadBspUart(uint8_t *buf, uint16_t len, uint16_t *out_read)
{
    if(buf == NULL) {
        sBspUart.mCurrentStatus = BSP_UART_ERR_NULL_PTR;
        return BSP_UART_ERR_NULL_PTR;
    }

    if (sBspUart.mCurrentNumberOfBytes == 0) {
        sBspUart.mCurrentStatus = BSP_UART_ERR_NO_DATA;
        *out_read = 0;
        return BSP_UART_ERR_NO_DATA;
    }

    uint16_t the_number_of_bytes_to_push_in_buffer = len;
    *out_read = len;

    if (len > sBspUart.mCurrentNumberOfBytes) {
        the_number_of_bytes_to_push_in_buffer = sBspUart.mCurrentNumberOfBytes;
        *out_read = sBspUart.mCurrentNumberOfBytes;
    }

    if (sBspUart.mCurrentNumberOfBytes > BSP_UART_RX_RING_BUF_SIZE) {
        *out_read = BSP_UART_RX_RING_BUF_SIZE;
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
