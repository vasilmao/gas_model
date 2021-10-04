#ifndef INCLUDE_RENDERABLE
#define INCLUDE_RENDERABLE

#include "Vector.h"
#include "Renderer.h"

class Renderable {
public:
    Renderable(){};
    virtual ~Renderable(){};

    virtual void render(Renderer* renderer) {
        // printf("drawing nothing");
    }
};

class RenderableCircle : public Renderable {
private:
    Vector center;
    float radius;
public:
    RenderableCircle(){}
    RenderableCircle(const Vector& center, const float r) : center(center), radius(r) {}

    virtual void render(Renderer* renderer) {
        // printf("drawing circle\n");
        renderer->drawFilledCircle(center, radius);
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
    Vector pos1;
    Vector pos2;
public:
    RenderableRect(){}
    RenderableRect(const Vector& pos1, const Vector& pos2) : pos1(pos1), pos2(pos2) {}

    const Vector& getPos() const {
        return pos1;
    }

    Vector getSize() const {
        return pos2 - pos1;
    }

    void setPos(const Vector& new_pos) {
        pos1 = new_pos;
    }

    void setSize(const Vector& new_pos) {
        pos2 = new_pos - pos1;
    }

    virtual void render(Renderer* renderer) {
        // printf("drawing rect\n");
        renderer->drawFilledRect(pos1, pos2);
    }
};

class RenderLine : public Renderable {
private:
    Vector point1;
    Vector point2;
public:
    RenderLine() {}
    RenderLine(const Vector& p1, const Vector& p2) : point1(p1), point2(p2) {}
    virtual void render(Renderer* renderer) {
        // printf("drawing line\n");
        renderer->drawLine(point1, point2);
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