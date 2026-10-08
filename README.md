# Clastofor

A fast, minimal DSL that compiles directly to C++17.

Write simple programs in Clastofor syntax, get optimized C++ binaries.

**Version: 1.0**

## Features

- **Fast compilation** — transpiles to C++17, compiled with clang++
- **Minimal syntax** — only what you need, nothing extra
- **Direct output** — generates readable, standard C++ code
- **Zero overhead** — no runtime, no virtual machine

## Quick Start

### Install

```bash
g++ -std=c++17 main.cpp -o clsf
```

### Write a program

Create `hello.clsf`:

```text
str name = console.input("Your name: ");
console.print("Hello, ", name, "!\n");
```

### Run it

```bash
./clsf
# Select: 1 (hello.clsf)
# Output: compiled and executed
```

## Language Syntax

### Variables

```text
int x = 42;
float pi = 3.14;
str message = "Hello";
```

### Output

```text
console.print("Value: ", x, "\n");
console.print(pi, " ", message, "\n");
```

### Input

```text
str response = console.input("Question: ");
int number = console.input("Number: ");
```

### Math

```text
int result = 10 + 5;
int division = 20 / 4;
int remainder = 10 % 3;
```

Supported operators: `+`, `-`, `*`, `/`, `%`

Division by zero is caught and reported.

## Example Programs

### Simple Calculator

```text
int a = console.input("First number: ");
int b = console.input("Second number: ");
console.print("Sum: ", a + b, "\n");
```

### String Output

```text
str greeting = "Welcome";
console.print(greeting, " to Clastofor\n");
```

## How It Works

1. Lexer tokenizes source code
2. Parser builds an AST
3. Code generator produces C++17
4. clang++ compiles to executable
5. Program runs

Generated C++ files are saved in `CompliteFilesClsf/` directory.

## Requirements

- C++17 compiler
- clang++ installed
- Linux/macOS

## Error Messages

Clear error reporting with line numbers and suggestions:

- `ErrorSyntax` — invalid syntax
- `ErrorMath` — math errors (division by zero, etc.)
- `ErrorVar` — variable issues
- `ErrorName` — undefined names
- `ErrorFunction` — function call errors

## Current Limitations (v1.0)

Version 1.0 is minimal by design. Future versions will add:

- Control flow (if/else, loops)
- Functions and procedures
- Arrays and data structures
- More complex expressions

For now, Clastofor focuses on core functionality: variables, I/O, and basic math.

## Notes

Clastofor is a minimal, practical language. It's designed to be simple and fast, not feature-rich.

## Author

Created by Truffle335. Language design and compiler implementation.
