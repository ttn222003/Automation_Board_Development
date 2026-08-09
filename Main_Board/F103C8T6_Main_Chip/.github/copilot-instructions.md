# Copilot Instructions

This repository follows a strict Core / Depe architecture.

Before performing any code review, audit, or bug analysis always read and follow:

* docs/ARCHITECTURE.md
* docs/CORE_RULES.md
* docs/DEPE_RULES.md
* docs/CODING_RULES.md
* docs/ERROR_HANDLING.md
* docs/FILE_CLASSIFICATION.md
* docs/REVIEW_CHECKLIST.md
* docs/REVIEW_POLICY.md
* docs/TEST_STRATEGY.md
* docs/API_CONTRACT.md

Review process:

1. Classify files according to FILE_CLASSIFICATION.md.
2. Determine module type:
   * Core
   * Depe

3. Apply only the rules relevant to the module type.
4. Report rule violations explicitly.
5. Reference the violated rule document when possible.

Architecture rules:

* Core must not depend on HAL.
* Core must not depend on FreeRTOS.
* Core must not access hardware registers.
* Depe is responsible for hardware abstraction.
* Depe must not contain business logic.

Testing rules:

* Follow TDD.
* Prefer unit-testable design.
* Verify boundary conditions.
* Verify error paths.

Review output:

* Executive Summary
* Critical Issues
* High Issues
* Medium Issues
* Low Issues
* Missing Tests
* Architecture Violations
* Recommendations
