# Clastofor

A minimal DSL and transpiler that generates readable C++17 code.

Clastofor is a lightweight experimental language built around the idea of simple syntax, clean code generation, and easy compiler experimentation. It currently supports the essentials: variables, I/O, arithmetic, and custom error reporting.

## Version
v1.0

## Features
- Variable declarations for `int`, `float`, and `str`
- Console output via `console.print(...)`
- Console input via `console.input(...)`
- Basic arithmetic: `+`, `-`, `*`, `/`, `%`
- Division-by-zero detection
- Custom error messages with line-based diagnostics
- Transpilation to readable C++17 source
- Executable generation and local runtime execution

## Quick Start

### Build the transpiler
```bash
g++ -std=c++17 main.cpp -o clsf
```

### Create a `.clsf` file
```text
str name = console.input("Your name: ");
console.print("Hello, ", name, "!\n");
```

### Run
```bash
./clsf
```

Then select the `.clsf` file to compile and run.

## Example
```text
int age = 18;
float pi = 3.14;
str name = console.input("Enter your name: ");

console.print("Hello, ", name, "!\n");
console.print("Your age plus 5 is: ", age + 5, "\n");
```

## Current Status
Clastofor v1.0 is a minimal but working foundation. It focuses on the essentials: variables, input/output, arithmetic, and clean C++ generation.

## Requirements
- C++17 compiler
- `clang++` or `g++`
- Linux/macOS environment recommended

## Repository Structure
```text
Clastofor-Transpiler/
├── main.cpp
├── README.md
├── UPDATES.md
├── .gitignore
└── examples/
```

## Notes
This project is intentionally simple and practical. It is designed as a solid base for further experimentation and language growth.

## Author
Created by Truffle335.
