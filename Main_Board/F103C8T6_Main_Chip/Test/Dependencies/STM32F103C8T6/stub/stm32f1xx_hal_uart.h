/*
 * stm32f1xx_hal_uart.h  -  TEST STUB
 *
 * Replaces STM32F1xx_HAL_Driver/Inc/stm32f1xx_hal_uart.h in the test build.
 * Derived directly from the real STM32F1xx HAL header:
 *   - All types and enums are copied verbatim so sizeof() matches production.
 *   - Hardware-register-dependent macros (__HAL_UART_*, IS_UART_*) are
 *     omitted – they pull in the full CMSIS register tree which cannot
 *     compile on a host PC.
 *   - USE_HAL_UART_REGISTER_CALLBACKS is forced to 1 because the CMock
 *     file was generated with callback registration enabled.
 *   - Opaque stubs are provided for USART_TypeDef, DMA_HandleTypeDef and
 *     HAL_LockTypeDef so the handle struct compiles without CMSIS.
 *
 * Rule: HAL_StatusTypeDef is already defined in stm32f1xx_hal_gpio.h.
 * This file guards against redefining it so both stubs can coexist
 * when included together through main.h.
 *
 */

#ifndef STM32F1XX_HAL_UART_STUB_H_
#define STM32F1XX_HAL_UART_STUB_H_

#include <stdint.h>

/* ── __IO – maps to volatile on target, plain on host ───────────────────── */

#ifndef __IO
#define __IO volatile
#endif

/* ── HAL_StatusTypeDef – guard against redefinition from gpio stub ────────
 * stm32f1xx_hal_gpio.h defines this enum first when included via main.h.
 * ────────────────────────────────────────────────────────────────────────── */

#ifndef STM32F1XX_HAL_GPIO_STUB_H_
typedef enum
{
    HAL_OK      = 0x00U,
    HAL_ERROR   = 0x01U,
    HAL_BUSY    = 0x02U,
    HAL_TIMEOUT = 0x03U
} HAL_StatusTypeDef;
#endif

/* ── Opaque peripheral register struct stubs ─────────────────────────────── */

/*
 * On the target these are CMSIS structs that map directly onto hardware
 * registers.  For host tests we only need them to be valid pointer targets
 * so a minimal opaque struct is sufficient.
 */
typedef struct { uint32_t reserved; } USART_TypeDef;
typedef struct { uint32_t reserved; } DMA_HandleTypeDef;

/* ── HAL lock ────────────────────────────────────────────────────────────── */

typedef enum
{
    HAL_UNLOCKED = 0x00U,
    HAL_LOCKED   = 0x01U
} HAL_LockTypeDef;

/* ── Force callback registration ON (matches how CMock was generated) ─────── */

#ifndef USE_HAL_UART_REGISTER_CALLBACKS
#define USE_HAL_UART_REGISTER_CALLBACKS  1
#endif

/* ── UART init structure (verbatim from real header) ────────────────────── */

typedef struct
{
    uint32_t BaudRate;
    uint32_t WordLength;
    uint32_t StopBits;
    uint32_t Parity;
    uint32_t Mode;
    uint32_t HwFlowCtl;
    uint32_t OverSampling;
} UART_InitTypeDef;

/* ── UART state (verbatim from real header) ──────────────────────────────── */

typedef enum
{
    HAL_UART_STATE_RESET      = 0x00U,
    HAL_UART_STATE_READY      = 0x20U,
    HAL_UART_STATE_BUSY       = 0x24U,
    HAL_UART_STATE_BUSY_TX    = 0x21U,
    HAL_UART_STATE_BUSY_RX    = 0x22U,
    HAL_UART_STATE_BUSY_TX_RX = 0x23U,
    HAL_UART_STATE_TIMEOUT    = 0xA0U,
    HAL_UART_STATE_ERROR      = 0xE0U
} HAL_UART_StateTypeDef;

/* ── Reception type and Rx event type (verbatim – both are uint32_t) ─────── */

typedef uint32_t HAL_UART_RxTypeTypeDef;
typedef uint32_t HAL_UART_RxEventTypeTypeDef;

/* Reception type values */
#define HAL_UART_RECEPTION_STANDARD    0x00000000U
#define HAL_UART_RECEPTION_TOIDLE      0x00000001U

/* Rx event type values */
#define HAL_UART_RXEVENT_TC            0x00U
#define HAL_UART_RXEVENT_HT            0x01U
#define HAL_UART_RXEVENT_IDLE          0x02U

/* ── UART handle (verbatim from real header, CMSIS types replaced) ────────── */

typedef struct __UART_HandleTypeDef
{
    USART_TypeDef                    *Instance;
    UART_InitTypeDef                  Init;
    const uint8_t                    *pTxBuffPtr;
    uint16_t                          TxXferSize;
    __IO uint16_t                     TxXferCount;
    uint8_t                          *pRxBuffPtr;
    uint16_t                          RxXferSize;
    __IO uint16_t                     RxXferCount;
    __IO HAL_UART_RxTypeTypeDef       ReceptionType;
    __IO HAL_UART_RxEventTypeTypeDef  RxEventType;
    DMA_HandleTypeDef                *hdmatx;
    DMA_HandleTypeDef                *hdmarx;
    HAL_LockTypeDef                   Lock;
    __IO HAL_UART_StateTypeDef        gState;
    __IO HAL_UART_StateTypeDef        RxState;
    __IO uint32_t                     ErrorCode;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
    void (* TxHalfCpltCallback)(struct __UART_HandleTypeDef *huart);
    void (* TxCpltCallback)(struct __UART_HandleTypeDef *huart);
    void (* RxHalfCpltCallback)(struct __UART_HandleTypeDef *huart);
    void (* RxCpltCallback)(struct __UART_HandleTypeDef *huart);
    void (* ErrorCallback)(struct __UART_HandleTypeDef *huart);
    void (* AbortCpltCallback)(struct __UART_HandleTypeDef *huart);
    void (* AbortTransmitCpltCallback)(struct __UART_HandleTypeDef *huart);
    void (* AbortReceiveCpltCallback)(struct __UART_HandleTypeDef *huart);
    void (* WakeupCallback)(struct __UART_HandleTypeDef *huart);
    void (* RxEventCallback)(struct __UART_HandleTypeDef *huart, uint16_t Pos);
    void (* MspInitCallback)(struct __UART_HandleTypeDef *huart);
    void (* MspDeInitCallback)(struct __UART_HandleTypeDef *huart);
#endif
} UART_HandleTypeDef;

/* ── Callback ID enum (verbatim, guarded same as real header) ─────────────── */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
typedef enum
{
    HAL_UART_TX_HALFCOMPLETE_CB_ID         = 0x00U,
    HAL_UART_TX_COMPLETE_CB_ID             = 0x01U,
    HAL_UART_RX_HALFCOMPLETE_CB_ID         = 0x02U,
    HAL_UART_RX_COMPLETE_CB_ID             = 0x03U,
    HAL_UART_ERROR_CB_ID                   = 0x04U,
    HAL_UART_ABORT_COMPLETE_CB_ID          = 0x05U,
    HAL_UART_ABORT_TRANSMIT_COMPLETE_CB_ID = 0x06U,
    HAL_UART_ABORT_RECEIVE_COMPLETE_CB_ID  = 0x07U,
    HAL_UART_WAKEUP_CB_ID                  = 0x08U,
    HAL_UART_MSPINIT_CB_ID                 = 0x0BU,
    HAL_UART_MSPDEINIT_CB_ID               = 0x0CU
} HAL_UART_CallbackIDTypeDef;

typedef void (*pUART_CallbackTypeDef)(UART_HandleTypeDef *huart);
typedef void (*pUART_RxEventCallbackTypeDef)(struct __UART_HandleTypeDef *huart, uint16_t Pos);
#endif

/* ── Error codes ─────────────────────────────────────────────────────────── */

#define HAL_UART_ERROR_NONE              0x00000000U
#define HAL_UART_ERROR_PE                0x00000001U
#define HAL_UART_ERROR_NE                0x00000002U
#define HAL_UART_ERROR_FE                0x00000004U
#define HAL_UART_ERROR_ORE               0x00000008U
#define HAL_UART_ERROR_DMA               0x00000010U
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
#define HAL_UART_ERROR_INVALID_CALLBACK  0x00000020U
#endif

/* ── Init parameter constants (numeric-only, safe for host build) ─────────── */

#define UART_WORDLENGTH_8B              0x00000000U
#define UART_WORDLENGTH_9B              0x00001000U   /* USART_CR1_M on F1 */

#define UART_STOPBITS_1                 0x00000000U
#define UART_STOPBITS_2                 0x00002000U   /* USART_CR2_STOP_1  */

#define UART_PARITY_NONE                0x00000000U
#define UART_PARITY_EVEN                0x00000400U   /* USART_CR1_PCE     */
#define UART_PARITY_ODD                 0x00000600U   /* PCE | PS          */

#define UART_HWCONTROL_NONE             0x00000000U
#define UART_HWCONTROL_RTS              0x00000100U   /* USART_CR3_RTSE    */
#define UART_HWCONTROL_CTS              0x00000200U   /* USART_CR3_CTSE    */
#define UART_HWCONTROL_RTS_CTS          0x00000300U

#define UART_MODE_RX                    0x00000004U   /* USART_CR1_RE      */
#define UART_MODE_TX                    0x00000008U   /* USART_CR1_TE      */
#define UART_MODE_TX_RX                 0x0000000CU

#define UART_OVERSAMPLING_16            0x00000000U

/* ── Function declarations (verbatim from real header) ───────────────────── */

/* Init / DeInit */
HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_HalfDuplex_Init(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_LIN_Init(UART_HandleTypeDef *huart, uint32_t BreakDetectLength);
HAL_StatusTypeDef HAL_MultiProcessor_Init(UART_HandleTypeDef *huart, uint8_t Address,
                                           uint32_t WakeUpMethod);
HAL_StatusTypeDef HAL_UART_DeInit(UART_HandleTypeDef *huart);
void              HAL_UART_MspInit(UART_HandleTypeDef *huart);
void              HAL_UART_MspDeInit(UART_HandleTypeDef *huart);

/* Callback registration */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
HAL_StatusTypeDef HAL_UART_RegisterCallback(UART_HandleTypeDef *huart,
                                             HAL_UART_CallbackIDTypeDef CallbackID,
                                             pUART_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_UART_UnRegisterCallback(UART_HandleTypeDef *huart,
                                               HAL_UART_CallbackIDTypeDef CallbackID);
HAL_StatusTypeDef HAL_UART_RegisterRxEventCallback(UART_HandleTypeDef *huart,
                                                    pUART_RxEventCallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_UART_UnRegisterRxEventCallback(UART_HandleTypeDef *huart);
#endif

/* Blocking IO */
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, const uint8_t *pData,
                                     uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData,
                                    uint16_t Size, uint32_t Timeout);

/* Interrupt IO */
HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, const uint8_t *pData,
                                        uint16_t Size);
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData,
                                       uint16_t Size);

/* DMA IO */
HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, const uint8_t *pData,
                                         uint16_t Size);
HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData,
                                        uint16_t Size);
HAL_StatusTypeDef HAL_UART_DMAPause(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_DMAResume(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_DMAStop(UART_HandleTypeDef *huart);

/* Extended – ReceiveToIdle */
HAL_StatusTypeDef HAL_UARTEx_ReceiveToIdle(UART_HandleTypeDef *huart, uint8_t *pData,
                                            uint16_t Size, uint16_t *RxLen, uint32_t Timeout);
HAL_StatusTypeDef HAL_UARTEx_ReceiveToIdle_IT(UART_HandleTypeDef *huart, uint8_t *pData,
                                               uint16_t Size);
HAL_StatusTypeDef HAL_UARTEx_ReceiveToIdle_DMA(UART_HandleTypeDef *huart, uint8_t *pData,
                                                uint16_t Size);
HAL_UART_RxEventTypeTypeDef HAL_UARTEx_GetRxEventType(UART_HandleTypeDef *huart);

/* Abort */
HAL_StatusTypeDef HAL_UART_Abort(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_AbortTransmit(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_AbortReceive(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_Abort_IT(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_AbortTransmit_IT(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_AbortReceive_IT(UART_HandleTypeDef *huart);

/* IRQ handler and callbacks */
void HAL_UART_IRQHandler(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);
void HAL_UART_AbortCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_AbortTransmitCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart);
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);

/* Peripheral control */
HAL_StatusTypeDef HAL_LIN_SendBreak(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_MultiProcessor_EnterMuteMode(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_MultiProcessor_ExitMuteMode(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_HalfDuplex_EnableTransmitter(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_HalfDuplex_EnableReceiver(UART_HandleTypeDef *huart);

/* Peripheral state */
HAL_UART_StateTypeDef HAL_UART_GetState(const UART_HandleTypeDef *huart);
uint32_t              HAL_UART_GetError(const UART_HandleTypeDef *huart);

/* Private functions exposed for mock (from real header's private section) */
HAL_StatusTypeDef UART_Start_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData,
                                         uint16_t Size);
HAL_StatusTypeDef UART_Start_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData,
                                          uint16_t Size);

#endif /* STM32F1XX_HAL_UART_STUB_H_ */