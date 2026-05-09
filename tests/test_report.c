#include "unity.h"
#include "ctdd/report.h"
#include "Mocklogger.h"

void setUp(void)    { Mocklogger_Init(); }
void tearDown(void) { Mocklogger_Verify(); Mocklogger_Destroy(); }

void test_report_formats_label_and_value(void) {
    log_message_Expect("count: 42");
    report_value("count", 42);
}

void test_report_negative_value(void) {
    log_message_Expect("score: -5");
    report_value("score", -5);
}

void test_report_zero(void) {
    log_message_Expect("total: 0");
    report_value("total", 0);
}

void test_report_calls_log_exactly_once(void) {
    log_message_Expect("x: 1");
    report_value("x", 1);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_report_formats_label_and_value);
    RUN_TEST(test_report_negative_value);
    RUN_TEST(test_report_zero);
    RUN_TEST(test_report_calls_log_exactly_once);
    return UNITY_END();
}
