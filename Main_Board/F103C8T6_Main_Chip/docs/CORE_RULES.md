# Core Rules

## Hardware independence
Core MUST NOT include:
- Depe
- STM32 HAL
- FreeRTOS

Core MUST NOT:
- Call Depe APIs
- Access hardware directly
- Contain hardware-specific implementation

Core may only depend on:
- C standard library
- Core headers
- Project-independent data types and utilities

## Responsibility
Core is responsible for:
- Protocol logic
- Business logic
- State machines
- Data processing
- Validation
- Application-independent control logic

Core MUST NOT contain:
- HAL access
- Hardware configuration
- Hardware control
- Hardware-specific timing
- Hardware driver logic

## Memory
- No malloc()
- No free()
- No dynamic allocation

## Functions
Functions should:
- Have one responsibility
- Have deterministic behavior
- Return explicit error status
- Name must clear and describe correct responsibility

## Testaiblity
Every Core module MUST BE unit-testable without physical hardware

## Timing
Core logic should not depend directly on hardware timing APIs

## Core and Depe Independence

Core and Depe are independent modules.

Core MUST NOT call Depe.

Depe MUST NOT call Core.

Task or higher-level application modules are responsible for coordinating Core and Depe.

Example:

Task / Higher-level Module
       /          \
      /            \
   Core            Depe

Core handles logic.

Depe handles hardware.

Neither layer should absorb the responsibility of the other.

Detail architecture rule in: ARCHITECTURE.md