#pragma once

#include <vector>
#include <algorithm>
#include <iterator>


/**
 * @brief Template'owy bufor cykliczny
*/
template<class T>
class CBuffer {
public:
    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        explicit Iterator(CBuffer<T>* collection, size_t element) : collection_(collection), element_(element) {}

        reference operator*() const { return collection_->getElement(element_); }
        Iterator& operator++() { ++element_; return *this; }
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        friend bool operator== (const Iterator& a, const Iterator& b) {
            return a.collection_ == b.collection_ && a.element_ == b.element_;
        };
        friend bool operator!= (const Iterator& a, const Iterator& b) {
            return a.collection_ != b.collection_ || a.element_ != b.element_;
        };


    private:
        CBuffer<T>* collection_;
        size_t element_;
    };

    Iterator begin() { return Iterator(this, 0); }
    Iterator end()   { return Iterator(this, currentSize_); }

    explicit CBuffer(size_t size);
    ~CBuffer();

    /**
     * @brief Dodaje nowy element do bufora.
    */
    void write(T);

    /**
     * @brief Zwraca całkowity rozmiar bufora.
    */
    size_t getBufferSize();

    /**
     * @brief Zwraca aktualny rozmiar bufora.
    */
    size_t getCurrentSize();

    /**
     * @brief Zwraca ostatnią dodaną wartość.
    */
    T read();

    T& last();

    /**
     * @brief Zwraca do bufora bfr, size ostatnio dodanych wartości, zaczynając od najstarszej.
     * 
     * @param bfr 
    */
    void read(T* bfr, size_t size);
    void clean();

    //TODO Zastanowić się jak powinno wyglądać kopiowanie
    CBuffer(CBuffer& bfr) = delete;
    CBuffer operator=(CBuffer& bfr) = delete;

private:
    void nextPtr();
//    void prevPtr();
    T& getElement(size_t index);

    std::vector<T> buffer_;
    size_t ptr_;
    size_t currentSize_;
};

#include "impl/cbuffer.tpp"
