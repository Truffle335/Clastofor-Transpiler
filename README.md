# Clastofor

A minimal DSL and transpiler that generates readable C++17 code.

Clastofor is a lightweight experimental language built around the idea of simple syntax, clean code generation, and easy compiler experimentation. It supports variables, I/O, arithmetic, booleans, and conditional logic.

## Version
v1.1

## Features
- Variable declarations for `int`, `float`, `str`, and `bool`
- Boolean literals: `true` and `false`
- Conditional statements: `if (...) { ... } else { ... }`
- Console output via `console.print(...)`
- Console input via `console.input(...)`
- Basic arithmetic: `+`, `-`, `*`, `/`, `%`
- Comparisons: `>`, `<`, `>=`, `<=`, `==`, `!=`
- Division-by-zero detection
- Custom error messages with line-based diagnostics
- Transpilation to readable C++17 source
- Executable generation and local runtime execution

## Quick Start

### Build the transpiler
```bash
cd Clastofor
g++ -std=c++17 main.cpp -o clsf
```

### Create a `.clsf` file
```text
int age = 20;
bool isAdult = age >= 18;

if (isAdult) {
    console.print("Adult\n");
} else {
    console.print("Minor\n");
}
```

### Run
```bash
./clsf
```

Then select the `.clsf` file to compile and run.

## Example
```text
int age = 20;
bool isAdult = age >= 18;

if (isAdult) {
    console.print("Adult\n");
} else {
    console.print("Minor\n");
}
```

## Repository Structure
```text
Clastofor-Transpiler/
├── Clastofor/
│   ├── main.cpp
│   └── examples/
│       ├── hello.clsf
│       ├── calculator.clsf
│       ├── input.clsf
│       └── condition.clsf
├── README.md
├── UPDATES.md
├── LICENSE
└── .gitignore
```

## Notes
This project is intentionally simple and practical. It is designed as a solid base for further experimentation and language growth.

## Author
Created by Truffle335.
