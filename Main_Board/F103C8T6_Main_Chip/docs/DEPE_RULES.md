# Depe rules

## Purpose
- Depe is the hardware-dependent layer responsible for isolating hardware implementation from the rest of the application.

- Depe provides project-defined APIs for hardware operations while hiding hardware-specific implementation details from higher-level modules.

## Responsibility
Depe is responsible for:
- Hardware access
- HAL mapping
- Peripheral configuration
- Hardware driver interaction
- Hardware-specific error mapping
- Hardware-specific timing and synchronization where required

Depe is NOT responsible for:
- Firmware update state machine
- Packet parsing
- Business logic
- Protocol decisions
- Application state machines
- Core logic

Depe MUST NOT call Core APIs.

Core and Depe are independent modules.

## API
The Depe implementation MAY depend on:
- STM32 HAL
- MCU registers
- Hardware drivers
- Hardware-specific types

However, the public Depe API MUST hide hardware-specific implementation details whenever they are not required by the API contract.

Public Depe APIs SHOULD use project-defined types instead of exposing:
- HAL-specific types
- MCU register types
- Hardware-specific implementation structures


Example:

BspUartStatus_t InitBspUart(void);
BspUartStatus_t SendBspUart(const uint8_t *data, uint16_t len);

rather than exposing HAL APIs directly:

HAL_StatusTypeDef HAL_UART_Init(...);
HAL_StatusTypeDef HAL_UART_Transmit(...);


The purpose is to allow the hardware implementation to change without requiring changes to higher-level application logic.