---
name: cmake
description: CMake build configuration for projects scaffolded from the ctdd
  template. Use when adding modules, adding dependencies, configuring coverage
  or sanitizers, or understanding the build system.
---

# CMake Skill

> Replace `<src>/` with the project source directory (e.g. `ctdd/`, `src/`).
> Replace `<module>` with the module name (e.g. `counter`, `timer`).

## Build Commands

| Action    | Command                                                                                    |
| --------- | ------------------------------------------------------------------------------------------ |
| Configure | `cmake -S . -B build -G Ninja`                                                             |
| Build     | `ninja -C build`                                                                           |
| Run tests | `ninja -C build check`                                                                     |
| Coverage  | `cmake -S . -B build-cov -G Ninja -DENABLE_COVERAGE=ON` then `ninja -C build-cov coverage` |
| ASan      | `cmake -S . -B build-asan -G Ninja -DENABLE_ASAN=ON` then `ninja -C build-asan`            |

> ASan and coverage cannot be used together.

## Adding a Module

Modules live in `<src>/` as static libraries. Register them in
`<src>/CMakeLists.txt`:

```cmake
add_library(<src>_<module> <module>.c)
target_include_directories(<src>_<module> PUBLIC "${CMAKE_SOURCE_DIR}")
target_compile_features(<src>_<module> PRIVATE c_std_23)
```

Link into `main` in the root `CMakeLists.txt`:

```cmake
target_link_libraries(main PRIVATE ... <src>_<module>)
```

## Adding a Dependency

Dependencies are fetched via custom `Find<name>.cmake` scripts in `deps/`.
`deps/` is on `CMAKE_MODULE_PATH` so `find_package()` resolves to these
scripts first.

### 1. Create `deps/Find<name>.cmake`

Pattern (follow `deps/FindUnity.cmake` or `deps/FindCMock.cmake`):

```cmake
if (DEFINED _FIND<NAME>_INCLUDED)
    return()
endif()
set(_FIND<NAME>_INCLUDED TRUE)

set(<NAME>_VERSION "x.y.z")
message(STATUS "Fetching <name> ${<NAME>_VERSION}")

include(FetchContent)

FetchContent_Declare(
    <name>
    URL https://github.com/org/<name>/archive/refs/tags/v${<NAME>_VERSION}.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(<name>)

if (NOT TARGET <Name>::<Name>)
    add_library(<Name>::<Name> ALIAS <name>)
endif()

set(<Name>_FOUND TRUE)
```

For libraries without CMakeLists.txt (like CMock), use
`FetchContent_Populate` and compile manually. See `FindCMock.cmake`
for the pattern.

### 2. Add `find_package(<Name> REQUIRED)` to `CMakeLists.txt`

Place it after `include(Platform)` and `include(Flags)`.

### 3. Link with `<Name>::<Name>`

In `<src>/CMakeLists.txt` or `tests/CMakeLists.txt` as needed:

```cmake
target_link_libraries(<src>_<module> PRIVATE <Name>::<Name>)
```

## Platform Flags

`deps/Platform.cmake` sets these variables for conditional logic:

| Variable          | Meaning                                          |
| ----------------- | ------------------------------------------------ |
| `IS_CLANG_OR_GCC` | Clang or GCC compiler                            |
| `IS_MSVC`         | MSVC compiler                                    |
| `IS_WINDOWS`      | Windows target                                   |
| `IS_LINUX`        | Linux target                                     |
| `IS_MACOS`        | macOS target                                     |
| `IS_IOS`          | iOS target                                       |
| `IS_ANDROID`      | Android target                                   |
| `IS_EMSCRIPTEN`   | Emscripten/WASM target                           |
| `ARCH`            | Normalized arch (`amd64`, `arm64`, `wasm`, etc.) |
| `IS_AMD64`        | x86_64                                           |
| `IS_ARM64`        | arm64/aarch64                                    |

## Compiler Flags

`deps/Flags.cmake` defines `BASE_OPTIONS` (warning flags) and
`BASE_DEFINITIONS`. Apply per-target to avoid polluting fetched
dependencies:

```cmake
target_compile_options(<target> PRIVATE ${BASE_OPTIONS})
target_compile_definitions(<target> PRIVATE ${BASE_DEFINITIONS})
```

## CMock Mock Generation

`tests/CMakeLists.txt` provides a `cmock_generate_mock` helper function:

```cmake
cmock_generate_mock(<target> "<absolute-path-to-header>")
```

With optional config file:

```cmake
cmock_generate_mock(<target> "<header>" "cmock_config.yml")
```

Generated mocks go into `build/mocks/` as `Mock<name>.h/.c`.

## Coverage

Enabled via `-DENABLE_COVERAGE=ON`. Requires GCC or Clang + `gcovr`.
Report at `build-cov/coverage/index.html`. Only `<src>/` sources are
measured; Unity, CMock, and mocks are excluded.

## Sanitizers

`deps/Sanitizers.cmake` provides AddressSanitizer via `-DENABLE_ASAN=ON`.
Incompatible with coverage.
