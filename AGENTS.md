# AGENTS.md

## Project Overview

`ctdd` is a starter template for C and C++ projects. Its small C23
application and test suite demonstrate test-driven development with
Unity and CMock. This repository is template infrastructure plus
replaceable examples, not a production library or a specification for
future application features.

The checked-in modules and tests are currently C-only. Unity is used for
the test examples; CMock demonstrates mocking C interfaces and is not a
general C++ class-mocking solution. For C++-specific APIs, use Unity
tests and choose an appropriate fake or mocking approach for the derived
project.

When using this starter, rewrite `AGENTS.md` to describe the resulting
project: its purpose, layout, languages, toolchain, build and test
commands, and coding conventions. Remove starter-specific guidance that
no longer applies. Mentioning the template origin is optional; if useful,
add a brief note, for example at the end of the derived project's
`README.md`. It is not required in `AGENTS.md`.

Project-specific skills are available under `.agents/skills/`. Read the
relevant skill when its guidance applies:

- TDD workflows: `.agents/skills/tdd/SKILL.md`
- Build configuration, dependencies, coverage, and sanitizers:
  `.agents/skills/cmake/SKILL.md`
- Committing, tagging, and branching: `.agents/skills/git/SKILL.md`

## Build System

- **Generator:** Ninja
- **CMake minimum:** 3.21
- **C examples:** C23
- **C++ examples:** No C++ standard is currently set by the template.
  Choose and set the standard explicitly for any C++ target.

### Dependencies

Dependencies are managed via custom `Find*.cmake` scripts in `deps/`.
See `.agents/skills/cmake/SKILL.md` for adding new dependencies.

## Build

Configure:

```sh
cmake -S . -B build -G Ninja
```

Build:

```sh
ninja -C build
```

Run tests (full Unity output):

```sh
ninja -C build check
```

Run tests (CTest summary only):

```sh
ninja -C build test
```

Configure with coverage:

```sh
cmake -S . -B build-cov -G Ninja -DENABLE_COVERAGE=ON
```

Generate coverage report:

```sh
ninja -C build-cov coverage
```

Open `build-cov/coverage/index.html` in a browser to view results.

Run the application:

```sh
./build/main
```

On Windows use `./build/main.exe`.

See `.agents/skills/cmake/SKILL.md` for coverage and sanitizer details.

## Coding Conventions

### C

- **Language:** C23 for the checked-in examples
- **East const** (e.g. `char const*` not `const char*`)
- **4-space indentation**
- `<>` includes for system headers (C stdlib, OS, etc.)
- `""` includes for third-party and local headers
- **Naming:** `snake_case` for variables, functions, and structs
- **Naming:** `SCREAMING_SNAKE_CASE` only for macros and constants

### C++

- Follow the standard selected for the target; this template does not
  currently configure a C++ standard.
- **Trailing return type** for function signatures
  (e.g. `auto fn() -> void`)
- `auto` for obvious types (e.g. `auto main(...) -> int`)
- **No semicolons after closing braces** for namespaces/classes
- **Trailing return type** for all function definitions, including
  operators (e.g. `auto operator=(T&&) noexcept -> T&`)
- **Public members first** in class declarations, private members at
  the bottom

### Include Order

Both C and C++ follow the same include order:

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

See `.agents/skills/git/SKILL.md` for commit message conventions, types,
and examples.

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

### Formatting

Use Oxfmt to format or verify formatting. The repository's `.oxfmtrc.json`
sets Markdown wrapping options.

Format all Oxfmt-supported files:

```sh
npx oxfmt --write .
```

Check all Oxfmt-supported files without changing them:

```sh
npx oxfmt --check .
```

The `.` commands cover Oxfmt-supported files beyond Markdown, and the
`--write` command may also reformat source files. To format or check only
Markdown files, pass a Markdown glob instead.

Format Markdown files only:

```sh
npx oxfmt --write "**/*.md"
```

Check Markdown files only:

```sh
npx oxfmt --check "**/*.md"
```

Oxfmt is not pinned in this template, so `npx` may use a different version
over time. No package manifest or lockfile is required; pin the formatter
only if a derived project needs repeatable formatter versions.

## Source Layout

The layout below describes the current C examples, not a required
architecture for projects created from this starter.

```
ctdd/
  str.h / str.c        Pure string utilities (no dependencies)
  report.h / report.c  Formats a value and calls log_info()
  logger.h / logger.c  Log levels, emits via log_write()
  log_write.h / log_write.c  Real log_write via printf to stdout
main.c                 Entry point (template placeholder)
tests/
  test_str.c           Unity state-based tests for ctdd/str
  test_report.c        Interaction-based tests using CMock
  test_logger.c        Interaction-based tests using CMock
deps/
  FindUnity.cmake      Fetches Unity v2.6.1 via ZIP
  FindCMock.cmake      Fetches CMock v2.6.0 via ZIP
```

## Behavioral Guidelines

Reduce common LLM coding mistakes. Bias toward caution over speed.
For trivial tasks, use judgment.

### Keep Changes Template-Focused

Treat the application modules as examples of how to wire C code,
Unity, and CMock together. Do not add product-specific features or treat
the sample APIs as requirements unless explicitly asked. Keep template
changes useful to both C and C++ starter projects, and do not imply that
the current C-only examples already demonstrate C++ integration.

When adapting this repository, follow the `AGENTS.md` customization
guidance in Project Overview. Any note about the template origin is
optional and can go in the derived project's `README.md` if desired.

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
