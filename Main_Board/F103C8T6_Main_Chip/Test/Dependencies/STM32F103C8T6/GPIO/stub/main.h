/*
 * main.h  -  TEST STUB
 * Replaces the STM32CubeIDE-generated main.h in the test build.
 *
 * None of that compiles on a host PC without the STM32F103xB define and
 * the full CMSIS tree.  This stub short-circuits the chain by including
 * only our minimal stm32f1xx_hal_gpio.h stub which provides every type
 * and declaration that GpioCommon actually needs.
 *
 * Rule: add an include here ONLY if a new BSP module needs a type that is
 * not already covered by stm32f1xx_hal_gpio.h.
 */

#ifndef MAIN_STUB_H_
#define MAIN_STUB_H_

#include "stm32f1xx_hal_gpio.h"

#endif /* MAIN_STUB_H_ */