#pragma once

#include "../CBuffer/cbuffer.hpp"

class MovingAverage
{
public:
    explicit MovingAverage(unsigned int bufferSize = 0);

    void update(double newValue);
    double getCurrentValue();
    size_t getCurrentSize();
    void reset();

private:
    CBuffer<double> buffer_;        // Buffer to store data
    bool infinity_buffer_;          // Stores only average and behaves like a buffer of infinity length.
    size_t count_ = 0;              // Added elements counter
};
