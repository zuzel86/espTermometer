#pragma once

#include "../CBuffer/cbuffer.hpp"

class MovingAverage
{
public:
    MovingAverage(unsigned int bufferSize = 0);

    void update(double newValue);
    double getCurrentValue();
    void reset();

private:
    CBuffer<double> buffer_;        // Buffer to store data
    bool infinity_buffer_;          // Stores only average and behaves like a buffer of infinity length.
    unsigned int count_ = 0;        // Added elements counter
};
