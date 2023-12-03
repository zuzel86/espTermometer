#pragma once

#include <algorithm>

template<class T>
class CBuffer {
public:
    CBuffer(int size);
    ~CBuffer();

    void write(T);
    unsigned int getBufferSize();
    unsigned int getCurrentSize();
    T read(void);
    void read(T* bfr, int size);
    void clean();

private:

    void nextPtr();
    CBuffer(CBuffer& bfr);
    CBuffer operator=(CBuffer& bfr);

    T* buffer;
    unsigned int bufferSize;
    int ptr;
    unsigned int currentSize;
};

#include "impl/cbuffer.tpp"
