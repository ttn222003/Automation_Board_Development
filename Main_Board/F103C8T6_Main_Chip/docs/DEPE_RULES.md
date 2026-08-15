# Depe rules

## Purpose
Depe provides an abstraction between Core logic and Hardware

## Responsibility
Depe is responsible for:
- Hardware access
- HAL mapping
- Peripheral configuration

Depe is NOT responsible for:
- Firmware update state machine
- Packet parsing
- Business logic
- Protocol decisions

## API
Depe API must be hardware-independent

Example:

InitBspUart() rather than HAL_UART_Init()
BSP_GPIO_Init() rather than HAL_GPIO_Init
