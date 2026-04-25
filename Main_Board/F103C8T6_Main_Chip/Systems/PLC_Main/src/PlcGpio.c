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
} OutputImage_t;

static OutputImage_t OutputImage;

void PLC_Core_Init()
{
	// Assign external variale to eternal variable
	OutputImage.mNumberOfOutputImage = OUTPUT_NUMBER;

	for (uint16_t index = 0; index < OutputImage.mNumberOfOutputImage; index++)
	{
		OutputImage.mOutputImage[index] = 0;
	}
}

void PLC_ReadInputs()
{

}

void PLC_UpdateOutputs()
{

}

void PLC_Core_SetState(PlcState_t state)
{

}

/**********************/
/*
 * This function is used to read output value after updating IO image from Execution Phase
 * */
uint16_t ReadOutputImage()
{
	uint16_t ret_val = 	OutputImage.mOutputImage[0];

	for (uint8_t index = 1; index < OutputImage.mNumberOfOutputImage; index++)
	{
		ret_val |= OutputImage.mOutputImage[index] << index;
	}

	return ret_val;
}
