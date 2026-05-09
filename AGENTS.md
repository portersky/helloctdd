# AGENTS.md

## Project Overview

`cuber` is an OpenGL 3D renderer with multiple scenes.

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

Run tests — full Unity output, colored:
```sh
ninja -C build check
```

Run tests — CTest summary only:
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

### Static Libraries

The project is split into static libraries:

- **`cbt_opengl`** — OpenGL abstraction and window management (window,
  context, buffer, texture, vao, shader, descriptor)
- **`cbt_scene`** — Base scene class
- **`scenes_cube`** — Spinning cube scene implementation
- **`scenes_sphere`** — Cube-to-sphere mapped mesh with diffuse lighting

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
- Each shell command gets its own fenced code block — do **not** combine
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
  logger.h / logger.c  Real log_message — printf to stdout
main.c                 Entry point
tests/
  test_str.c           Unity state-based tests for ctdd/str
  test_report.c        Interaction-based tests using CMock
deps/
  FindUnity.cmake      Fetches Unity v2.6.1 via ZIP
  FindCMock.cmake      Fetches CMock v2.6.0 via ZIP
```

## Platform Support

The project supports Windows, Linux, Emscripten, and Android via
`Platform.cmake` and `Flags.cmake` in `deps/`.
