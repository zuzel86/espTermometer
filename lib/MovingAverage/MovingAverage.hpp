

#pragma once

class MovingAverage
{
public:
    MovingAverage();

    void update(double newValue);
    double getCurrentValue();
    void reset();

private:
    double currentAverage;
    unsigned int count;
};
