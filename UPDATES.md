# Clastofor Update Log

This file tracks the major changes, milestones, and planned improvements for the Clastofor language and transpiler.

## Version 1.0

### Release date
- 2026-10-08

### Included features
- Variable declarations for `int`, `float`, and `str`
- Basic console input via `console.input(...)`
- Console output via `console.print(...)`
- Basic arithmetic operators: `+`, `-`, `*`, `/`, `%`
- Division-by-zero protection
- Custom error system with line-based diagnostics
- Transpilation to readable C++17 code
- Executable generation and local runtime execution

### Current status
- The language is functional in its core form
- Syntax is intentionally minimal to keep the system simple and fast
- The project is designed as a strong foundation for future expansion

---

## Planned updates

### v1.1 — Control flow
- Add `if` and `else`
- Add comparison operators: `==`, `!=`, `>`, `<`, `>=`, `<=`
- Add boolean logic support

### v1.2 — Reusable logic
- Add functions and procedures
- Add parameter passing
- Support return values

### v1.3 — Loops
- Add `while` loops
- Add `for` loops
- Improve parsing of block-based statements

### v1.4 — Data structures
- Arrays
- Lists
- More typed containers

### v1.5 — Better runtime behavior
- Cleanup generated C++ output
- Optimize for clearer, faster transpiled code
- Improve compile-time checks and syntax diagnostics

---

## Notes

This update log is intentionally simple and keeps track of the project roadmap rather than every small internal change.

Clastofor is currently in its first stable minimal version. The goal is to expand gradually without losing the language's simplicity.
