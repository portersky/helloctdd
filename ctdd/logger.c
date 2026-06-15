#include <stdio.h>

#include "ctdd/logger.h"
#include "ctdd/log_write.h"

static log_level s_level = LOG_DEBUG;

void logger_set_level(log_level level) { s_level = level; }

static void emit(char const* prefix, char const* msg) {
    char buf[512];
    snprintf(buf, sizeof(buf), "[%s] %s", prefix, msg);
    log_write(buf);
}

void log_debug(char const* msg) { if (s_level <= LOG_DEBUG) emit("DEBUG", msg); }
void log_info(char const* msg)  { if (s_level <= LOG_INFO)  emit("INFO",  msg); }
void log_warn(char const* msg)  { if (s_level <= LOG_WARN)  emit("WARN",  msg); }
void log_err(char const* msg)   { if (s_level <= LOG_ERROR) emit("ERROR", msg); }
