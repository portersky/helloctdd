cmake_minimum_required(VERSION 3.21)
project(ctdd VERSION 0.1.0)

# CMake configuration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/deps")

# Platform flags
include(Platform)
include(Flags)

# Example Library
add_subdirectory(ctdd)

# Main executable
add_executable(main main.c)
target_include_directories(main PRIVATE .)
target_compile_features(main PRIVATE c_std_23)
target_link_libraries(main PRIVATE ctdd_str ctdd_report ctdd_logger)

# Testing
enable_testing()
add_subdirectory(tests)

# IDE configuration
include(IDE)
