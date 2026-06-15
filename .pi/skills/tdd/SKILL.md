---
name: tdd
description: Test-driven development with Unity and CMock. Use when adding new
  modules, writing tests, mocking dependencies, or following Red-Green-Refactor
  in projects scaffolded from the ctdd template.
---

# TDD Skill

> Replace `<src>/` with the project source directory (e.g. `ctdd/`, `src/`).
> Replace `<module>` with the module name (e.g. `counter`, `timer`).

## Quick Reference

| Action                  | Command                                                 |
| ----------------------- | ------------------------------------------------------- |
| Configure (default)     | `cmake -S . -B build -G Ninja`                          |
| Configure (coverage)    | `cmake -S . -B build-cov -G Ninja -DENABLE_COVERAGE=ON` |
| Build                   | `ninja -C build`                                        |
| Run tests (full output) | `ninja -C build check`                                  |
| Run tests (summary)     | `ninja -C build test`                                   |
| Run tests + coverage    | `ninja -C build-cov coverage`                           |

## Red-Green-Refactor

Every change to `<src>/` follows this cycle:

1. **RED** — Write a failing test. Confirm it fails.
2. **GREEN** — Write the minimum code to pass. Confirm it passes.
3. **REFACTOR** — Clean up without changing behavior. Tests still pass.

Never skip the RED step. If the test passes immediately, you wrote the test
before the implementation was missing.

## Adding a New Module

### 1. Write the header

Create `<src>/<module>.h` with the public prototype(s).

C:

```c
#pragma once
#include <stddef.h>

int fn_name(int arg);
```

C++:

```cpp
#pragma once
#include <cstddef>

auto fn_name(int arg) -> int;
```

### 2. Write the test

Create `tests/test_<module>.c`. Two patterns exist:

**State-based** (pure functions, no mocks):

```c
#include "unity.h"
#include "<src>/<module>.h"

void setUp(void) {}
void tearDown(void) {}

void test_fn_name_returns_expected(void) {
    TEST_ASSERT_EQUAL_INT(42, fn_name(1));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_fn_name_returns_expected);
    return UNITY_END();
}
```

**Interaction-based** (mocking a dependency):

```c
#include "unity.h"
#include "<src>/<module>.h"
#include "Mock<dep>.h"

void setUp(void)    { Mock<dep>_Init(); }
void tearDown(void) { Mock<dep>_Verify(); Mock<dep>_Destroy(); }

void test_fn_calls_dep(void) {
    dep_fn_ExpectAndReturn(arg, expected);
    TEST_ASSERT_TRUE(fn_name());
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_fn_calls_dep);
    return UNITY_END();
}
```

### 3. Register the test in `tests/CMakeLists.txt`

Add after existing test targets, before the `check` target:

**State-based (no mock):**

```cmake
add_executable(test_<module> test_<module>.c)
target_include_directories(test_<module> PRIVATE "${CMAKE_SOURCE_DIR}")
target_link_libraries(test_<module> PRIVATE <src>_<module> Unity::Unity)
target_compile_features(test_<module> PRIVATE c_std_23)
add_test(NAME test_<module> COMMAND test_<module>)
list(APPEND TEST_TARGETS test_<module>)
```

**Interaction-based (with mock):**

```cmake
add_executable(test_<module> test_<module>.c)
target_include_directories(test_<module> PRIVATE "${CMAKE_SOURCE_DIR}")
target_link_libraries(test_<module> PRIVATE <src>_<module> Unity::Unity CMock::CMock)
target_compile_features(test_<module> PRIVATE c_std_23)
cmock_generate_mock(test_<module> "${CMAKE_SOURCE_DIR}/<src>/<dep>.h")
add_test(NAME test_<module> COMMAND test_<module>)
list(APPEND TEST_TARGETS test_<module>)
```

### 4. Stub the implementation

Create `<src>/<module>.c` with a dummy return:

C:

```c
#include "<src>/<module>.h"

int fn_name(int arg) {
    return 0;
}
```

C++:

```cpp
#include "<src>/<module>.h"

auto fn_name(int arg) -> int {
    return 0;
}
```

Register the library in `<src>/CMakeLists.txt`:

```cmake
add_library(<src>_<module> <module>.c)
target_include_directories(<src>_<module> PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(<src>_<module> PRIVATE c_std_23)
```

### 5. Confirm RED

```sh
ninja -C build check
```

The test must fail. If it does not, something is wrong.

### 6. Implement and confirm GREEN

Write the real implementation. Run tests again:

```sh
ninja -C build check
```

All tests must pass.

## CMock Patterns

### Expecting a call with arguments

```c
dep_fn_Expect(arg1, arg2);
```

### Expecting a call and returning a value

```c
dep_fn_ExpectAndReturn(arg, expected_value);
```

### Verifying a call was NOT made

Do not set any `_Expect`. If the function is called, CMock fails
automatically in `tearDown`.

### Multiple calls

```c
dep_fn_ExpectAndReturn(first_call_arg, first_return);
dep_fn_ExpectAndReturn(second_call_arg, second_return);
```

Calls are matched in order.

## Test Naming

- `test_<function>_<scenario>`
- Use present tense: `test_upper_converts_lowercase`
- Cover: happy path, edge cases, boundaries, and error conditions

## Unity Assertions

| Assertion                        | Use              |
| -------------------------------- | ---------------- |
| `TEST_ASSERT_TRUE(x)`            | Boolean true     |
| `TEST_ASSERT_FALSE(x)`           | Boolean false    |
| `TEST_ASSERT_EQUAL_INT(a, b)`    | Integer equality |
| `TEST_ASSERT_EQUAL_STRING(a, b)` | String equality  |
| `TEST_ASSERT_NULL(p)`            | Null pointer     |
| `TEST_ASSERT_NOT_NULL(p)`        | Non-null pointer |

## Coverage

To check coverage after tests:

```sh
ninja -C build-cov coverage
```

Open `build-cov/coverage/index.html` in a browser.

## Checklist

Before considering a task done:

- [ ] Header declares the public API
- [ ] Test covers at least one happy path, one edge case
- [ ] Test registered in `tests/CMakeLists.txt`
- [ ] Library registered in `<src>/CMakeLists.txt` (if new module)
- [ ] `ninja -C build check` passes with zero failures
- [ ] No semicolons after closing braces
- [ ] Trailing return type on all functions
- [ ] East const (`char const*`)
- [ ] `snake_case` naming
