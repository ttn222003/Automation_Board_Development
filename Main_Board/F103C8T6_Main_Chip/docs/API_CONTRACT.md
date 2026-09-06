# API Contract

## GPIO
- int8_t InitBspInternalOutputs(void);
Input: No
Return: Internal output status
ISR Safe: No

int8_t DeinitBspInternalOutputs(void);
Input: No
Return: Internal output status
ISR Safe: No

- void InitBspGpio(void);
Input: No
Return: BSP GPIO status
ISR Safe: No

- void WriteBspGpioOutputs(uint16_t output_image);
Input: output_image
Return: BSP GPIO status
ISR Safe: No