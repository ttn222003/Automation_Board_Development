/*
 * UartBootloaderProtocolDepeHost.c
 *
 *  Created on: Nov 20, 2025
 *      Author: TTN
 */
 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "portmacro.h"
#include "soc/gpio_num.h"
#include "string.h"
#include "driver/gpio.h"
#include "UartBootloaderProtocolCoresHost.h"
#include "UartBootloaderProtocolDepeHost.h"
#include <stdint.h>
#include "GpioDriver.h"

/*static const int RX_BUF_SIZE = 1024;*/


#define UART_NUM		UART_NUM_1
#define TXD_PIN 		(GPIO_NUM_21)
#define RXD_PIN 		(GPIO_NUM_20)

QueueHandle_t uart_queue;
TaskHandle_t HandleTransmissionTask_handle;
TaskHandle_t HandleGetCommandTask_handle;

void read_input_task(void* arg);

void InitializeUart(void);
void UartReceptionTask(void* args);
void UartTransmissionTask(void* args);
void HandleGetCommandTask(void* args);

void app_main(void)
{
	GpioReadInit(GPIO_NUM_1, PULL_UP, NEG_EDGE);
	
	xTaskCreate(read_input_task, "read input task", 8192, NULL, configMAX_PRIORITIES - 1, NULL);
	
	
	
    /*InitializeUart();
    InitializeUartBootloaderProtocol(&mUartBootloader);
    
	InitializeDataBuffer();
	
	xTaskCreate(UartReceptionTask, "Uart Rx", 8192, NULL, configMAX_PRIORITIES - 1, NULL);
	xTaskCreate(UartTransmissionTask, "Uart Tx", 8192, NULL, configMAX_PRIORITIES - 2, &HandleTransmissionTask_handle);
	xTaskCreate(HandleGetCommandTask, "Handle GC", 8192, NULL, configMAX_PRIORITIES - 3, &HandleGetCommandTask_handle);*/
}

void read_input_task(void* arg)
{
    uint32_t pin_num;
    while (1) {
        if (xQueueReceive(mGpioEventQueue, &pin_num, portMAX_DELAY)) {
			switch (pin_num) {
				case GPIO_NUM_1:
					// @TODO: Handle to jump to send GET_CMD here
					// @TODO 2: Handle debounce button
					ESP_LOGI("Test GPIO", "Interrupt on pin 1");
					break;
			}
        }
    }
}















/*void InitializeUart(void)
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
    uart_driver_install(UART_NUM, RX_BUF_SIZE * 2, 0, 10, &uart_queue, 0);
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void UartTransmissionTask(void* args)
{	
	DelayMs(5000);
	while (1) {
		
		switch (mUartBootloader.CommandCode) {
			case GET_CMD:
//				HandleBeginingProcessData(mUartBootloader, TransmittedDataToDevice);
				
				for (uint8_t transmitted_data_index = 0; transmitted_data_index < TransmittedDataToDevice[1]; transmitted_data_index++)	{
					uart_write_bytes(UART_NUM, &TransmittedDataToDevice[transmitted_data_index], 1);
				}
				
				break;
		}
		
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
}

void UartReceptionTask(void* args)
{
	uart_event_t UartEvent;
	uint8_t received_data_from_device = 0x00;
	while (1) {
		// Wait until get data
		if (xQueueReceive(uart_queue, &UartEvent, portMAX_DELAY)) {
			if (UartEvent.type == UART_DATA) {
				uart_read_bytes(UART_NUM, &received_data_from_device, UartEvent.size, 1);
				
				ReceiveDataAndPutInBuffer(received_data_from_device);
				
				
				if(GetProcessStatus(mUartBootloader) == IN_PROCESS)
				{
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
		
		
		switch (GetHandlingStep(mUartBootloader)) {
			case STEP_1:
				if(ReceivedDataFromDevice[2] == ACK)
				{
					ESP_LOGI("Rx", "%d", ACK);
				}
				
				SetHandlingStep(&mUartBootloader, STEP_2);
				SetProcessStatus(&mUartBootloader, NOT_IN_PROCESS);
				
				
		}
	}
}*/