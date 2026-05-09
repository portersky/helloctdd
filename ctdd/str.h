#pragma once
#include <stddef.h>

int  str_starts_with(char const* s, char const* prefix);
int  str_ends_with(char const* s, char const* suffix);
int  str_count(char const* s, char c);
void str_upper(char* dst, char const* src, size_t n);
