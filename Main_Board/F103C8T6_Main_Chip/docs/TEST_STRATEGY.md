# Test Strategy

## Core
Every Core module must have:

- Unit test
- Mocked dependencies (if need)

## Depe
Depe tests verify:
- HAL mapping
- Timeout
- Hardware interaction
- Peripheral configuration

## Test flow
RED
↓
Write falling test
↓
GREEN
↓
Implement minium code
↓
REFACTOR
↓
Run all tests

## Coverage target
80%