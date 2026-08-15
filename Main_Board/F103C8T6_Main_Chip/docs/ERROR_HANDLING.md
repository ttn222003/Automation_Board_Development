# Error handling

## Core
### PLC_Main
- Does not support now

### Protocol
typedef enum {
    NACK_ERR_CRC_FAIL = 0x01,
    NACK_ERR_UNKNOWN_CMD,
    NACK_ERR_INVALID_PAYLOAD_LEN,
    NACK_ERR_PAYLOAD_TOO_LARGE,
    NACK_ERR_MALFORMED_FRAME,
    NACK_ERR_FRAME_INCOMPLETE,
} NackErrorCode_t;

## Depe
### GPIO
- Does not support now

### UART
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