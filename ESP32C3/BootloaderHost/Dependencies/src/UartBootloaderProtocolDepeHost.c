/*
 * UartBootloaderProtocolDepeHost.c
 *
 *  Created on: Nov 20, 2025
 *      Author: TTN
 */

#include "UartBootloaderProtocolDepeHost.h"
#include "UartBootloaderProtocolState.h"

StatusResult ParseGetCommand(UartBootloaderProtocolDepeHost_t* uart_bootloader_prtcl_depe_host, uint8_t received_data[])
{
	if(received_data[1] == 0x07)
	{
		uart_bootloader_prtcl_depe_host->get_command_parameter.SupportVersion = received_data[4];
		uart_bootloader_prtcl_depe_host->get_command_parameter.SupportId = received_data[5];
		uart_bootloader_prtcl_depe_host->get_command_parameter.SupportReadMem = received_data[6];
		uart_bootloader_prtcl_depe_host->get_command_parameter.SupportGo = received_data[7];
		uart_bootloader_prtcl_depe_host->get_command_parameter.SupportWriteMem = received_data[8];
		uart_bootloader_prtcl_depe_host->get_command_parameter.SupportEraseMem = received_data[9];
		uart_bootloader_prtcl_depe_host->get_command_parameter.SupportGetChecksum = received_data[10];
		
		return STATUS_OK;
	}
	
	return STATUS_FAIL;
}