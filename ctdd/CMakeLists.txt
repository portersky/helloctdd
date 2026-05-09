add_library(ctdd_str STATIC str.c)
target_include_directories(ctdd_str PUBLIC "${CMAKE_SOURCE_DIR}")
target_compile_features(ctdd_str PRIVATE c_std_23)

# Reporter — calls log_info(); symbol resolved by the final binary
add_library(ctdd_report STATIC report.c)
target_include_directories(ctdd_report PUBLIC "${CMAKE_SOURCE_DIR}")
target_compile_features(ctdd_report PRIVATE c_std_23)

# Level-filtering logger — calls log_write(); symbol resolved by the final binary
add_library(ctdd_logger STATIC logger.c)
target_include_directories(ctdd_logger PUBLIC "${CMAKE_SOURCE_DIR}")
target_compile_features(ctdd_logger PRIVATE c_std_23)

# Real log_write implementation — linked into production binaries only
add_library(ctdd_log_write STATIC log_write.c)
target_include_directories(ctdd_log_write PUBLIC "${CMAKE_SOURCE_DIR}")
target_compile_features(ctdd_log_write PRIVATE c_std_23)
