#ifndef INCLUDE_APP
#define INCLUDE_APP

#include <time.h>
#include <stdio.h>
#include "RenderableShapes.h"
#include "PhysShapes.h"
#include "Renderer.h"
#include "DynamicArray.h"
#include "Tools.h"
#include "CollisionManager.h"

class Shape {
protected:
    Renderable* render_object;
    PhysShape* phys_object;
public:
    virtual PhysShape* getPhysObject() const {
        return phys_object;
    }
    virtual Renderable* getRenderObject() const {
        return render_object;
    }
    virtual void translateCoords() = 0;
    virtual ~Shape() = 0;
};

class Circle : public Shape {
public:
    Circle(){
        render_object = new RenderableCircle();
        phys_object = new PhysCircle();
    }

    Circle(const Vector& coords, float r, const Vector& speed, float mass){
        phys_object = new PhysCircle(coords, r, speed, mass);
        render_object = new RenderableCircle(coords, r);
    }

    virtual void translateCoords() {
        ((RenderableCircle*)render_object)->setR(((PhysCircle*)phys_object)->getR());
        ((RenderableCircle*)render_object)->setCenter(((PhysCircle*)phys_object)->getCenter());
    }

    virtual ~Circle() {
        delete phys_object;
        delete render_object;
    }
};

class Rect : public Shape {
public:
    Rect(){
        render_object = new RenderableRect();
        phys_object = new PhysCircle();
    }

    Rect(const Vector& pos, const Vector& size, const Vector& speed, float mass){
        phys_object = new PhysRect(pos, size, speed, mass);
        render_object = new RenderableRect(pos, size);
    }

    virtual void translateCoords() {
        ((RenderableRect*)render_object)->setPos(((PhysCircle*)phys_object)->getCenter() - Vector(((PhysCircle*)phys_object)->getR(), ((PhysCircle*)phys_object)->getR()));
        ((RenderableRect*)render_object)->setSize(2 * Vector((((PhysCircle*)phys_object)->getR()), (((PhysCircle*)phys_object)->getR())));
    }

    virtual ~Rect() {
        delete phys_object;
        delete render_object;
    }
};

class Wall : public Shape {
public:
    Wall () {
    }

    Wall (const Vector& pos1, const Vector& pos2) {
        phys_object = new PhysWall(pos1, pos2, Vector(0, 0), 0);
        render_object = new RenderLine(pos1, pos2);
    }

    virtual void translateCoords() {
        ((RenderLine*)render_object)->setFirstPoint(((PhysWall*)phys_object)->getFirstPoint());
        ((RenderLine*)render_object)->setSecondPoint(((PhysWall*)phys_object)->getSecondPoint());
    }

    virtual ~Wall() {
        delete phys_object;
        delete render_object;
    }
};

class App {
private:
    friend Renderer;
    Renderer* renderer;
    CollisionManager* manager;
    DynamicArray<Renderable*> renderable_objects;
    DynamicArray<PhysShape*>  phys_objects;
    DynamicArray<Shape*>  objects;
    bool running;

    int width;
    int height;

    void parseEvent(int event);

public:
    App();

    void run();

    ~App();
};

#endif