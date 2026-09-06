# Architecture

## Layers

The system is divided into independent Core and Depe modules.

                Task / Higher-level Module
                       /        \
                      /          \
                   Core          Depe
                    │              │
                    │              ▼
                    │         Hardware Backend
                    │              │
                    │              ▼
                    │            MCU HAL

Core and Depe are independent from each other.

Higher-level Task/Application modules are responsible for coordinating Core and Depe.

## Dependency rules

### Core
Core MUST NOT depend on:
- STM32 HAL
- Depe
- ESP-IDF
- MCU registers
- FreeRTOS API
- Hardware-specific headers
- Hardware drivers

Core may only depend on:
- C standard library
- Core headers
- Project-independent data types and utilities

Core MUST NOT include Depe implementation files

Detail rule in: CORE_RULES.md

### Depe
Depe is the hardware-dependent layer of the system.

Depe MAY depend on:
- STM32 HAL
- MCU registers
- FreeRTOS API
- Hardware drivers
- Hardware-specific configuration

Depe MUST NOT depend on:

- Core implementation
- Core state machines
- Protocol/business logic

Detail rule in: DEPE_RULES.md

### Task / Higher-level Module
Task or higher-level application modules coordinate Core and Depe.

They MAY:
- Call Core APIs
- Call Depe APIs
- Transfer data between Core and Depe
- Control execution flow and sequencing

They are responsible for orchestration, not for moving Core logic into Depe or hardware logic into Core.

### Backend
Hardware Backend implements the hardware-facing behavior required by Depe.

The backend may use:
- MCU HAL
- MCU registers
- Hardware-specific drivers

Hardware-specific implementation should remain isolated from Core logic.