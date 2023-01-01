#pragma once

#include "Arduino.h"

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
    void toString();

private:

    void nextPtr();
    CBuffer(CBuffer& bfr);
    CBuffer operator=(CBuffer& bfr);

    T* buffer;
    unsigned int bufferSize;
    int ptr;
    unsigned int currentSize;
};

template<class T>
CBuffer<T>::CBuffer(int size) {
    bufferSize = size;
    buffer = new T[bufferSize];
    ptr = 0;
    currentSize = 0;
    clean();
}

template<class T>
CBuffer<T>::~CBuffer() {
    delete[] buffer;
}

template<class T>
void CBuffer<T>::write(T value) {
    nextPtr();
    buffer[ptr] = value;
    currentSize = min(currentSize + 1, bufferSize);
}

template<class T>
unsigned int CBuffer<T>::getBufferSize() {
    return bufferSize;
}

template<class T>
unsigned int CBuffer<T>::getCurrentSize() {
    return currentSize;
}

template<class T>
T CBuffer<T>::read(void) {
    return buffer[ptr];
}

template<class T>
void CBuffer<T>::read(T* bfr, int size) {
    int tempPtr = ptr - size + 1;
    tempPtr = tempPtr < 0 ? tempPtr + bufferSize : tempPtr;
    for (int i=0; i<size; i++) {
        bfr[i] = buffer[(tempPtr+i) % bufferSize];
    }
}

template<class T>
void CBuffer<T>::clean() {
    for (unsigned int i=0; i<bufferSize; i++) {
        buffer[i] = 0;
    }
    currentSize = 0;
}

template<class T>
void CBuffer<T>::nextPtr() {
    ++ptr %= bufferSize;
}

template<class T>
void CBuffer<T>::toString() {
    String result("-----------\n");
    result += "size: ";
    result += bufferSize;
    result += "\n";
    result += "ptr: ";
    result += ptr;
    result += "\n";
    result += "buffer: [";
    for (int i=0; i<bufferSize-1; i++) {
        result += buffer[i];
        result += ", ";
    }
    result += buffer[bufferSize-1];
    result += "]\n";
    result += "-----------\n";

    Serial.println(result);
}

