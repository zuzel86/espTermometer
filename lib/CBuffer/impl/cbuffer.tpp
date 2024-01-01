

template<class T>
CBuffer<T>::CBuffer(size_t size) : buffer_(size),
                                   ptr_(buffer_.size()-1),
                                   currentSize_(0) {
    clean();
}

template<class T>
CBuffer<T>::~CBuffer() = default;

template<class T>
void CBuffer<T>::write(T value) {
    auto bufferSize = buffer_.size();

    nextPtr();
    buffer_[ptr_] = value;
    currentSize_ = std::min(currentSize_ + 1, bufferSize);
}

template<class T>
size_t CBuffer<T>::getBufferSize() {
    return buffer_.size();
}

template<class T>
size_t CBuffer<T>::getCurrentSize() {
    return currentSize_;
}

template<class T>
T CBuffer<T>::read() {
    return buffer_[ptr_];
}

// TODO usunąć - Metoda tymczasowa do czasu wprowadzenia operatora []
template<class T>
T &CBuffer<T>::last() {
    return buffer_[ptr_];
}

template<class T>
void CBuffer<T>::read(T* bfr, size_t size) {
    auto bufferSize = buffer_.size();

    int tempPtr = ptr_ - size + 1;
    tempPtr = tempPtr < 0 ? tempPtr + bufferSize : tempPtr;
    for (int i=0; i<size; i++) {
        bfr[i] = buffer_[(tempPtr + i) % bufferSize];
    }
}

template<class T>
void CBuffer<T>::clean() {
    for (unsigned int i=0; i < buffer_.size(); i++) {
        buffer_[i] = 0;
    }
    currentSize_ = 0;
}

template<class T>
void CBuffer<T>::nextPtr() {
    ++ptr_ %= buffer_.size();
}

//template<class T>
//void CBuffer<T>::prevPtr() {
//    ptr_ = ptr_ == 0 ? buffer_.size() -1 : ptr_ --;
//}

template<class T>
T& CBuffer<T>::getElement(size_t index) {

    if (index >= buffer_.size()) {
        return *(buffer_.end());
    }

    int delay = buffer_.size() - index;
    return buffer_[(ptr_ + delay) % buffer_.size()];
}
