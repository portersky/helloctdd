// Template placeholder: replace with your own entry point.
// Remove or rename this file when scaffolding a new project.

#include "ctdd/str.h"
#include "ctdd/report.h"
#include "ctdd/logger.h"

int main([[maybe_unused]]int argc, [[maybe_unused]]char const* argv[]) {
    char const* words[] = { "hello", "world", "ctdd", "tdd" };
    int n = 4, tdd_words = 0;
    for (int i = 0; i < n; i++) {
        if (str_ends_with(words[i], "tdd"))
            tdd_words++;
    }
    report_value("words ending in 'tdd'", tdd_words);

    char upper[32];
    str_upper(upper, "hello, tdd", sizeof(upper));
    log_info(upper);

    return 0;
}
