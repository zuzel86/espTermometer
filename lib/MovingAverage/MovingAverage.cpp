
#include "MovingAverage.hpp"

#include <numeric>

using namespace std;

MovingAverage::MovingAverage(unsigned int bufferSize) : buffer_(bufferSize == 0 ? 1 : bufferSize),
                                                        infinity_buffer_(bufferSize == 0),
                                                        count_(0)
{
}

void MovingAverage::update(double newValue)
{
    if (infinity_buffer_) {
        // If buffer is no limit buffer, we store current average besides current sum.
        auto& currentAverage = buffer_.last();
        currentAverage = (currentAverage * count_ + newValue) / (count_ + 1);
        count_++;
    } else {
        buffer_.write(newValue);;
    }
}

double MovingAverage::getCurrentValue()
{
    if (infinity_buffer_) {
        return buffer_.read();
    } else {
        auto sum = std::accumulate(buffer_.begin(), buffer_.end(), 0.0);
        return sum / static_cast<double>(buffer_.getCurrentSize());
    }
}

void MovingAverage::reset()
{
    buffer_.clean();
    count_ = 0U;
}
