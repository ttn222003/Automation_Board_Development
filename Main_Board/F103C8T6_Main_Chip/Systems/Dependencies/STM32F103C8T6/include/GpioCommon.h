/*
 * GpioCommon.h
 *
 *  Created on: Apr 19, 2026
 *      Author: Nhan Truong
 */

#ifndef DEPENDENCIES_STM32F103C8T6_INCLUDE_GPIOCOMMON_H_
#define DEPENDENCIES_STM32F103C8T6_INCLUDE_GPIOCOMMON_H_

#include "main.h"
#include "PlcConfig.h"

int8_t GpioCommon(void);
void BSP_GPIO_Init(void);
void BSP_WriteOutputs(uint16_t output_image);
void BSP_FeedWatchdog(void);


#endif /* DEPENDENCIES_STM32F103C8T6_INCLUDE_GPIOCOMMON_H_ */
