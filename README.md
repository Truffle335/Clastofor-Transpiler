# Clastofor

Clastofor is a transpiler (source-to-source translator) from a simple programming language into C++17. It is designed for those who want to write programs in C++ but struggle with the complexity of its syntax and environment setup. Clastofor takes over the routine work: you write code in a simplified language with clear syntax, and the transpiler turns it into proper C++ and compiles it immediately via clang++.

---

## Overview

| Parameter | Value |
|---|---|
| Name | Clastofor |
| Version | 1.2.2 |
| Type | C++ transpiler |
| Author | truffle355 |
| Release date | 08.10.2026 |
| Implementation language | C++17 |
| Target language | C++17 (compiled via clang++) |
| Source file extension | .clsf |
| Platform | Cross-platform (Linux, macOS, Windows with clang++ installed) |

---

## Purpose of the language

Clastofor was created with a single goal: to lower the entry barrier to C++. Many people want to write in C++ but give up because of:

- complex syntax (templates, pointers, header files, #include, std:: prefixes);
- the need to configure the build and compilation manually;
- bulky constructs for simple tasks (input, output, random numbers).

Clastofor solves these problems. You write simple readable code — the transpiler turns it into correct C++ and compiles it immediately. In essence, Clastofor is a bridge between simplicity and the power of C++.

---

## How it works

1. You create a file with the .clsf extension in the current directory.
2. You run the transpiler.
3. The program finds all .clsf files in the folder and offers you to choose one.
4. The source goes through three stages:
   - Lexical analysis (FastLexer) — splits the text into tokens.
   - Parsing (Parser) — builds an abstract syntax tree (AST) with type and semantic checks.
   - Code generation (codegen) — turns the AST into C++ code.
5. The generated .cpp file is saved to the CompliteFilesClsf/ folder.
6. clang++ is invoked to compile it into an executable.
7. The resulting binary is run immediately.

All language errors (syntax, types, undeclared variables, division by zero) are caught and printed in a clear form with the line number and a hint.

---

## Language syntax

### Variable declarations

int x = 10;
float pi = 3.14;
str name = "Truffle";
bool flag = true;

Types: int, float, str, bool.

### Console output

console.print("Hello, world!");
console.print("x = ", x);

### Console input

int age = console.input("Enter your age: ");
str name = console.input("Your name: ");

The result type is automatically adjusted to the variable type.

### Conditions

if (x > 10) {
    console.print("Greater than ten");
} else {
    console.print("Less than or equal to ten");
}

Supported comparison operators: >, <, >=, <=, ==, !=.

### Arithmetic

int a = 5 + 3 * 2;
int b = 10 % 3;
float c = 5.0 / 2.0;

Operators: +, -, *, /, %.

### Assignment

x = 42;
name = "New name";

### Module import

import random;

After the import, the module's functions become available.

### Random numbers (random module)

int n = random.random.int(1, 100);
float f = random.random.float(0.0, 1.0);
bool b = random.random.bool();

---

## Modules

Modules are .cpp files located in the modules/ folder next to the transpiler. They are automatically included into the generated code when imported.

The official module set is downloaded by the init command from the GitHub repository into the modules/ folder.

---

## Error handling

Clastofor distinguishes several error categories and prints them with the line number, reason, and a hint:

| Category | Meaning |
|---|---|
| ErrorSyntax | Syntax violation (missing ;, unclosed bracket, etc.) |
| ErrorMainKey | Incorrect use of a keyword or construct |
| ErrorMath | Math error (division by zero, extra dot in a number) |
| ErrorFunction | Error in a function call |
| ErrorVar | Variable-related error (redeclaration) |
| ErrorName | Unknown name or undeclared variable |
| ErrorType | Type mismatch |

Example output:

[ ErrorType: ] (Line 4)
  Reason: Type mismatch: cannot initialize 'int' variable 'x' with 'str' value
  Tip:    Ensure the assigned value matches the declared variable type ('int', 'float', 'str', 'bool').

---

## Requirements

- clang++ compiler with C++17 support.
- curl or wget utility for the init command.
- Standard C++17 library.

---

## License and authorship

Author: truffle355
Version: 1.2.2
Release date: 08.10.2026

Clastofor is distributed as a free tool for simplifying work with C++.
