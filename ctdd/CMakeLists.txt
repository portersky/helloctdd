add_library(ctdd_str STATIC str.c)
target_include_directories(ctdd_str PUBLIC "${CMAKE_SOURCE_DIR}")
target_compile_features(ctdd_str PRIVATE c_std_23)

# Reporter — calls log_message(); symbol resolved by the final binary
add_library(ctdd_report STATIC report.c)
target_include_directories(ctdd_report PUBLIC "${CMAKE_SOURCE_DIR}")
target_compile_features(ctdd_report PRIVATE c_std_23)

# Real log_message implementation — linked into production binaries only
add_library(ctdd_logger STATIC logger.c)
target_include_directories(ctdd_logger PUBLIC "${CMAKE_SOURCE_DIR}")
target_compile_features(ctdd_logger PRIVATE c_std_23)
