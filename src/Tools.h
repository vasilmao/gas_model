#ifndef INCLUDE_TOOLS_STRUCT
#define INCLUDE_TOOLS_STRUCT
#include <stdio.h>
#include <assert.h>

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