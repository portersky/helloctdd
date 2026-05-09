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
    # MOCK_GEN_DIR for the generated header; header_dir so the generated
    # #include "logger.h" resolves without the ctdd/ prefix
    target_include_directories("${target}" PRIVATE "${MOCK_GEN_DIR}" "${header_dir}")
endfunction()

# str tests — pure functions, no mock needed
add_executable(test_str test_str.c)
target_include_directories(test_str PRIVATE "${CMAKE_SOURCE_DIR}")
target_link_libraries(test_str PRIVATE ctdd_str Unity::Unity)
target_compile_features(test_str PRIVATE c_std_23)
add_test(NAME test_str COMMAND test_str)

# report tests — CMock-generated mock for log_message
add_executable(test_report test_report.c)
target_include_directories(test_report PRIVATE "${CMAKE_SOURCE_DIR}")
target_link_libraries(test_report PRIVATE ctdd_report Unity::Unity CMock::CMock)
target_compile_features(test_report PRIVATE c_std_23)
cmock_generate_mock(test_report "${CMAKE_SOURCE_DIR}/ctdd/logger.h")
add_test(NAME test_report COMMAND test_report)
