#pragma once

typedef enum { LOG_DEBUG = 0, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_NONE } log_level;

void logger_set_level(log_level level);
void log_debug(char const* msg);
void log_info(char const* msg);
void log_warn(char const* msg);
void log_err(char const* msg);
