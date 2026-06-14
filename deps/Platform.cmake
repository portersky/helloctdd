# ==============================================================================
# Platform Detection
# ==============================================================================
# This module detects the current platform and compiler, setting IS_* variables
# that can be used throughout the build system for conditional logic.
#
# Compiler flags set:
#   IS_CLANG_OR_GCC  - TRUE if using Clang or GCC compiler
#   IS_MSVC          - TRUE if using Microsoft Visual C++ compiler
#
# Platform flags set:
#   IS_WINDOWS       - TRUE if building for Windows
#   IS_LINUX         - TRUE if building for Linux
#   IS_MACOS         - TRUE if building for macOS
#   IS_IOS           - TRUE if building for iOS
#   IS_ANDROID       - TRUE if building for Android
#   IS_EMSCRIPTEN    - TRUE if building for WebAssembly via Emscripten
#
# Architecture flags set:
#   ARCH             - Normalized architecture name (amd64, arm64, wasm, etc.)
#   IS_AMD64         - TRUE if building for x86_64/amd64
#   IS_ARM64         - TRUE if building for arm64/aarch64
# ==============================================================================

# ------------------------------------------------------------------------------
# Compiler Detection
# ------------------------------------------------------------------------------
set(IS_CLANG_OR_GCC FALSE)
set(IS_MSVC         FALSE)

if(MSVC)
    set(IS_MSVC TRUE)
elseif(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    set(IS_CLANG_OR_GCC TRUE)
endif()

# ------------------------------------------------------------------------------
# Platform Detection
# ------------------------------------------------------------------------------
set(IS_WINDOWS    FALSE)
set(IS_LINUX      FALSE)
set(IS_MACOS      FALSE)
set(IS_IOS        FALSE)
set(IS_ANDROID    FALSE)
set(IS_EMSCRIPTEN FALSE)

if(EMSCRIPTEN)
    message(STATUS "Platform: Emscripten")
    set(IS_EMSCRIPTEN TRUE)
elseif(ANDROID)
    message(STATUS "Platform: Android")
    set(IS_ANDROID TRUE)
elseif(APPLE)
    if(IOS)
        message(STATUS "Platform: iOS")
        set(IS_IOS TRUE)
    else()
        message(STATUS "Platform: macOS")
        set(IS_MACOS TRUE)
    endif()
elseif(WIN32)
    message(STATUS "Platform: Windows")
    set(IS_WINDOWS TRUE)
elseif(UNIX)
    message(STATUS "Platform: Linux")
    set(IS_LINUX TRUE)
else()
    message(FATAL_ERROR "Unknown platform!")
endif()

# ------------------------------------------------------------------------------
# Architecture Detection
# ------------------------------------------------------------------------------
set(ARCH "")
set(IS_AMD64 FALSE)
set(IS_ARM64 FALSE)

if (IS_EMSCRIPTEN)
    set(ARCH "wasm")
elseif(DEFINED CMAKE_OSX_ARCHITECTURES)
    # Cross-compile on macOS (e.g. iOS simulator on Apple Silicon)
    set(ARCH "${CMAKE_OSX_ARCHITECTURES}")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64|^arm64|^ARM64")
    set(ARCH "arm64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^x86_64|^AMD64|^x64")
    set(ARCH "amd64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^armv[0-9]")
    set(ARCH "arm")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^i[3-6]86")
    set(ARCH "x86")
else()
    set(ARCH "${CMAKE_SYSTEM_PROCESSOR}")
endif()

if(ARCH STREQUAL "amd64")
    set(IS_AMD64 TRUE)
elseif(ARCH STREQUAL "arm64")
    set(IS_ARM64 TRUE)
endif()

message(STATUS "Architecture: ${ARCH}")
