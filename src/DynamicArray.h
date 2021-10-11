#ifndef DYNAMIC_ARRAY_INCLUDE
#define DYNAMIC_ARRAY_INCLUDE

#include <assert.h>

template<class T>
class DynamicArray {
private:
    T* array;
    int size = 0;
    int capacity = 0;
public:

    DynamicArray() {
        const int STARTING_CAPACITY = 8;
        capacity = STARTING_CAPACITY;
        array = (T*)calloc(capacity, sizeof(T));
    }

    DynamicArray(int init_capacity) {
        assert(init_capacity > 0);
        capacity = init_capacity;
        array = (T*)calloc(capacity, sizeof(T));
    }

    DynamicArray(int init_size, T init_elem) {
        capacity = init_size;
        size = init_size;
        array = (T*)calloc(capacity, sizeof(T));
        for (int i = 0; i < size; ++i) {
            array[i] = init_elem;
        }
    }

    ~DynamicArray() {
        free(array);
        //printf("dynamic array destroyed!\n");
    }

    void push_back(T& elem) {
        if (capacity == size) {
            capacity <<= 1;
            array = (T*)realloc(array, capacity * sizeof(T));
        }
        array[size++] = elem; // копирование
    }

    int length() const {
        return size;
    }

    void pop_back() {
        assert(size > 0);
        size--;
        if ((size << 2) <= capacity) {
            capacity >>= 1;
            array = realloc(array, capacity * sizeof(T));
        }
    }

    T& operator [] (int i) {
        assert(i >= 0 && i < size);
        return array[i];
    }

    T* begin() {
        return array;
    }

    T* end() {
        return array + size;
    }

};

#endif