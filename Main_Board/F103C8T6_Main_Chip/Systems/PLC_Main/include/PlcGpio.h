/*
 * PlcGpio.h
 *
 *  Created on: Apr 19, 2026
 *      Author: Nhan Truong
 */

#ifndef PLC_MAIN_INCLUDE_PLCGPIO_H_
#define PLC_MAIN_INCLUDE_PLCGPIO_H_

#define PLC_SCAN_PERIOD_MS		10

typedef enum {
	ePlcStateStop = 0
	, ePlcStateRun
} PlcState_t;

void PLC_Core_Init();
void PLC_ReadInputs();
void PLC_UpdateOutputs();
void PLC_Core_SetState(PlcState_t state);


#endif /* PLC_MAIN_INCLUDE_PLCGPIO_H_ */
