# Core Rules

## Hardware independence
Core MUST NOT include:
- STM32 HAL
- FreeRTOS

Core MUST NOT access hardware registers

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