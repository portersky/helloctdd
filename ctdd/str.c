#include "ctdd/str.h"
#include <string.h>
#include <ctype.h>

int str_starts_with(char const* s, char const* prefix) {
    return strncmp(s, prefix, strlen(prefix)) == 0;
}

int str_ends_with(char const* s, char const* suffix) {
    size_t sl = strlen(s);
    size_t xl = strlen(suffix);
    if (xl > sl) return 0;
    return strcmp(s + sl - xl, suffix) == 0;
}

int str_count(char const* s, char c) {
    int n = 0;
    for (; *s; s++) if (*s == c) n++;
    return n;
}

void str_upper(char* dst, char const* src, size_t n) {
    size_t i;
    for (i = 0; i < n - 1 && src[i]; i++)
        dst[i] = (char)toupper((unsigned char)src[i]);
    dst[i] = '\0';
}
