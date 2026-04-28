/*
 * PlcMainExecuteLogic.c
 *
 *  Created on: Apr 19, 2026
 *      Author: Nhan Truong
 */

/* User execute logic at here, don't interfere any other files */

#include "PlcMainExecuteLogic.h"

/*
 * Users don't change anything below
 * */

static uint8_t OUT[OUTPUT_NUMBER];

/*======= Internal function =======*/

int8_t InitializeVariable(void)
{
	memset(OUT, 0, sizeof(OUT));

	return 1;
}

static void UpdateResultOutput(uint16_t* output_image)
{
	for (uint8_t index = 0; index < OUTPUT_NUMBER; index++)
	{
		*(output_image + index) = OUT[index];
	}
}

/**********************************/

/*
 * Users start to write code here
 * */



/*
 * Users end to write code here
 * */

void ExecutePlcLogic(uint16_t* output_image)
{
	/*
	 * Users don't change anything below
	 * */

	/**********************************/

	/*
	 * Users start to write code here
	 * */

	OUT[0] = 1;
	OUT[1] = 0;
	OUT[2] = 1;
	OUT[3] = 0;
	OUT[4] = 1;
	OUT[5] = 0;

	/*
	 * Users end to write code here
	 * */

	/*
	 * Users don't change anything below
	 * */
	UpdateResultOutput(output_image);

	/**********************************/
}
