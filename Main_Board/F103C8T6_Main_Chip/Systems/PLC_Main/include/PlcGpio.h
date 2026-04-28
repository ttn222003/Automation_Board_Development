/*
 * PlcGpio.h
 *
 *  Created on: Apr 19, 2026
 *      Author: Nhan Truong
 */

#ifndef PLC_MAIN_INCLUDE_PLCGPIO_H_
#define PLC_MAIN_INCLUDE_PLCGPIO_H_

#include <stdint.h>
#include "PlcConfig.h"

typedef enum {
	ePlcStateStop = 0
	, ePlcStateRun
} PlcState_t;

// Main function for PLC task
void InitPlcCore(void);
void ReadPlcInputs(void);
void UpdatePlcOutputs(uint16_t output_image[]);
void SetPlcState(PlcState_t state);
PlcState_t GetPlcState(void);

// Support function
uint16_t ReadOutputImage(void);

#endif /* PLC_MAIN_INCLUDE_PLCGPIO_H_ */
