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

typedef enum {
	BSP_GPIO_INIT_OK							= 1,
	BSP_GPIO_WRITE_OK							= 2,
	BSP_GPIO_WARNING_OUTPUTS_OUT_RANGE			= 3,
	BSP_GPIO_ERR_UNINITALIZED_INTERNAL_OUTPUTS	= -1,
} eBspGpioStatus;

int8_t InitBspInternalOutputs(void);
int8_t DeinitBspInternalOutputs(void);
eBspGpioStatus InitBspGpio(void);
eBspGpioStatus WriteBspGpioOutputs(uint16_t output_image);
void BSP_FeedWatchdog(void);


#endif /* DEPENDENCIES_STM32F103C8T6_INCLUDE_GPIOCOMMON_H_ */
