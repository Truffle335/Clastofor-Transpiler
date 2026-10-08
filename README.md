# Clastofor-Transpiler
Fast C++17 transpiler and compiler for the Clastofor DSL programming language with custom error system.

# What can Clastofor do?
Clastofor can create variables, protect math, output and enter data, issue errors and execute .clsf files.

# What is it for?
It's impossible to say for sure yet... Because Clastofor is still in development... And version 1.0

# Who created it?
The author is truffle335, that is, I did not create it myself, but created it with the help of AI, but I got all the syntax commands and all the rules myself.

# What commands are there?
Current commands: 

  Key commands:
    int - creates an int variable
    str - creates a string variable
    float - creates a float variable

  Console I/O & Formatting:
    console.print(...) - outputs data to console (does NOT automatically add a new line)
    console.input("prompt") - gets input from user
    \n - new line character (must be added manually inside strings for line breaks)

  Math & operators:
    There are operators: + - / * % = ,
    Protection: division by zero checks

# Code Example
int age = 18;
float pi = 3.14;
str name = console.input("Enter your name: ");

console.print("Hello, ", name, "!\n");
console.print("Your age plus 5 is: ", age + 5, "\n");

# How to build and run?
1. Make sure you have C++17 and clang++ installed.
2. Compile the transpiler:
   g++ -std=c++17 main.cpp -o clsf_compiler
3. Place your .clsf files in the same folder.
4. Run:
   ./clsf_compiler
   
