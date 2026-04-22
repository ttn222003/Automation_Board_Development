/*
 * PlcGpio.h
 *
 *  Created on: Apr 19, 2026
 *      Author: Nhan Truong
 */

#ifndef PLC_MAIN_INCLUDE_PLCGPIO_H_
#define PLC_MAIN_INCLUDE_PLCGPIO_H_

#include "main.h"

#define PLC_SCAN_PERIOD_MS		10

#define OUT0	0
#define OUT1	1
#define OUT2	2
#define OUT3	3
#define OUT4	4
#define OUT5	5

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
