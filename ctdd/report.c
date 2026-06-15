#include <stdio.h>

#include "ctdd/report.h"
#include "ctdd/logger.h"

void report_value(char const* label, int value) {
    char buf[256];
    snprintf(buf, sizeof(buf), "%s: %d", label, value);
    log_info(buf);
}
