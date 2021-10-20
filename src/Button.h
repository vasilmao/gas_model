#ifndef INCLUDE_BUTTON
#define INCLUDE_BUTTON

#include "Vector.h"
#include "GraphicLib.h"

class AbstractFunctor {
private:
    virtual void function() = 0;
public:
    AbstractFunctor(){};
    virtual void operator()() = 0;
};


class Button {
private:
    Vector pos;
    Vector size;
    const char* title;
    AbstractFunctor* functor;
public:
    Button(){}
    Button(const Vector& pos, const Vector& size, AbstractFunctor* new_functor, const char* title) : pos(pos), size(size), title(title) {
        // printf("button initialized, my coords are (%f, %f), (%f, %f)\n", pos.getX(), pos.getY(), size.getX(), size.getY());
        functor = new_functor;
    }
    bool onMouseTest(const Vector& mouse_pos);
    bool onClick(const Vector& mouse_pos);
    void render(float dt, Renderer* renderer);
};

#endif