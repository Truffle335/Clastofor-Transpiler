# Clastofor-Transpiler

Clastofor is a small experimental DSL (domain-specific language) and transpiler designed for learning, prototyping, and exploring how custom languages can be compiled into native C++17 code.

The project currently supports a minimalist syntax for variable declarations, console output, and console input, and it generates readable C++ code from `.clsf` source files.

## Project Goals

- Create a simple custom language with clear, readable syntax
- Learn the basics of lexing, parsing, and AST generation
- Transpile a DSL into valid C++17 code
- Build a friendly developer experience with helpful error messages
- Experiment with language design and compiler concepts

## Current Features

### Variable declarations
- `int` — integer variables
- `float` — floating-point variables
- `str` — string variables

Example:

```text
int age = 18;
float pi = 3.14;
str name = "Alice";
```

### Console output
- `console.print(...)` — prints values to the console

Example:

```text
console.print("Hello, world!\n");
console.print("Your age is: ", age, "\n");
```

### Console input
- `console.input("prompt")` — reads input from the user

Example:

```text
str name = console.input("Enter your name: ");
```

### Arithmetic support
- `+`, `-`, `*`, `/`, `%`
- basic division-by-zero protection

Example:

```text
int total = 10 + 5;
float result = 7.5 / 2.0;
```

## Error System

The transpiler includes a custom exception system with human-readable diagnostics such as:

- `ErrorSyntax:`
- `ErrorMath:`
- `ErrorFunction:`
- `ErrorVar:`
- `ErrorName:`

Each error includes:
- line number,
- reason,
- suggested fix.

## Example Program

```text
int age = 18;
float pi = 3.14;
str name = console.input("Enter your name: ");

console.print("Hello, ", name, "!\n");
console.print("Your age plus 5 is: ", age + 5, "\n");
```

## How It Works

The compiler pipeline is simple:

1. Find `.clsf` files in the current directory
2. Lex the source into tokens
3. Parse tokens into an AST
4. Generate C++17 code
5. Compile the generated C++ file with `clang++`
6. Run the executable

## Requirements

- C++17 compiler
- `clang++` recommended
- Linux/macOS environment recommended

## Build and Run

Compile the transpiler:

```bash
g++ -std=c++17 main.cpp -o clsf_compiler
```

Place your `.clsf` files in the same directory as the executable and run:

```bash
./clsf_compiler
```

## Repository Structure

```text
Clastofor-Transpiler/
├── main.cpp
├── README.md
└── .gitignore
```

## Notes

This project is still in active development. It is designed as an experimental learning project and a proof-of-concept for custom language tooling.

## License

This project is currently unlicensed. If you want, you can add an open-source license such as MIT or Apache 2.0 later.

## Author

Created by Truffle335, with the language concept designed independently and the implementation assisted with AI tooling.
