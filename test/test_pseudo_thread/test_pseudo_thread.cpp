#include "pseudoThread.hpp"

#include <unity.h>

static int f1_cnt, f2_cnt = 0;
static unsigned long fake_time = 0UL;

void setUp(void) {
    // f1_cnt, f2_cnt = 0;
    // fake_time = 0UL;
}

void tearDown(void) {
    // clean stuff up here
}

void test_get_identifier_gives_different_id_each_time_is_called() {
    auto first_id = getIdentifier();
    auto second_id = getIdentifier();

    TEST_ASSERT_NOT_EQUAL(first_id, second_id);
}

void f1() { f1_cnt++; }
void f2() { f2_cnt++; }
unsigned long time_stub() {return fake_time; }

void test_second_run_before_time_left() {
    auto id = getIdentifier();

    fake_time = 100UL;
    executeIfTimeLeft(id, 15UL, &f1, &f2, &time_stub);

    fake_time = 105UL;
    executeIfTimeLeft(id, 15UL, &f1, &f2, &time_stub);

    TEST_ASSERT_EQUAL(1, f1_cnt);
    TEST_ASSERT_EQUAL(1, f2_cnt);
}

void test_second_run_after_time_left() {
    auto id = getIdentifier();
    f1_cnt = f2_cnt = 0;

    fake_time = 100UL;
    executeIfTimeLeft(id, 15UL, &f1, &f2, &time_stub);

    fake_time = 120UL;
    executeIfTimeLeft(id, 15UL, &f1, &f2, &time_stub);

    TEST_ASSERT_EQUAL(2, f1_cnt);
    TEST_ASSERT_EQUAL(0, f2_cnt);
}

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_get_identifier_gives_different_id_each_time_is_called);
    RUN_TEST(test_second_run_before_time_left);
    RUN_TEST(test_second_run_after_time_left);

    UNITY_END();
}
