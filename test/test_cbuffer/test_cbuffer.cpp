#include "../../lib/CBuffer/cbuffer.hpp"

#include <memory>

#include <unity.h>


using cbuffer_int = CBuffer<int>;

const int BUFFER_SIZE = 5;
std::shared_ptr<cbuffer_int> buffer;

void setUp(void) {
    buffer = std::make_shared<cbuffer_int>(BUFFER_SIZE);
}

void tearDown(void) {
    // clean stuff up here
}

void test_GetBufferSize_Call_ReturnsProperSize() {

    TEST_ASSERT_EQUAL(BUFFER_SIZE, buffer->getBufferSize());
}

void test_Write_BufferNotOverflowYet_CurrentSizeIsCorrect() {

    buffer->write(1);
    buffer->write(2);
    buffer->write(3);

    TEST_ASSERT_EQUAL(3, buffer->getCurrentSize());
}

void test_Write_BufferOverflowed_CurrentSizeIsCorrect() {

    buffer->write(1);
    buffer->write(2);
    buffer->write(3);
    buffer->write(4);
    buffer->write(5);
    buffer->write(6);

    TEST_ASSERT_EQUAL(BUFFER_SIZE, buffer->getCurrentSize());
}

void test_Read_BufferNotOverflowYet_ReturnAddedValues() {

    buffer->write(1);
    buffer->write(2);
    buffer->write(3);

    TEST_ASSERT_EQUAL(3, buffer->read());

    int read_buffer[3];
    buffer->read(read_buffer, 3);
    TEST_ASSERT_EQUAL(1, read_buffer[0]);
    TEST_ASSERT_EQUAL(2, read_buffer[1]);
    TEST_ASSERT_EQUAL(3, read_buffer[2]);
}

void test_Read_BufferOverflowed_ReturnAddedValues() {

    buffer->write(1);
    buffer->write(2);
    buffer->write(3);
    buffer->write(4);
    buffer->write(5);
    buffer->write(6);

    TEST_ASSERT_EQUAL(6, buffer->read());

    int read_buffer[4];
    buffer->read(read_buffer, 4);
    TEST_ASSERT_EQUAL(3, read_buffer[0]);
    TEST_ASSERT_EQUAL(4, read_buffer[1]);
    TEST_ASSERT_EQUAL(5, read_buffer[2]);
    TEST_ASSERT_EQUAL(6, read_buffer[3]);
}

void test_begin_returnsLastAddedValueIterator() {
    buffer->write(1);
    buffer->write(2);
    buffer->write(3);

    TEST_ASSERT_EQUAL(3, *buffer->begin());

    buffer->write(4);
    buffer->write(5);
    buffer->write(6);
    buffer->write(7);

    TEST_ASSERT_EQUAL(7, *buffer->begin());
}

void test_iterator_iteratesOverCollection() {
    buffer->write(1);

    for(auto& el : *buffer) {
        TEST_ASSERT_EQUAL(1, el);
    }
}


int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_GetBufferSize_Call_ReturnsProperSize);
    RUN_TEST(test_Write_BufferNotOverflowYet_CurrentSizeIsCorrect);
    RUN_TEST(test_Write_BufferOverflowed_CurrentSizeIsCorrect);
    RUN_TEST(test_Read_BufferNotOverflowYet_ReturnAddedValues);
    RUN_TEST(test_Read_BufferOverflowed_ReturnAddedValues);
    RUN_TEST(test_begin_returnsLastAddedValueIterator);
    RUN_TEST(test_iterator_iteratesOverCollection);

    UNITY_END();
}
