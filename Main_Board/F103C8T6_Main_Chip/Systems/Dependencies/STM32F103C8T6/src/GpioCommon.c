/*
 * GpioCommon.c
 *
 *  Created on: Apr 19, 2026
 *      Author: Nhan Truong
 */

#include "GpioCommon.h"

/* This define image of IO base on version and product */
#define V1		1

#ifdef V1
#define OUTPUT_NUMBER	6

//#define OUT0	OutputImage[0]
//#define	OUT1	OutputImage[1]
//#define OUT2	OutputImage[2]
//#define	OUT3	OutputImage[3]
//#define OUT4	OutputImage[4]
//#define	OUT5	OutputImage[5]

#endif

typedef struct
{
	uint8_t mNumberOfOutput;
	uint16_t mOutputImage[OUTPUT_NUMBER];
} Output_t;

static Output_t mOutput;

int8_t GpioCommon()
{
	mOutput.mNumberOfOutput = OUTPUT_NUMBER;

	mOutput.mOutputImage[0] = GPIO_PIN_0;
	mOutput.mOutputImage[1] = GPIO_PIN_1;
	mOutput.mOutputImage[2] = GPIO_PIN_2;
	mOutput.mOutputImage[3] = GPIO_PIN_3;
	mOutput.mOutputImage[4] = GPIO_PIN_4;
	mOutput.mOutputImage[5] = GPIO_PIN_5;

	return 1;
}

void BSP_GPIO_Init()
{
	// Initialize Output
	uint32_t odr_register = GPIOA->ODR;

	for (uint8_t index = 0; index < OUTPUT_NUMBER; index++)
	{
		HAL_GPIO_WritePin(GPIOA, mOutput.mOutputImage[index], (odr_register >> index) & 0x01);
	}
}

void BSP_WriteOutputs()
{

}

void BSP_FeedWatchdog()
{

}
