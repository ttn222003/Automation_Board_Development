/*
 * GpioCommon.c
 *
 *  Created on: Apr 19, 2026
 *      Author: Nhan Truong
 */

#include "GpioCommon.h"

/* This define image of IO base on version and product */

typedef enum {
	emOUTPUTS_OK				= 1,
	emOUTPUTS_ERR_UNINITALIZED	= -1,
} eInternalOutputStatus;

typedef struct
{
	uint8_t mNumberOfUpdatingOutput;
	uint16_t mUpdatingOutput[OUTPUT_NUMBER];
} Output_t;


static Output_t mOutput;
static eInternalOutputStatus mOutputStatus = emOUTPUTS_ERR_UNINITALIZED;


/*======= API =======*/
int8_t InitBspInternalOutputs(void)
{
	mOutput.mNumberOfUpdatingOutput = OUTPUT_NUMBER;

	mOutput.mUpdatingOutput[0] = GPIO_PIN_0;
	mOutput.mUpdatingOutput[1] = GPIO_PIN_1;
	mOutput.mUpdatingOutput[2] = GPIO_PIN_2;
	mOutput.mUpdatingOutput[3] = GPIO_PIN_3;
	mOutput.mUpdatingOutput[4] = GPIO_PIN_4;
	mOutput.mUpdatingOutput[5] = GPIO_PIN_5;

	mOutputStatus = emOUTPUTS_OK;

	return emOUTPUTS_OK;
}

int8_t DeinitBspInternalOutputs(void)
{
	mOutput.mNumberOfUpdatingOutput = 0;

	mOutput.mUpdatingOutput[0] = 0;
	mOutput.mUpdatingOutput[1] = 0;
	mOutput.mUpdatingOutput[2] = 0;
	mOutput.mUpdatingOutput[3] = 0;
	mOutput.mUpdatingOutput[4] = 0;
	mOutput.mUpdatingOutput[5] = 0;

	mOutputStatus = emOUTPUTS_ERR_UNINITALIZED;

	return emOUTPUTS_ERR_UNINITALIZED;
}

eBspGpioStatus InitBspGpio(void)
{
	if (mOutputStatus == emOUTPUTS_ERR_UNINITALIZED) {
		return BSP_GPIO_ERR_UNINITALIZED_INTERNAL_OUTPUTS;
	}

	uint32_t odr_register = GPIOA->ODR;

	for (uint8_t index = 0; index < mOutput.mNumberOfUpdatingOutput; index++)
	{
		HAL_GPIO_WritePin(GPIOA, mOutput.mUpdatingOutput[index], (GPIO_PinState)(odr_register >> index) & 0x01);
	}

	return BSP_GPIO_INIT_OK;
}

eBspGpioStatus WriteBspGpioOutputs(uint16_t output_image)
{
	if (mOutputStatus == emOUTPUTS_ERR_UNINITALIZED) {
		return BSP_GPIO_ERR_UNINITALIZED_INTERNAL_OUTPUTS;
	}

	for (uint8_t index = 0; index < mOutput.mNumberOfUpdatingOutput; index++)
	{
		HAL_GPIO_WritePin(GPIOA, mOutput.mUpdatingOutput[index], (GPIO_PinState)(output_image >> index) & 0x01);
	}

	if (output_image > (uint16_t)(~(0xFFFF << mOutput.mNumberOfUpdatingOutput))) {
		return BSP_GPIO_WARNING_OUTPUTS_OUT_RANGE;
	}

	return BSP_GPIO_WRITE_OK;
}

// Suspend this function after handle communication and will put it to another file related to WDG, so don't need to review
void BSP_FeedWatchdog(void)
{

}
