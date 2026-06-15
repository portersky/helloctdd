#include "unity.h"

#include "ctdd/str.h"

void setUp(void) {}
void tearDown(void) {}

void test_starts_with_match(void) {
    TEST_ASSERT_TRUE(str_starts_with("hello world", "hello"));
}

void test_starts_with_no_match(void) {
    TEST_ASSERT_FALSE(str_starts_with("hello world", "world"));
}

void test_starts_with_empty_prefix(void) {
    TEST_ASSERT_TRUE(str_starts_with("hello", ""));
}

void test_ends_with_match(void) {
    TEST_ASSERT_TRUE(str_ends_with("hello world", "world"));
}

void test_ends_with_no_match(void) {
    TEST_ASSERT_FALSE(str_ends_with("hello world", "hello"));
}

void test_ends_with_full_string(void) {
    TEST_ASSERT_TRUE(str_ends_with("tdd", "tdd"));
}

void test_count_multiple(void) {
    TEST_ASSERT_EQUAL_INT(3, str_count("banana", 'a'));
}

void test_count_none(void) {
    TEST_ASSERT_EQUAL_INT(0, str_count("hello", 'z'));
}

void test_count_single(void) {
    TEST_ASSERT_EQUAL_INT(1, str_count("ctdd", 'c'));
}

void test_upper_lowercase(void) {
    char dst[16];
    str_upper(dst, "hello", sizeof(dst));
    TEST_ASSERT_EQUAL_STRING("HELLO", dst);
}

void test_upper_mixed(void) {
    char dst[16];
    str_upper(dst, "Hello World", sizeof(dst));
    TEST_ASSERT_EQUAL_STRING("HELLO WORLD", dst);
}

void test_upper_already_upper(void) {
    char dst[16];
    str_upper(dst, "TDD", sizeof(dst));
    TEST_ASSERT_EQUAL_STRING("TDD", dst);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_starts_with_match);
    RUN_TEST(test_starts_with_no_match);
    RUN_TEST(test_starts_with_empty_prefix);
    RUN_TEST(test_ends_with_match);
    RUN_TEST(test_ends_with_no_match);
    RUN_TEST(test_ends_with_full_string);
    RUN_TEST(test_count_multiple);
    RUN_TEST(test_count_none);
    RUN_TEST(test_count_single);
    RUN_TEST(test_upper_lowercase);
    RUN_TEST(test_upper_mixed);
    RUN_TEST(test_upper_already_upper);
    return UNITY_END();
}
