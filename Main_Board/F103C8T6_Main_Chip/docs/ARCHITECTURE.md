# Architecture

## Layers

Core
↓
Depe
↓
Hardware Backend
↓
MCU HAL

## Dependency rules

### Core
Core MUST NOT depend on:
- STM32 HAL
- ESP-IDF
- MCU registers
- FreeRTOS API
- Hardware-specific headers

Core may only depend on:
- C standard library
- Core headers

Detail rule in: CORE_RULES.md

### Depe
Depe MAY depend on:
- STM32 HAL
- MCU registers
- FreeRTOS API
- Hardware drivers

Core MUST NOT include Depe implementation files

Detail rule in: DEPE_RULES.md

### Backend
Hardware backend implements Depe API

