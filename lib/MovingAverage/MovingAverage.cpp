
#include "MovingAverage.hpp"

#include <iostream>
using namespace std;

MovingAverage::MovingAverage(unsigned int bufferSize) : buffer_(bufferSize == 0 ? 1 : bufferSize), 
                                                        infinity_bufer_(bufferSize == 0),
                                                        count_(0)
{
}

void MovingAverage::update(double newValue)
{
    cout << "Update: " << newValue << endl;
    
    if (infinity_bufer_) {
//         If buffer is no limit buffer, we store current average besides current sum.
        auto& currentAverage = buffer_.last();
        currentAverage = (currentAverage * count_ + newValue) / (count_ + 1);
        count_++;
    } else {
        buffer_.write(newValue);;
    }

}

double MovingAverage::getCurrentValue()
{
    // TODO Uprościć po dodaniu iterator'ów do kolekcji CBuffer
    if (infinity_bufer_) {
        return buffer_.read();
    } else {
        double sum = 0.0;
        auto* bfr = new double[buffer_.getCurrentSize()];
        buffer_.read(bfr, buffer_.getCurrentSize());
        for(int i=0; i<buffer_.getCurrentSize(); i++) {
            cout << "i: " << i << " ---> " << bfr[i] << endl;
            sum += bfr[i];
        }
        delete[] bfr;

        return sum / buffer_.getCurrentSize();
    }
}

void MovingAverage::reset()
{
    buffer_.clean();
    count_ = 0U;
}
