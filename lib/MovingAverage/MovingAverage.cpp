
#include "MovingAverage.hpp"

MovingAverage::MovingAverage() : currentAverage(0.0), count(0U)
{}

void MovingAverage::update(double newValue)
{
    currentAverage = (currentAverage * count + newValue) / (count + 1);
    count++;
}

double MovingAverage::getCurrentValue()
{
    return currentAverage;
}

void MovingAverage::reset()
{
    currentAverage = 0.0;
    count = 0U;
}
