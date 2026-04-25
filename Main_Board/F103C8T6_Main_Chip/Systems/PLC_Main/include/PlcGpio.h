/*
 * PlcGpio.h
 *
 *  Created on: Apr 19, 2026
 *      Author: Nhan Truong
 */

#ifndef PLC_MAIN_INCLUDE_PLCGPIO_H_
#define PLC_MAIN_INCLUDE_PLCGPIO_H_

#include "main.h"
#include "PlcConfig.h"

typedef enum {
	ePlcStateStop = 0
	, ePlcStateRun
} PlcState_t;

// Main function for PLC task
void PLC_Core_Init();
void PLC_ReadInputs();
void PLC_UpdateOutputs();
void PLC_Core_SetState(PlcState_t state);

// Support function
uint16_t ReadOutputImage();

#endif /* PLC_MAIN_INCLUDE_PLCGPIO_H_ */
