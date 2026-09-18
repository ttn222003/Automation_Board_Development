# API Contract

## GPIO
- int8_t MappingBspGpioOutputsToInternalOutputs(void);
Input: No
Return: Internal output status
ISR Safe: No

int8_t DeMappintBspGpioOutputsFromInternalOutputs(void);
Input: No
Return: Internal output status
ISR Safe: No

- eBspGpioStatus InitializeBspGpioOutputs(void);
Input: No
Return: BSP GPIO status
ISR Safe: No

- eBspGpioStatus WriteBspGpioOutputs(uint16_t output_image);
Input: output_image
Return: BSP GPIO status
ISR Safe: No