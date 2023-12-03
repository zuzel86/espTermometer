#pragma once

#include <algorithm>

/**
 * @brief Template'owy bufor cykliczny
*/
template<class T>
class CBuffer {
public:
    CBuffer(int size);
    ~CBuffer();

    /**
     * @brief Dodaje nowy element do bufora.
    */
    void write(T);

    /**
     * @brief Zwraca całkowity rozmiar bufora.
    */
    unsigned int getBufferSize();

    /**
     * @brief Zwraca aktualny rozmiar bufora.
    */
    unsigned int getCurrentSize();

    /**
     * @brief Zwraca ostatnią dodaną wartość.
    */
    T read(void);

    /**
     * @brief Zwraca do bufora bfr, size ostatnio dodanych wartości, zaczynając od najstarszej.
     * 
     * @param bfr 
    */
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
