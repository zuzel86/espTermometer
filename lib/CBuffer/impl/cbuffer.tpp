

template<class T>
CBuffer<T>::CBuffer(size_t size) {

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
    currentSize = std::min(currentSize + 1, bufferSize);
}

template<class T>
size_t CBuffer<T>::getBufferSize() {
    return bufferSize;
}

template<class T>
size_t CBuffer<T>::getCurrentSize() {
    return currentSize;
}

template<class T>
T CBuffer<T>::read() {
    return buffer[ptr];
}

// TODO usunąć - Metoda tymczasowa do czasu wprowadzenia operatora []
template<class T>
T &CBuffer<T>::last() {
    return buffer[ptr];
}

template<class T>
void CBuffer<T>::read(T* bfr, size_t size) {
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
