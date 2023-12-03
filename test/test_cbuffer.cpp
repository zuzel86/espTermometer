#include <unity.h>

#include "cbuffer.hpp"


void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_failing() {

    TEST_ASSERT_TRUE(true);
}


int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_failing);

    UNITY_END();
}
