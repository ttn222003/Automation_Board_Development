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
    uint16_t mCurrentNumberOfBytes;
    BspUartStatus_t mCurrentStatus;
    uint8_t mOverflowFlag;
} BspUart_t;

static BspUart_t sBspUart;

/*======= Internal function =======*/

static uint8_t IsBufferEmpty(void)
{
    for (uint16_t index = 0; index < BSP_UART_RX_RING_BUF_SIZE; index++) {
        if (sBspUart.mDataBuffer[index] != 0) {
            return 1;
        }
    }
    return 0;
}

/*=================================*/

void ResetBspUartState(void)
{
    memset(sBspUart.mDataBuffer, 0, sizeof(sBspUart.mDataBuffer));
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
    sBspUart.mCurrentNumberOfBytes = 0;
    sBspUart.mCurrentStatus = BSP_UART_OK;
    sBspUart.mOverflowFlag = 0;

    return BSP_UART_OK;
}

BspUartStatus_t SendBspUart(UART_HandleTypeDef *huart, const uint8_t *data, uint16_t len)
{
	if(data == NULL) {
		sBspUart.mCurrentStatus = BSP_UART_ERR_NULL_PTR;
		return BSP_UART_ERR_NULL_PTR;
	}

    if (sBspUart.mCurrentStatus != BSP_UART_OK) {
        sBspUart.mCurrentStatus = BSP_UART_ERR_NOT_INIT;
        return BSP_UART_ERR_NOT_INIT;
    }

    if (len == 0) {
    	sBspUart.mCurrentStatus = BSP_UART_ERR_ZERO_LEN;
    	return BSP_UART_ERR_ZERO_LEN;
    }

    HAL_StatusTypeDef uart_status = HAL_UART_Transmit(huart, data, len, BSP_UART_TX_TIMEOUT_MS);
    if ((uart_status == HAL_ERROR) || (uart_status == HAL_BUSY)) {
    	sBspUart.mCurrentStatus = BSP_UART_ERR_HAL;
    	return BSP_UART_ERR_HAL;
    }

    return BSP_UART_OK;
}

void HandleBspUartIsrRx(uint8_t byte)
{
    sBspUart.mDataBuffer[sBspUart.mCurrentNumberOfBytes] = byte;

    sBspUart.mCurrentNumberOfBytes += 1;

    if (sBspUart.mCurrentNumberOfBytes > BSP_UART_RX_RING_BUF_SIZE) {
        sBspUart.mOverflowFlag = 1;
    }
}

uint16_t GetBspUartAvailable(void)
{
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

    if ((!IsBufferEmpty()) && (sBspUart.mCurrentNumberOfBytes == 0)) {
        sBspUart.mCurrentStatus = BSP_UART_ERR_NO_DATA;
        *out_read = 0;
        return BSP_UART_ERR_NO_DATA;
    }

    *out_read = sBspUart.mCurrentNumberOfBytes;

    uint16_t the_number_of_bytes_to_push_in_buffer = len;

    if (len > sBspUart.mCurrentNumberOfBytes) {
        the_number_of_bytes_to_push_in_buffer = sBspUart.mCurrentNumberOfBytes;
    }

    for (uint16_t index = 0; index < the_number_of_bytes_to_push_in_buffer; index++) {
        buf[index] = (uint8_t)sBspUart.mDataBuffer[index];
        sBspUart.mCurrentNumberOfBytes -= 1;
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
