/*
 * PlcGpio.c
 *
 *  Created on: Apr 19, 2026
 *      Author: Nhan Truong
 */

#include "PlcGpio.h"

#define V1		1

#ifdef V1
#define OUTPUT_NUMBER	6

#endif

typedef struct {
	uint8_t mNumberOfOutputImage;
	uint16_t mOutputImage[OUTPUT_NUMBER];
} OutputImage_t;

static OutputImage_t OutputImage;

void PLC_Core_Init()
{
	// Assign external variale to eternal variable
	OutputImage.mNumberOfOutputImage = OUTPUT_NUMBER;

	OutputImage.mOutputImage[0] = OUT0;
	OutputImage.mOutputImage[1] = OUT1;
	OutputImage.mOutputImage[2] = OUT2;
	OutputImage.mOutputImage[3] = OUT3;
	OutputImage.mOutputImage[4] = OUT4;
	OutputImage.mOutputImage[5] = OUT5;
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
	uint16_t ret_val = 	OutputImage.mOutputImage[0] & \
						(OutputImage.mOutputImage[1] << 1) & \
						(OutputImage.mOutputImage[2] << 2) & \
						(OutputImage.mOutputImage[3] << 3) & \
						(OutputImage.mOutputImage[4] << 4) & \
						(OutputImage.mOutputImage[5] << 5);

	return ret_val;
}
