/*
 * PlcConfig.h
 *
 *  Created on: Apr 23, 2026
 *      Author: ASUS
 */

#ifndef COMMONDEFINE_PLCCONFIG_H_
#define COMMONDEFINE_PLCCONFIG_H_

/*======= Users Configurate parameters here =======*/
/*======= Define Versions of PLC =======*/
#define VERSION         V1


/*======= Define Type of CRC  =======*/
#define CRC_TYPE	    CRC_16_MODBUS

/*=================================================*/

/*======= PLC Parameters =======*/
#define V1              1
#define CRC_16_MODBUS	0

#if VERSION == V1

#define OUTPUT_NUMBER	6
#define INPUT_NUMBER	0

#endif

#if CRC_TYPE == CRC_16_MODBUS

#define CRC_POLYNOMIAL			0xA001

#endif

#define PLC_SCAN_PERIOD_MS		10

#endif /* COMMONDEFINE_PLCCONFIG_H_ */
