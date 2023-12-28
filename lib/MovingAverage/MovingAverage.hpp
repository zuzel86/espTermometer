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
    CBuffer<double> buffer_;
    bool infinity_bufer_;
    unsigned int count_ = 0;
};
