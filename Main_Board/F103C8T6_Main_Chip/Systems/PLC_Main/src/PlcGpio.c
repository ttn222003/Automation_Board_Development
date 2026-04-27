/*
 * PlcGpio.c
 *
 *  Created on: Apr 19, 2026
 *      Author: Nhan Truong
 */

#include "PlcGpio.h"

typedef struct {
	uint8_t mNumberOfOutputImage;
	uint16_t mOutputImage[OUTPUT_NUMBER];
	PlcState_t mPlcState;
} OutputImage_t;

static OutputImage_t OutputImage;

/*======= Internal function =======*/
static uint8_t IsPlcRunning(void)
{
	if (OutputImage.mPlcState == ePlcStateRun) {
		return 1;
	}

	else {
		return 0;
	}
}

/*=================================*/
void InitPlcCore(void)
{
	// Assign external variale to eternal variable
	OutputImage.mNumberOfOutputImage = OUTPUT_NUMBER;

	for (uint16_t index = 0; index < OutputImage.mNumberOfOutputImage; index++)
	{
		OutputImage.mOutputImage[index] = 0;
	}

	OutputImage.mPlcState = ePlcStateStop;
}

void ReadPlcInputs(void)
{
	/* TO DO */
}

void UpdatePlcOutputs(uint16_t output_image[])
{
	if (IsPlcRunning() != 1)
	{
		// We will handle error later
		return;
	}

	for (uint8_t index = 0; index < OutputImage.mNumberOfOutputImage; index++)
	{
		OutputImage.mOutputImage[index] = output_image[index];
	}
}

void SetPlcState(PlcState_t state)
{
	OutputImage.mPlcState = state;
}

PlcState_t GetPlcState(void)
{
	return OutputImage.mPlcState;
}

/**********************/
/*
 * This function is used to read output value after updating IO image from Execution Phase
 * */
uint16_t ReadOutputImage(void)
{
	uint16_t ret_val = 0;

	for (uint8_t index = 0; index < OutputImage.mNumberOfOutputImage; index++)
	{
		ret_val |= (uint16_t)((OutputImage.mOutputImage[index] & 0x01) << index);
	}

	return ret_val;
}
