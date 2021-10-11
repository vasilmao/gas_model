#ifndef INCLUDE_RENDERABLE
#define INCLUDE_RENDERABLE

#include "Vector.h"
#include "Renderer.h"

class Renderable {
public:
    Renderable(){};
    Renderable(Color color) : color(color) {}
    virtual ~Renderable(){}

    virtual void render(Renderer* renderer) = 0;
    Color color = {255, 255, 255, 255};
};

class RenderableCircle : public Renderable {
private:
    Vector center;
    float radius;
public:
    RenderableCircle(){}
    RenderableCircle(const Vector& center, const float r, Color color) : Renderable(color), center(center), radius(r) {}

    virtual void render(Renderer* renderer) {
        // printf("drawing circle!!! center = (%f, %f), r = %f\n", center.getX(), center.getY(), radius);
        renderer->drawFilledCircle(center, radius, color);
    }

    const Vector& getCenter() const {
        return center;
    }

    void setCenter(const Vector& new_center) {
        center = new_center;
    }

    float getR() const {
        return radius;
    }

    void setR(const float new_r) {
        radius = new_r;
    }
    
};

class RenderableRect : public Renderable {
private:
    Vector pos;
    Vector size;
public:
    RenderableRect(){}
    RenderableRect(const Vector& pos, const Vector& size, Color color) : Renderable(color), pos(pos), size(size) {}

    const Vector& getPos() const {
        return pos;
    }

    const Vector& getSize() const {
        return size;
    }

    void setPos(const Vector& new_pos) {
        pos = new_pos;
    }

    void setSize(const Vector& new_size) {
        size = new_size;
    }

    virtual void render(Renderer* renderer) {
        // printf("drawing rect :( pos = (%f, %f), size = (%f %f)\n", pos.getX(), pos.getY(), size.getX(), size.getY());
        renderer->drawFilledRect(pos, pos + size, color);
    }
};

class RenderLine : public Renderable {
private:
    Vector point1;
    Vector point2;
public:
    RenderLine() {}
    RenderLine(const Vector& p1, const Vector& p2, Color color) : Renderable(color), point1(p1), point2(p2) {}

    virtual void render(Renderer* renderer) {
        // printf("drawing line\n");
        renderer->drawLine(point1, point2, color);
    }

    const Vector& getFirstPoint() const {
        return point1;
    }

    const Vector& getSecondPoint() const {
        return point2;
    }

    void setFirstPoint(const Vector& new_point) {
        point1 = new_point;
    }

    void setSecondPoint(const Vector& new_point) {
        point2 = new_point;
    }

};

#endif