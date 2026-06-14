# ==============================================================================
# Sanitizers
# ==============================================================================
# AddressSanitizer (ASan) support.
# Works with GCC/Clang on Linux and macOS, Clang on Windows (requires
# compiler-rt with sanitizers), and MSVC on Windows (/fsanitize=address).
#
# Usage: cmake -DENABLE_ASAN=ON ...
# ==============================================================================

option(ENABLE_ASAN "Build with AddressSanitizer" OFF)

if (ENABLE_ASAN)
    if (ENABLE_COVERAGE)
        message(FATAL_ERROR "ENABLE_ASAN and ENABLE_COVERAGE cannot be used together")
    endif()

    if (MSVC)
        add_compile_options(/fsanitize=address)
        message(STATUS "ASan: enabled (MSVC)")
    elseif (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address)
        message(STATUS "ASan: enabled (${CMAKE_C_COMPILER_ID})")
    else()
        message(FATAL_ERROR "ENABLE_ASAN: unsupported compiler ${CMAKE_C_COMPILER_ID}")
    endif()
endif()
