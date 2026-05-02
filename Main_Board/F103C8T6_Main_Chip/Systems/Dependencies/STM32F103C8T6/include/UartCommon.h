/*
 * UartCommon.h
 *
 *  Created on: Apr 30, 2026
 *      Author: ASUS
 */

#ifndef DEPENDENCIES_STM32F103C8T6_INCLUDE_UARTCOMMON_H_
#define DEPENDENCIES_STM32F103C8T6_INCLUDE_UARTCOMMON_H_


#include "main.h"
#include <string.h>
#include "PlcConfig.h"

#define BSP_UART_RX_RING_BUF_SIZE   256U
#define BSP_UART_TX_TIMEOUT_MS      100U

typedef enum
{
    BSP_UART_OK           =  0,
    BSP_UART_ERR_HAL      = -1,   /* HAL returned an error          */
    BSP_UART_ERR_NULL_PTR = -2,   /* NULL pointer argument          */
    BSP_UART_ERR_ZERO_LEN = -3,   /* len = 0 passed to Send         */
    BSP_UART_ERR_BUF_FULL = -4,   /* ring buffer overflow           */
    BSP_UART_ERR_NO_DATA  = -5,   /* ring buffer empty on Read      */
    BSP_UART_ERR_NOT_INIT = -6,   /* InitBspUart not yet called     */
} BspUartStatus_t;

void ResetBspUartState(void);
BspUartStatus_t InitBspUart(UART_HandleTypeDef *huart);
BspUartStatus_t SendBspUart(UART_HandleTypeDef *huart, const uint8_t *data, uint16_t len);
void HandleBspUartIsrRx(uint8_t byte);
uint16_t GetBspUartAvailable(void);
BspUartStatus_t ReadBspUart(uint8_t *buf, uint16_t len, uint16_t *outRead);
uint8_t IsBspUartOverflow(void);

#endif /* DEPENDENCIES_STM32F103C8T6_INCLUDE_UARTCOMMON_H_ */
