/*
 * PlcMainExecuteLogic.h
 *
 *  Created on: Apr 19, 2026
 *      Author: Nhan Truong
 */

#ifndef PLC_MAIN_INCLUDE_PLCMAINEXECUTELOGIC_H_
#define PLC_MAIN_INCLUDE_PLCMAINEXECUTELOGIC_H_

#include <stdint.h>
#include <string.h>
#include "PlcConfig.h"

int8_t InitializeVariable(void);
void ExecutePlcLogic(uint16_t* output_image);

#endif /* PLC_MAIN_INCLUDE_PLCMAINEXECUTELOGIC_H_ */
