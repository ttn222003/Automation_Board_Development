# API Contract

## GPIO
- int8_t GpioCommon(void);
Input: No
Return: OK, ERROR
ISR Safe: No

- void BSP_GPIO_Init(void);
Input: No
Return: No
ISR Safe: No

- void BSP_WriteOutputs(uint16_t output_image);
Input: output_image
Return: No
ISR Safe: No