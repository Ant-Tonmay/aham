# Aham Programming Language

Aham is a bytecode-compiled programming language implemented in C++. It compiles .aha source files into compact .ahc bytecode, which is executed by the Aham Virtual Machine (AVM), a stack-based virtual machine.
---

## How It Works

```
source.aha  →  Lexer  →  Parser  →  Compiler  →  source.ahc  →  VM  →  output
```

1. `aham -c file.aha` — compiles source to bytecode
2. `aham -r file.ahc` — runs the compiled bytecode in the VM

There is no interpreter mode. All execution goes through the bytecode VM.

---

## Current Feature Snapshot

| Feature | Status |
|---|---|
| Functions (`func`), `return` | ✅ |
| Call-by-value and `ref` parameters | ✅ |
| Variables, assignment | ✅ |
| Arithmetic, comparison, logical, bitwise operators | ✅ |
| `if` / `else` | ✅ |
| `while`, `for`, `break`, `continue` | ✅ |
| Dynamic arrays `[]`, fixed arrays `shape(n)` | ✅ |
| Classes, fields, methods | ✅ |
| Access blocks (`public` / `private` / `protected`/`shared`) | ✅ |
| Single inheritance (`inherits`) | ✅ |
| Traits (`trait`, `impl`) | ✅ |
| `super` keyword | ✅ |
| Method overloading (multiple definitions) | ✅ |
| Shared (static) fields | ✅ |
| Exception handling (`try` / `catch` / `finally` / `throw`) | ✅ |
| User-defined exception classes | ✅ |
| Modules (`alias m = include math; export { ... }`) | ✅ |
| String interpolation (`"value is {x}"`) | ✅ |
| Type casts: `int()`, `float()`, `string()`, `bool()`, `char()` | ✅ |
| Built-ins: `print`, `println`, `readline`, `type`, `length`, `push` | ✅ |
| `ref` references | ✅ |
| Project builds (`aham build` / `aham run` via `aham.toml`) | ✅ |

---

## Prerequisites

- CMake ≥ 3.16
- C++17 compiler — GCC 9+, Clang 10+, or MSVC 2019+

---

## Build

```bash
cmake -S . -B build
cmake --build build -j
```

The binary is placed at `./aham`.

---

## Usage

```bash
# Compile a source file to bytecode
aham -c examples/hello.aha        # produces examples/hello.ahc

# Run compiled bytecode
aham -r examples/hello.ahc

# Project commands (requires aham.toml in the project root)
aham build                         # compile all sources in dependency order
aham run                           # build if needed, then run the entry point

# Info
aham --help
aham --version
aham --info
```

---

## Quick Example

```aham
{
    func fib(n) {
        if (n == 0 || n == 1) { return n; }
        return fib(n - 1) + fib(n - 2);
    }

    func main() {
        println(fib(10));
    }
}
```
---

## Test

Run all tests:

```bash
ctest --test-dir build --output-on-failure
```

Run a single test:

```bash
ctest --test-dir build -R ParserTest --output-on-failure
```

Registered test targets:

| Test name | Source |
|---|---|
| `LexerTest` | `tests/test_lexer.cpp` |
| `ParserTest` | `tests/test_parser.cpp` |
| `ParserArrayTest` | `tests/test_parser_arrays.cpp` |
| `ParserFunctionTest` | `tests/test_parser_functions.cpp` |
| `ParserLoopsTest` | `tests/test_parser_loops.cpp` |
| `ParserClassesTest` | `tests/test_parser_classes.cpp` |
| `SymbolTableTest` | `tests/test_symbol_table.cpp` |
| `InterpreterTest` | `tests/test_interpreter.cpp` (legacy) |

> VM-level tests are not yet automated. Manual verification uses the `.aha`
> example files in `examples/`.

---

## Project Layout

```
include/            Public headers
  lexer/            Lexer and token definitions
  parser/           AST nodes and parser
  compiler/         Bytecode compiler and serializer/deserializer
  vm/               VM, opcode table, value types
  exceptions/       CompileError / RuntimeError
  dependency_graph/ Import dependency resolution
  loader/           Runtime module loader
  manifest/         aham.toml project manifest

src/                Implementations
  lexer/
  parser/
  compiler/
  vm/
  interpreter/      (legacy tree-walk interpreter — not active)
  symbol_table/     (legacy — not active)
  dependency_graph/
  loader/
  manifest/
  utils/
  main.cpp

examples/           Example .aha programs and compiled .ahc bytecode
tests/              Unit test source files
grammar/            Language grammar notes
goals/              Design goals and planning notes
docs/               Architecture documentation
```

---

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for development setup, architecture map,
coding guidelines, and PR checklist.
