#ifndef INCLUDE_TOOLS_STRUCT
#define INCLUDE_TOOLS_STRUCT
#include <stdio.h>
#include <assert.h>

template<class T>
const T& Max(const T& el1, const T& el2) {
    return el1 < el2 ? el2 : el1;
}

template<class T>
const T& Min(const T& el1, const T& el2) {
    return el1 < el2 ? el1 : el2;
}

struct Rect2i {
    int x;
    int y;
    int width;
    int height;
};

struct Rect2f {
    float x;
    float y;
    float width;
    float height;
};

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

template<class T>
void Swap(T* el1, T* el2) {
    T tmp = *el1;
    *el1 = *el2;
    *el2 = tmp;
}

#endif