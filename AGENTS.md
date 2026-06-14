# AGENTS.md

## Project Overview

`ctdd` is a C23 project wired for test-driven development using
Unity and CMock.

## Build System

- **Generator:** Ninja
- **CMake minimum:** 3.21
- **C standard:** C23

### Commands

Configure (default):
```sh
cmake -S . -B build -G Ninja
```

Configure with coverage:
```sh
cmake -S . -B build-cov -G Ninja -DENABLE_COVERAGE=ON
```

Build:
```sh
ninja -C build
```

Run tests (full Unity output, colored):
```sh
ninja -C build check
```

Run tests (CTest summary only):
```sh
ninja -C build test
```

Run tests and generate coverage HTML:
```sh
ninja -C build-cov coverage
```

Run (Linux/macOS):
```sh
./build/main
```

Run (Windows):
```sh
./build/main.exe
```

### Dependencies

Dependencies are managed via custom `Find*.cmake` scripts in `deps/`.
These scripts use `FetchContent` under the hood to download and build
libraries automatically.

To add a new dependency:

1. Add the corresponding `Find<name>.cmake` to `deps/`
2. Add `find_package(<name> REQUIRED)` to `CMakeLists.txt`
3. Link with `<name>::<name>` in `target_link_libraries()`

### CMake Module Path

`deps/` is added to `CMAKE_MODULE_PATH` so `find_package()` resolves
to the custom scripts instead of system-installed packages.

## Coding Conventions

- **Language:** C23
- **Trailing return type** for function signatures (e.g. `auto fn() -> void`)
- **4-space indentation**
- **No semicolons after closing braces** for namespaces/classes
- `auto` for obvious types (e.g. `auto main(...) -> int`)
- **East const** (e.g. `char const*` not `const char*`)
- **Trailing return type** for all function definitions, including
  operators (e.g. `auto operator=(T&&) noexcept -> T&`)
- **Public members first** in class declarations, private members at the
  bottom
- `<>` includes only for system headers (std, OS, etc.)
- `""` includes for third-party dependencies (e.g. `fmt`,
  `nlohmann/json`)
- **Naming:** `snake_case` for variables, functions, and classes
- **Naming:** `SCREAMING_SNAKE_CASE` only for macros and constants
- Include order:
  1. C++ standard library headers (`<chrono>`, `<vector>`, etc.)
  2. *(blank line)*
  3. C standard library headers (`<stdlib.h>`, `<string.h>`, etc.)
  4. *(blank line)*
  5. OS-specific headers (Windows API, POSIX, etc.)
  6. *(blank line)*
  7. Third-party dependencies (`"fmt/core.h"`, etc.)
  8. *(blank line)*
  9. Local/project headers

## Shell Scripts

- Always use `#!/bin/sh` shebang for shell scripts
- Scripts must be POSIX compliant (no bashisms)
- When providing commands to users:
  - Windows/PowerShell: use `` ` `` for line continuation
  - Unix/Linux/macOS: use `\` for line continuation

## Commit Messages

- Follow the 50/72 rule:
  - Subject line: max 50 characters
  - Body lines: wrapped at 72 characters
- Use conventional commit prefixes (`feat:`, `fix:`, `docs:`, `chore:`,
  etc.)
- Separate subject from body with a blank line
- Do **not** add yourself as a co-author (`Co-Authored-By:` trailers are
  forbidden)

Example:

```
feat: add stopwatch timer

Replace Hello World with a live stopwatch that prints elapsed time
in HH:MM:SS.mmm format, updating every 10ms with color output.
```

## Documentation (Markdown)

- Wrap normal text and lists at **max 80 columns** (for readability in
  terminals and editors).
- **Exceptions**: Tables and code blocks (```` ``` ````) can exceed 80
  columns when formatting requires it (e.g. trees, alignment).
- Use standard Markdown: `**bold**`, `` `inline code` ``, `##` headings,
  `-` or numbered lists, fenced code blocks with language hints
  (```` ```cpp ````, ```` ```sh ````).
- Keep examples concise, up-to-date, and self-documenting.
- Do not use em dashes (`—`). Use a colon or rewrite the sentence.
- Each shell command gets its own fenced code block; do **not** combine
  multiple commands into one block. Precede each block with a short
  plain-text label describing what the command does:

  Setup build:
  ```sh
  cmake -S . -B build -G Ninja
  ```

- This file (`AGENTS.md`) follows its own rules.

## Source Layout

```text
ctdd/
  str.h / str.c        Pure string utilities (no dependencies)
  report.h / report.c  Formats a value and calls log_message()
  logger.h / logger.c  Real log_message via printf to stdout
main.c                 Entry point
tests/
  test_str.c           Unity state-based tests for ctdd/str
  test_report.c        Interaction-based tests using CMock
deps/
  FindUnity.cmake      Fetches Unity v2.6.1 via ZIP
  FindCMock.cmake      Fetches CMock v2.6.0 via ZIP
```

## TDD Workflow

This project follows Red-Green-Refactor. All changes to testable source
files under `ctdd/` should be test-driven: write a failing test first,
then implement.

### Adding a new module

1. Create `ctdd/<module>.h` with the public prototype.
2. Create `tests/test_<module>.c`. Set CMock expectations for any
dependency calls, then assert the result.
3. Register the test in `tests/CMakeLists.txt`:

```cmake
add_executable(test_module test_module.c)
target_include_directories(test_module PRIVATE "${CMAKE_SOURCE_DIR}")
target_link_libraries(test_module PRIVATE ctdd_module Unity::Unity CMock::CMock)
target_compile_features(test_module PRIVATE c_std_23)
cmock_generate_mock(test_module "${CMAKE_SOURCE_DIR}/ctdd/dep.h")
add_test(NAME test_module COMMAND test_module)
list(APPEND TEST_TARGETS test_module)
```

4. Stub `ctdd/<module>.c` with a dummy return, confirm RED, implement,
confirm GREEN:

```sh
ninja -C build check
```

### Mocking a dependency

Use `cmock_generate_mock` in the test target to generate a mock from a
header. Include `Mock<name>.h` in the test and use the generated API:

```c
#include "Mockdep.h"

void setUp(void)    { Mockdep_Init(); }
void tearDown(void) { Mockdep_Verify(); Mockdep_Destroy(); }

void test_something(void) {
    dep_fn_ExpectAndReturn(arg, expected);
    TEST_ASSERT_TRUE(module_do_thing());
}
```

## Behavioral Guidelines

Reduce common LLM coding mistakes. Bias toward caution over speed.
For trivial tasks, use judgment.

### Think Before Coding

- State assumptions explicitly. If uncertain, ask.
- If multiple interpretations exist, present them, don't pick silently.
- If something is unclear, stop. Name what's confusing. Ask.

### Simplicity First

Minimum code that solves the problem. Nothing speculative.

- No features beyond what was asked.
- No abstractions for single-use code.
- If you write 200 lines and it could be 50, rewrite it.

### Surgical Changes

Touch only what you must. Clean up only your own mess.

- Don't "improve" adjacent code, comments, or formatting.
- Don't refactor things that aren't broken.
- Match existing style, even if you'd do it differently.
- Remove imports/variables/functions that YOUR changes made unused.
- Don't remove pre-existing dead code unless asked.

### Goal-Driven Execution

Transform tasks into verifiable goals:

- "Add validation" means: write tests for invalid inputs, then pass.
- "Fix the bug" means: write a test that reproduces it, then pass.
- "Refactor X" means: ensure tests pass before and after.

## Platform Support

The project supports Windows, Linux, macOS, Emscripten, and Android via
`Platform.cmake` and `Flags.cmake` in `deps/`.
