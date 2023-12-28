#pragma once

#include <algorithm>

//TODO Poprawić c-style pointery na smartpointery.

/**
 * @brief Template'owy bufor cykliczny
*/
template<class T>
class CBuffer {
public:
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

    T* buffer;
    unsigned int bufferSize;
    int ptr;
    unsigned int currentSize;
};

#include "impl/cbuffer.tpp"
