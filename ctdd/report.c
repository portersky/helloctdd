#include "ctdd/report.h"
#include "ctdd/logger.h"
#include <stdio.h>

void report_value(char const* label, int value) {
    char buf[256];
    snprintf(buf, sizeof(buf), "%s: %d", label, value);
    log_info(buf);
}
