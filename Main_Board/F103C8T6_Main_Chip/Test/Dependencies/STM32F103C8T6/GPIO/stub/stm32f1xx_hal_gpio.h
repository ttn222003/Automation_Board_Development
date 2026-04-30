/*
 * stm32f1xx_hal_gpio.h  -  TEST STUB
 *
 * Replaces the STM32F1xx_HAL_Driver/Inc/stm32f1xx_hal_gpio.h header in the test build.
 *   - Provide ONLY the types and declarations actually used by:
 *       GpioCommon.c / GpioCommon.h
 *       Mockstm32f1xx_hal_gpio.h / Mockstm32f1xx_hal_gpio.c
 *
 */

#ifndef STM32F1XX_HAL_GPIO_STUB_H_
#define STM32F1XX_HAL_GPIO_STUB_H_

#include <stdint.h>

/* ── GPIO pin bit-masks ─────────────────────────────────────────────────────
 * Must match the values stored in Output.mUpdatingOutput[] by GpioCommon().
 * GpioCommon() assigns GPIO_PIN_0..GPIO_PIN_5 to indices 0..5.
 * CMock compares them as uint16_t via UNITY_TEST_ASSERT_EQUAL_HEX16.
 * ────────────────────────────────────────────────────────────────────────── */
#define GPIO_PIN_0    ((uint16_t)0x0001U)
#define GPIO_PIN_1    ((uint16_t)0x0002U)
#define GPIO_PIN_2    ((uint16_t)0x0004U)
#define GPIO_PIN_3    ((uint16_t)0x0008U)
#define GPIO_PIN_4    ((uint16_t)0x0010U)
#define GPIO_PIN_5    ((uint16_t)0x0020U)
#define GPIO_PIN_6    ((uint16_t)0x0040U)
#define GPIO_PIN_7    ((uint16_t)0x0080U)
#define GPIO_PIN_8    ((uint16_t)0x0100U)
#define GPIO_PIN_9    ((uint16_t)0x0200U)
#define GPIO_PIN_10   ((uint16_t)0x0400U)
#define GPIO_PIN_11   ((uint16_t)0x0800U)
#define GPIO_PIN_12   ((uint16_t)0x1000U)
#define GPIO_PIN_13   ((uint16_t)0x2000U)
#define GPIO_PIN_14   ((uint16_t)0x4000U)
#define GPIO_PIN_15   ((uint16_t)0x8000U)
#define GPIO_PIN_All  ((uint16_t)0xFFFFU)


typedef enum
{
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET   = 1
} GPIO_PinState;


typedef enum
{
    HAL_OK      = 0x00U,
    HAL_ERROR   = 0x01U,
    HAL_BUSY    = 0x02U,
    HAL_TIMEOUT = 0x03U
} HAL_StatusTypeDef;


typedef struct
{
    uint32_t CRL;   /* 0x00 - Configuration register low  */
    uint32_t CRH;   /* 0x04 - Configuration register high */
    uint32_t IDR;   /* 0x08 - Input data register         */
    uint32_t ODR;   /* 0x0C - Output data register        */  /* ← used by BSP_GPIO_Init */
    uint32_t BSRR;  /* 0x10 - Bit set/reset register      */
    uint32_t BRR;   /* 0x14 - Bit reset register          */
    uint32_t LCKR;  /* 0x18 - Configuration lock register */
} GPIO_TypeDef;


typedef struct
{
    uint32_t Pin;
    uint32_t Mode;
    uint32_t Pull;
    uint32_t Speed;
} GPIO_InitTypeDef;


extern GPIO_TypeDef *GPIOA;


void              HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init);
void              HAL_GPIO_DeInit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
GPIO_PinState     HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void              HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void              HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
HAL_StatusTypeDef HAL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void              HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin);
void              HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif /* STM32F1XX_HAL_GPIO_STUB_H_ */