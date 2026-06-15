#include "unity.h"

#include "ctdd/logger.h"
#include "Mocklog_write.h"

void setUp(void)    { Mocklog_write_Init(); logger_set_level(LOG_DEBUG); }
void tearDown(void) { Mocklog_write_Verify(); Mocklog_write_Destroy(); }

void test_log_debug_emits_at_debug_level(void) {
    log_write_Expect("[DEBUG] hello");
    log_debug("hello");
}

void test_log_info_emits_at_debug_level(void) {
    log_write_Expect("[INFO] world");
    log_info("world");
}

void test_log_warn_emits_at_warn_level(void) {
    logger_set_level(LOG_WARN);
    log_write_Expect("[WARN] alert");
    log_warn("alert");
}

void test_log_err_emits_at_warn_level(void) {
    logger_set_level(LOG_WARN);
    log_write_Expect("[ERROR] fatal");
    log_err("fatal");
}

void test_log_debug_suppressed_at_info_level(void) {
    logger_set_level(LOG_INFO);
    log_debug("silent");
}

void test_log_info_suppressed_at_warn_level(void) {
    logger_set_level(LOG_WARN);
    log_info("silent");
}

void test_log_warn_suppressed_at_error_level(void) {
    logger_set_level(LOG_ERROR);
    log_warn("silent");
}

void test_log_none_suppresses_all(void) {
    logger_set_level(LOG_NONE);
    log_debug("silent");
    log_info("silent");
    log_warn("silent");
    log_err("silent");
}

void test_level_can_be_raised_then_lowered(void) {
    logger_set_level(LOG_ERROR);
    log_info("silent");
    logger_set_level(LOG_DEBUG);
    log_write_Expect("[INFO] now visible");
    log_info("now visible");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_log_debug_emits_at_debug_level);
    RUN_TEST(test_log_info_emits_at_debug_level);
    RUN_TEST(test_log_warn_emits_at_warn_level);
    RUN_TEST(test_log_err_emits_at_warn_level);
    RUN_TEST(test_log_debug_suppressed_at_info_level);
    RUN_TEST(test_log_info_suppressed_at_warn_level);
    RUN_TEST(test_log_warn_suppressed_at_error_level);
    RUN_TEST(test_log_none_suppresses_all);
    RUN_TEST(test_level_can_be_raised_then_lowered);
    return UNITY_END();
}
