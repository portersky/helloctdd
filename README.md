# ctdd

A C23 project wired for test-driven development using
[Unity](https://github.com/ThrowTheSwitch/Unity) and
[CMock](https://github.com/ThrowTheSwitch/CMock).

All dependencies are fetched automatically via CMake `FetchContent` — no
manual installation required beyond the tools listed below.

## Requirements

| Tool                | Purpose               |
| ------------------- | --------------------- |
| CMake ≥ 3.21        | Build system          |
| Ninja               | Build backend         |
| C23 compiler        | GCC 14+, Clang 18+    |
| Ruby ≥ 3.0          | CMock mock generation |

## Build

```sh
cmake -S . -B build -G Ninja
ninja -C build
```

## Test

```sh
ninja -C build check         # full Unity output, colored
ninja -C build test          # CTest summary only
```

`check` builds all suites and runs CTest with `--output-on-failure`,
so assertion-level detail appears on any failure without running
binaries by hand.

## Run

```sh
./build/main.exe             # Windows
./build/main                 # Linux / macOS
```


## Adding a new module (TDD workflow)

### 1. Write the header

```c
// ctdd/counter.h
#pragma once
int counter_increment(int value);
```

### 2. Write a failing test

```c
// tests/test_counter.c
#include "unity.h"
#include "ctdd/counter.h"

void setUp(void) {}
void tearDown(void) {}

void test_increment_adds_one(void) {
    TEST_ASSERT_EQUAL_INT(2, counter_increment(1));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_increment_adds_one);
    return UNITY_END();
}
```

### 3. Register the test in `tests/CMakeLists.txt`

```cmake
add_executable(test_counter test_counter.c)
target_include_directories(test_counter PRIVATE "${CMAKE_SOURCE_DIR}")
target_link_libraries(test_counter PRIVATE ctdd_counter Unity::Unity)
target_compile_features(test_counter PRIVATE c_std_23)
add_test(NAME test_counter COMMAND test_counter)
list(APPEND TEST_TARGETS test_counter)
```

### 4. Add a stub, confirm RED, then implement GREEN

Stub `ctdd/counter.c` with `return 0;`, run tests to see the failure,
then implement the real logic and confirm they pass.

## Mocking a dependency

If your module calls an external function (e.g. `log_message`), generate
a mock from its header and add it to the test target:

```cmake
# tests/CMakeLists.txt
add_executable(test_mymodule test_mymodule.c)
target_link_libraries(test_mymodule PRIVATE ctdd_mymodule Unity::Unity CMock::CMock)
target_compile_features(test_mymodule PRIVATE c_std_23)
cmock_generate_mock(test_mymodule "${CMAKE_SOURCE_DIR}/ctdd/logger.h")
add_test(NAME test_mymodule COMMAND test_mymodule)
```

CMock generates `Mocklogger.h` into the build directory. Include it and
use the generated API in your test:

```c
#include "Mocklogger.h"

void setUp(void)    { Mocklogger_Init(); }
void tearDown(void) { Mocklogger_Verify(); Mocklogger_Destroy(); }

void test_something_logs(void) {
    log_message_Expect("expected string");
    my_function_under_test();
}
```

The `_Expect` call records the expected argument. CMock verifies the
actual argument matches at call time using string comparison, and
`Mocklogger_Verify` confirms the expected number of calls were made.
