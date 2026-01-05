/*
 * GpioDriver.c
 *
 *  Created on: Jan 4, 2026
 *      Author: ASUS
 */

#include "GpioDriver.h"

/*------- Member variable -------*/
QueueHandle_t mGpioEventQueue = NULL;

/*------- Private variable -------*/
static uint32_t gpio_context[GPIO_NUM_MAX];
static bool isr_installed = false;

/*------- Private function -------*/
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t pin_num = *(uint32_t*) arg;
    xQueueSendFromISR(mGpioEventQueue, &pin_num, NULL);
}

/*------- Interface -------*/
uint8_t GpioReadInit(uint8_t pin_number, uint8_t pull_mode, uint8_t edge_mode, void* isr_handler)
{
	uint8_t pull_up = GPIO_PULLUP_DISABLE;
	uint8_t pull_down = GPIO_PULLDOWN_DISABLE;
	
	uint8_t edge = GPIO_INTR_DISABLE;
	
	if(pull_mode == PULL_UP)
	{
		pull_up = GPIO_PULLUP_ENABLE;
		pull_down = GPIO_PULLDOWN_DISABLE;
	}
	else if (pull_mode == PULL_DOWN) 
	{
		pull_up = GPIO_PULLUP_DISABLE;
		pull_down = GPIO_PULLDOWN_ENABLE;
	}
	else if(pull_mode == PULL_UP_DOWN)
	{
		pull_up = GPIO_PULLUP_ENABLE;
		pull_down = GPIO_PULLDOWN_ENABLE;
	}
	else
	{
		return -1;
	}
	
	if(edge_mode == POS_EDGE)			edge = GPIO_INTR_POSEDGE;
	else if (edge_mode == NEG_EDGE)		edge = GPIO_INTR_NEGEDGE;
	else if(edge_mode == BOTH_EDGE)		edge = GPIO_INTR_ANYEDGE;
	else								return -1;
	
	gpio_config_t input_config = {
        .pin_bit_mask = (1ULL << pin_number),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = pull_up,
        .pull_down_en = pull_down,
        .intr_type = edge
    };
    
    gpio_config(&input_config);
    
    if (mGpioEventQueue == NULL) {
	    mGpioEventQueue = xQueueCreate(32, sizeof(uint32_t));
	}
    
    if (!isr_installed) {
	    gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
	    isr_installed = true;
	}
	
	gpio_context[pin_number] = pin_number;

	if(isr_handler == NULL)
	{
		isr_handler = gpio_isr_handler;
	}
	
    gpio_isr_handler_add(pin_number, isr_handler, &gpio_context[pin_number]);
    
    return 1;
}
