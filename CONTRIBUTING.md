# Contributing to Aham

This project welcomes contributions to the language, compiler, VM runtime, and tests.

---

## Architecture Overview

Aham uses a **two-phase, bytecode-compiled pipeline**. There is no tree-walk interpreter
involved in normal execution.

```
Source (.aha)
   │
   ▼
 Lexer          → tokens              include/lexer/lexer.h
                                      src/lexer/lexer.cpp
                                      src/lexer/utils/lexer_helpers.cpp
   │
   ▼
 Parser         → AST                 include/parser/ast.h
                                      include/parser/parser.h
                                      src/parser/parser.cpp
   │
   ▼
 Compiler       → Bytecode chunks     include/compiler/compiler.h
   │               (FunctionObject)   src/compiler/compiler_core.cpp
   │                                  src/compiler/compiler_expr.cpp
   │                                  src/compiler/compiler_stmt.cpp
   │
   ▼
 Serializer     → .ahc file          include/compiler/utils/serializer.h
                                      src/compiler/utils/serializer.cpp
   │
   ▼  (aham -r)
 Deserializer   → FunctionObjects    include/compiler/utils/deserializer.h
                                      src/compiler/utils/deserializer.cpp
   │
   ▼
 VM             → executes bytecode   include/vm/vm.h
                                      include/vm/value.h
                                      include/vm/opcode.h
                                      src/vm/vm.cpp
                                      src/vm/vm_ops.cpp
                                      src/vm/vm_call.cpp
                                      src/vm/vm_class.cpp
                                      src/vm/vm_array.cpp
                                      src/vm/vm_builtins.cpp
                                      src/vm/vm_helper.cpp
                                      src/vm/vm_trait.cpp
```

### Supporting Subsystems

| Subsystem | Headers | Sources | Purpose |
|---|---|---|---|
| Error reporting | `include/exceptions/error.h` | `src/utils/utils.cpp` | `CompileError`, `RuntimeError`, `printError` |
| Dependency graph | `include/dependency_graph/` | `src/dependency_graph/` | Resolves `import` order before compilation |
| Module loader | `include/loader/module_loader.h` | `src/loader/module_loader.cpp` | Loads `.ahc` modules at runtime |
| Manifest system | `include/manifest/` | `src/manifest/` | Project-level build configuration (`aham build` / `aham run`) |
| Value utilities | `include/vm/utils/` | `src/vm/utils/` | `typeOf`, `asDouble`, `valueToString`, etc. |

> **Note:** The `src/interpreter/` and `include/interpreter/` directories exist but belong to a legacy
> tree-walk interpreter that is no longer part of the active execution pipeline. Do **not**
> add new language features there. All runtime work goes in `src/vm/`.

---

## Development Setup

### Prerequisites

- C++17-compatible compiler (GCC 9+, Clang 10+, or MSVC 2019+)
- CMake 3.16+

### Build

```bash
cmake -S . -B build
cmake --build build -j
```

The output binary is `./aham` (also placed in `build/`).

### CLI Usage

```bash
# Compile a source file to bytecode
aham -c <file.aha>            # produces <file.ahc> beside the source

# Run a compiled bytecode file
aham -r <file.ahc>

# Project-level commands (requires aham.toml manifest)
aham build                    # compile all sources in dependency order
aham run                      # build if needed, then execute entry point
```

### Run Tests

```bash
ctest --test-dir build --output-on-failure
```

---

## Where to Make Changes

### Language Syntax (tokens / grammar)

- Token types: `include/lexer/lexer.h` → `TokenType` enum
- Lexing rules: `src/lexer/lexer.cpp`, `src/lexer/utils/lexer_helpers.cpp`
- AST node types: `include/parser/ast.h`
- Parser grammar: `src/parser/parser.cpp`

### Compiler (AST → Bytecode)

- Opcode definitions: `include/vm/opcode.h`
- Expression compilation: `src/compiler/compiler_expr.cpp`
- Statement compilation: `src/compiler/compiler_stmt.cpp`
- Compiler core / helpers: `src/compiler/compiler_core.cpp`
- Bytecode serialization: `src/compiler/utils/serializer.cpp` / `deserializer.cpp`

### VM Runtime

- Dispatch loop & module execution: `src/vm/vm.cpp`
- Arithmetic / comparison / jump / cast ops: `src/vm/vm_ops.cpp`
- Function calls and returns: `src/vm/vm_call.cpp`
- Class / instance / method dispatch: `src/vm/vm_class.cpp`
- Trait handling: `src/vm/vm_trait.cpp`
- Array operations: `src/vm/vm_array.cpp`
- Built-in functions (`println`, `type`, etc.): `src/vm/vm_builtins.cpp`
- Exception throw / catch helpers: `src/vm/vm_helper.cpp`
- Value type definitions: `include/vm/value.h` (`Value` variant, all `ObjHeader` structs)
- Value helpers: `src/vm/utils/value_utils.cpp`

### Error Reporting

- `CompileError` / `RuntimeError` types: `include/exceptions/error.h`
- `printError` formatter: `src/utils/utils.cpp`
- `SourceLocation` (line, col, source text): `include/lexer/lexer.h`

### CLI Entry Point

- `src/main.cpp` — handles `-c`, `-r`, `build`, `run` modes

---

## Contribution Workflow

1. Create a feature branch from `main`.
2. Make focused changes — one logical change per PR where possible.
3. Add or update tests for any behavior change.
4. Run the full test suite locally before opening a PR.
5. Open a PR describing:
   - What changed
   - Why it changed
   - How you tested it

---

## Testing Expectations

Tests live in `tests/`. When changing language behavior, update the relevant file:

| Change area | Test file |
|---|---|
| Lexer tokens / rules | `tests/test_lexer.cpp` |
| Expression parsing | `tests/test_parser.cpp` |
| Array expression parsing | `tests/test_parser_arrays.cpp` |
| Class / OOP parsing | `tests/test_parser_classes.cpp` |
| Function parsing | `tests/test_parser_functions.cpp` |
| Loop parsing | `tests/test_parser_loops.cpp` |
| Symbol table (legacy) | `tests/test_symbol_table.cpp` |

> **VM semantics are not yet covered by automated unit tests.** Verify VM behavior
> manually using the example programs in `examples/`. A dedicated `test_vm.cpp`
> is a welcome contribution.

Run a subset of tests while iterating:

```bash
ctest --test-dir build -R LexerTest --output-on-failure
ctest --test-dir build -R Parser --output-on-failure
```

---

## Coding Guidelines

- Use **C++17**-compatible code.
- All runtime objects (`FunctionObject`, `ClassObject`, etc.) must inherit `ObjHeader`
  and be allocated via `VM::allocate<T>()` so the VM's object list tracks them for cleanup.
- Use `CompileError` for errors detected during compilation (lexing, parsing, compiling).
  Use `RuntimeError` for errors detected at execution time.
- Error messages shown to users go through `printError()` in `src/utils/utils.cpp`.
  Do not write directly to `std::cerr` from the VM or compiler.
- Platform-specific integer sizing uses the `IntType` alias in `include/vm/value.h`
  (`int64_t` on Windows / MSVC, `__int128` on all other platforms). Use `IntType`
  instead of hardcoding `int64_t` for language integer values.
- Keep changes minimal and locally coherent. Match existing style in surrounding files.
- Prefer readable, explicit logic over clever shortcuts.

---

## PR Checklist

- [ ] Builds cleanly with CMake (`cmake --build build -j`)
- [ ] All tests pass (`ctest --test-dir build --output-on-failure`)
- [ ] Added or updated tests for new behavior
- [ ] No unrelated refactors bundled in the same PR
- [ ] New opcodes are added to `include/vm/opcode.h` **and** handled in `vm.cpp`
- [ ] Bytecode format changes are reflected in both `serializer.cpp` and `deserializer.cpp`
- [ ] Documentation and example files updated when syntax or behavior changes

---

## Notes on Generated Files

- `.ahc` files are compiled bytecode. Do not commit them unless they are checked-in
  as part of a test fixture with a clear reason.
- Prefer out-of-source builds in `build/`. Avoid committing `CMakeCache.txt`,
  `compile_commands.json`, or other CMake-generated artifacts.
- The `build/` directory is listed in `.gitignore`.
