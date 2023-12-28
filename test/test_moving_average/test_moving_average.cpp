#include "MovingAverage.hpp"

#include <unity.h>

void setUp(void) {
}

void tearDown(void) {
    // clean stuff up here
}

void test_add_values_to_whole_buffer() {

    MovingAverage buffer(3);

    buffer.update(3.0);
    buffer.update(4.0);
    buffer.update(5.0);

    double expected = (3.0 + 4.0 + 5.0) / 3;
    TEST_ASSERT_EQUAL(expected, buffer.getCurrentValue());
}

void test_add_more_values_than_buffer_size() {

    MovingAverage buffer(3);

    buffer.update(3.0);
    buffer.update(4.0);
    buffer.update(5.0);
    buffer.update(6.0);
    buffer.update(7.0);
    buffer.update(8.0);

    double expected = (6.0 + 7.0 + 8.0) / 3;
    TEST_ASSERT_EQUAL(expected, buffer.getCurrentValue());
}

void test_add_less_values_than_buffer_size() {

    MovingAverage buffer(3);

    buffer.update(3.0);
    buffer.update(4.0);

    double expected = (3.0 + 4.0) / 2;
    TEST_ASSERT_EQUAL(expected, buffer.getCurrentValue());
}

void test_add_some_values_to_infinite_buffer() {

    MovingAverage buffer(0);

    buffer.update(3.0);
    buffer.update(4.0);
    buffer.update(5.0);
    buffer.update(6.0);

    double expected = (3.0 + 4.0 + 5.0 + 6.0) / 4;
    TEST_ASSERT_EQUAL(expected, buffer.getCurrentValue());
}


int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_add_values_to_whole_buffer);
    RUN_TEST(test_add_more_values_than_buffer_size);
    RUN_TEST(test_add_less_values_than_buffer_size);
    RUN_TEST(test_add_some_values_to_infinite_buffer);

    UNITY_END();
}
