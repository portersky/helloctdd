find_package(Unity REQUIRED)
find_package(CMock REQUIRED)

set(MOCK_GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/mocks")
file(MAKE_DIRECTORY "${MOCK_GEN_DIR}")

# Generate a CMock mock from a header and attach it to a target.
# Usage: cmock_generate_mock(<target> <absolute-path-to-header>)
# CMock names generated files Mock<Name>.h/.c (capital M, no separator)
function(cmock_generate_mock target header)
    if (NOT RUBY_EXECUTABLE)
        message(FATAL_ERROR "Ruby is required for CMock generation")
    endif()
    get_filename_component(name "${header}" NAME_WE)
    get_filename_component(header_dir "${header}" DIRECTORY)
    set(mock_src "${MOCK_GEN_DIR}/Mock${name}.c")
    set(mock_hdr "${MOCK_GEN_DIR}/Mock${name}.h")
    add_custom_command(
        OUTPUT  "${mock_src}" "${mock_hdr}"
        COMMAND "${RUBY_EXECUTABLE}" "${CMOCK_SCRIPT}"
                "--mock_path=${MOCK_GEN_DIR}"
                "${header}"
        DEPENDS "${header}"
        COMMENT "CMock: generating Mock${name}"
        VERBATIM
    )
    target_sources("${target}" PRIVATE "${mock_src}")
    target_include_directories("${target}" PRIVATE "${MOCK_GEN_DIR}" "${header_dir}")
endfunction()

set(TEST_TARGETS "")

# str tests — pure functions, no mock needed
add_executable(test_str test_str.c)
target_include_directories(test_str PRIVATE "${CMAKE_SOURCE_DIR}")
target_link_libraries(test_str PRIVATE ctdd_str Unity::Unity)
target_compile_features(test_str PRIVATE c_std_23)
add_test(NAME test_str COMMAND test_str)
list(APPEND TEST_TARGETS test_str)

# report tests — mocks logger.h so log_info calls are intercepted
add_executable(test_report test_report.c)
target_include_directories(test_report PRIVATE "${CMAKE_SOURCE_DIR}")
target_link_libraries(test_report PRIVATE ctdd_report Unity::Unity CMock::CMock)
target_compile_features(test_report PRIVATE c_std_23)
cmock_generate_mock(test_report "${CMAKE_SOURCE_DIR}/ctdd/logger.h")
add_test(NAME test_report COMMAND test_report)
list(APPEND TEST_TARGETS test_report)

# logger tests — mocks log_write.h so output calls are intercepted
add_executable(test_logger test_logger.c)
target_include_directories(test_logger PRIVATE "${CMAKE_SOURCE_DIR}")
target_link_libraries(test_logger PRIVATE ctdd_logger Unity::Unity CMock::CMock)
target_compile_features(test_logger PRIVATE c_std_23)
cmock_generate_mock(test_logger "${CMAKE_SOURCE_DIR}/ctdd/log_write.h")
add_test(NAME test_logger COMMAND test_logger)
list(APPEND TEST_TARGETS test_logger)

# 'check' builds all suites and runs CTest with full Unity output.
# USES_TERMINAL keeps ANSI colors alive through Ninja's output buffering.
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND}
            --test-dir "${CMAKE_BINARY_DIR}"
            --output-on-failure
            --progress
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    USES_TERMINAL
    DEPENDS ${TEST_TARGETS}
)

if (ENABLE_COVERAGE)
    set(COVERAGE_DIR "${CMAKE_BINARY_DIR}/coverage")
    add_custom_target(coverage
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        COMMAND ${CMAKE_COMMAND} -E make_directory "${COVERAGE_DIR}"
        COMMAND ${GCOVR_EXE}
                --gcov-executable "${GCOV_EXECUTABLE}"
                --root "${CMAKE_SOURCE_DIR}"
                --filter "${CMAKE_SOURCE_DIR}/ctdd/"
                --exclude "${CMAKE_SOURCE_DIR}/tests/"
                --exclude ".*Mock.*"
                --exclude ".*unity.*"
                --exclude ".*cmock.*"
                --html-details "${COVERAGE_DIR}/index.html"
                --txt
                --print-summary
                "${CMAKE_BINARY_DIR}"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        USES_TERMINAL
        DEPENDS ${TEST_TARGETS}
        COMMENT "Coverage report: ${COVERAGE_DIR}/index.html"
    )
endif()
