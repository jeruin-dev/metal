# Agent Guidelines for Metal Project

## Build Commands
- Setup build: `meson setup build`
- Build project: `meson compile -C build`
- Clean build: `rm -rf build && meson setup build`

## Test Commands
- Run all tests: `meson test -C build` (no tests currently defined)
- Run single test: N/A (no tests defined)

## Code Style Guidelines

### Language & Standards
- C++23 standard required
- Use clang++ compiler with warning level 3

### Imports & Includes
- Use angle brackets for standard library headers: `#include <iostream>`
- Group includes logically (standard library first, then external deps)

### Naming Conventions
- Constants: UPPERCASE_WITH_UNDERSCORES (e.g., `PROJECT_NAME`)
- Functions: camelCase (e.g., `main`)
- Variables: camelCase or descriptive names

### Error Handling
- Return 0 for success, non-zero for errors
- Use std::cout/std::cerr for output/error messages
- Check function arguments and provide usage messages

### Formatting
- 4-space indentation
- Opening braces on same line as function/if statements
- Consistent spacing around operators

### Dependencies
- LLVM core and irreader modules required
- Use Meson dependency system for external libraries

## Language Implementation

### Components
- **Lexer** (`lexer.h`, `lexer.cpp`): Tokenizes source code into tokens
- **Parser** (`parser.h`, `parser.cpp`): Builds AST from tokens using recursive descent
- **AST** (`ast.h`): Defines abstract syntax tree node structures
- **Code Generator** (`codegen.h`, `codegen.cpp`): Generates LLVM IR from AST

### Usage
- Run with: `./build/metal "function_name(param1;param2) expression"`
- Example: `./build/metal "foo(a;b) 1+2*3"`