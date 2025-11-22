/*
 * BootloaderHost.c
 *
 *  Created on: Nov 20, 2025
 *      Author: ASUS
 */
 
#include "FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "soc/gpio_num.h"
#include "string.h"
#include "driver/gpio.h"
#include <stdint.h>
#include <sys/_intsup.h>
#include "UartBootloaderProtocolCoresHost.h"
#include "UartBootloaderProtocolState.h"
#include "UartBootloaderProtocolDepeHost.h"


#define RX_BUF_SIZE 	128
#define MAX_DATA_LEN	64
#define UART_NUM		UART_NUM_1
#define TXD_PIN			(GPIO_NUM_21)
#define RXD_PIN 		(GPIO_NUM_20)

uint8_t TransmittedDataToDevice[MAX_DATA_LEN] = { 0 };
uint8_t ReceivedDataFromDevice[MAX_DATA_LEN] = { 0 };
UartBootloaderProtocolDepeHost_t uart_bootloader;
QueueHandle_t uart_queue;
TaskHandle_t HandleGetCommandTask_handle;


void DelayMs(uint32_t delay_time);
void uart_init(void);
void UartTransmissionTask(void* args);
void UartReceptionTask(void* args);
void HandleGetCommandTask(void* args);

void app_main(void)
{
	// Initialize
	memset(TransmittedDataToDevice, 0, 64);
	memset(ReceivedDataFromDevice, 0, 64);
	uart_bootloader.BootloaderCommandType = GET_CMD;
	uart_bootloader.CurrentStep = STEP_1;
    uart_init();
    xTaskCreate(UartReceptionTask, "Uart Reception", 64, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(UartTransmissionTask, "Uart Transmission", 64, NULL, configMAX_PRIORITIES - 2, NULL);
	xTaskCreate(HandleGetCommandTask, "Handle Get Command", 64, NULL, configMAX_PRIORITIES - 3, &HandleGetCommandTask_handle);
}

void DelayMs(uint32_t delay_time)
{
	vTaskDelay(delay_time / portTICK_PERIOD_MS);
}

void uart_init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM, RX_BUF_SIZE * 2, 0, 0, &uart_queue, 0);
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void UartTransmissionTask(void* args)
{
	int number_of_byte_sent_successfully = 0x00;
	
	DelayMs(2000);
	
	while (1) {
		switch (uart_bootloader.BootloaderCommandType) {
			case GET_CMD:
				TransmittedDataToDevice[0] = 2;
				TransmittedDataToDevice[1] = uart_bootloader.BootloaderCommandType;
				TransmittedDataToDevice[2] = 0xFF - uart_bootloader.BootloaderCommandType;
				number_of_byte_sent_successfully = uart_write_bytes(UART_NUM, TransmittedDataToDevice, 64);
				
				if (number_of_byte_sent_successfully == 64) {
					ESP_LOGI("Uart Transmitt", "Uart Transmitt Successfully!");
				}
				else {
					ESP_LOGE("Uart Transmitt", "Transmitt Error!");
				}
				
				uart_bootloader.BootloaderCommandType = NOT_CODE;
				
				break;
		}
	}
}

void UartReceptionTask(void* args)
{
	uart_event_t UartEvent;
	
	while (1) {
		// Get event from queue
		if (xQueueReceive(uart_queue, &UartEvent, portMAX_DELAY)) {
			if (UartEvent.type == UART_DATA) {
				uart_read_bytes(UART_NUM, ReceivedDataFromDevice, 64, 5 / portTICK_PERIOD_MS);
	
				if (ReceivedDataFromDevice[1] == ACK) {
					switch (uart_bootloader.BootloaderCommandType) {
						case GET_CMD:
							xTaskNotifyGive(HandleGetCommandTask_handle);
							break;
						
						case GET_VERSION:
							break;
					}
				}
				else if (ReceivedDataFromDevice[0] == 11) {
					xTaskNotifyGive(HandleGetCommandTask_handle);
				}
			}
		}
	}
}

void HandleGetCommandTask(void* args)
{
	while(1) {
		// pdTRUE: Reset counter notify to 0 after get signal
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		
		switch (uart_bootloader.CurrentStep) {
			case STEP_1:
				uart_bootloader.CurrentStep = STEP_2;
				break;
			
			case STEP_2:
				if(ParseGetCommand(&uart_bootloader, ReceivedDataFromDevice) == STATUS_OK)
				{
					ESP_LOGI("Info Get Command", "Support Version: %d", uart_bootloader.get_command_parameter.SupportVersion);
					ESP_LOGI("Info Get Command", "Support ID: %d", uart_bootloader.get_command_parameter.SupportId);
					ESP_LOGI("Info Get Command", "Support Read Memory: %d", uart_bootloader.get_command_parameter.SupportReadMem);
					ESP_LOGI("Info Get Command", "Support Go: %d", uart_bootloader.get_command_parameter.SupportGo);
					ESP_LOGI("Info Get Command", "Support Write Memory: %d", uart_bootloader.get_command_parameter.SupportWriteMem);
					ESP_LOGI("Info Get Command", "Support Erase Memory: %d", uart_bootloader.get_command_parameter.SupportEraseMem);
					ESP_LOGI("Info Get Command", "Support Get Checksum: %d", uart_bootloader.get_command_parameter.SupportGetChecksum);
					
					uart_bootloader.BootloaderCommandType = GET_VERSION;
					
					uart_bootloader.CurrentStep = STEP_3;
				}
				else {
					ESP_LOGE("Error Get Command", "Wrong data!");
					return;
				}
				
				break;
			
			case STEP_3:
				uart_bootloader.CurrentStep = STEP_1;
				break;
		}
	}
}
