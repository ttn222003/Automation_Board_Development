# Review checklist

## 1. Architecture
- [ ] Core does not depend on hardware
- [ ] Core does not include HAL
- [ ] Core does not access registers
- [ ] Depe does not contain business logic
- [ ] Hardware-specific code stays in backend

## 2. Core Logic
- [ ] Logic is deterministic
- [ ] Error paths handled
- [ ] Boundary condition checked
- [ ] State transitions correct
- [ ] No hidden hardware dependency

## 3. Depe
- [ ] API is hardware-independent
- [ ] HAL errors mapped correctly
- [ ] Timeout handled
- [ ] ISR safety considered
- [ ] Thread safety documented

## 4. TDD
- [ ] Test written before implementation
- [ ] Test fails before implementation
- [ ] Test passes after implementation
- [ ] Error path tested
- [ ] Boundary tested
- [ ] Mock behavior is meaningful
- [ ] No test depends on another test

## 5. Test Quality
- [ ] Test verifies behavior, not implementation
- [ ] Mock expectations are correct
- [ ] Failure cases are tested
- [ ] Regression test added for bug fixes

## 6. Embedded
- [ ] No unintended dynamic allocation
- [ ] No buffer overflow
- [ ] No integer overflow
- [ ] No race condition
- [ ] No deadlock
- [ ] Timeout exist where required
- [ ] ISR APIs are correct

