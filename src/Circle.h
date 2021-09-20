#ifndef INCLUDE_CIRCLE
#define INCLUDE_CIRCLE

#include "Tools.h"
#include "Vector.h"

class Circle {
private:
    Vector center;
    float r = 0;
    Vector speed;
public:
    Circle(){}
    Circle(Vector center, float r, Vector speed) : center(center), r(r), speed(speed) {}
    float getR() const;
    const Vector& getCenter() const;
    void collide(Circle& other, float time);
    void collide_with_walls(const Rect2f& walls, const float time);
    void move(float time);
};
#endif