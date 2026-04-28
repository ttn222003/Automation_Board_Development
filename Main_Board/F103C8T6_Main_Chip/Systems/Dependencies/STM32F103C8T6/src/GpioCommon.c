/*
 * GpioCommon.c
 *
 *  Created on: Apr 19, 2026
 *      Author: Nhan Truong
 */

#include "GpioCommon.h"

/* This define image of IO base on version and product */


typedef struct
{
	uint8_t mNumberOfUpdatingOutput;
	uint16_t mUpdatingOutput[OUTPUT_NUMBER];
} Output_t;

static Output_t Output;

int8_t GpioCommon(void)
{
	Output.mNumberOfUpdatingOutput = OUTPUT_NUMBER;

	Output.mUpdatingOutput[0] = GPIO_PIN_0;
	Output.mUpdatingOutput[1] = GPIO_PIN_1;
	Output.mUpdatingOutput[2] = GPIO_PIN_2;
	Output.mUpdatingOutput[3] = GPIO_PIN_3;
	Output.mUpdatingOutput[4] = GPIO_PIN_4;
	Output.mUpdatingOutput[5] = GPIO_PIN_5;

	return 1;
}

void BSP_GPIO_Init(void)
{
	uint32_t odr_register = GPIOA->ODR;

	for (uint8_t index = 0; index < Output.mNumberOfUpdatingOutput; index++)
	{
		HAL_GPIO_WritePin(GPIOA, Output.mUpdatingOutput[index], (GPIO_PinState)(odr_register >> index) & 0x01);
	}
}

void BSP_WriteOutputs(uint16_t output_image)
{
	for (uint8_t index = 0; index < Output.mNumberOfUpdatingOutput; index++)
	{
		HAL_GPIO_WritePin(GPIOA, Output.mUpdatingOutput[index], (GPIO_PinState)(output_image >> index) & 0x01);
	}
}

// Suspend this function after handle communication and will put it to another file related to WDG
void BSP_FeedWatchdog(void)
{

}
