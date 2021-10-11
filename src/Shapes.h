#ifndef INCLUDE_SHAPES
#define INCLUDE_SHAPES

#include "RenderableShapes.h"
#include "PhysShapes.h"

class Shape {
public:
    enum class MoleculeType {
        NoTypeMolecule = -1,
        CircleMolecule,
        RectangleMolecule,
        Wall
    };
protected:
    Renderable* render_object;
    PhysShape* phys_object;
    MoleculeType type_id = MoleculeType::NoTypeMolecule;
    Shape(MoleculeType type_id) : type_id(type_id) {}
public:
    
    virtual PhysShape* getPhysObject() const {
        return phys_object;
    }
    virtual Renderable* getRenderObject() const {
        return render_object;
    }
    virtual void translateCoords() = 0;

    virtual int getType() const {
        return static_cast<int> (type_id);
    }

    virtual ~Shape() = 0;
};

class Circle : public Shape {
public:
    Circle() : Shape(MoleculeType::CircleMolecule) {
        render_object = new RenderableCircle();
        phys_object = new PhysCircle();
    }

    Circle(const Vector& coords, float r, const Vector& speed, float mass) : Shape(MoleculeType::CircleMolecule) {
        phys_object = new PhysCircle(coords, r, speed, mass);
        render_object = new RenderableCircle(coords, r);
        // printf("new circle! it has renderableCircle and type_id = %d\n", type_id);
    }

    virtual void translateCoords() {
        ((RenderableCircle*)render_object)->setR(((PhysCircle*)phys_object)->getR());
        ((RenderableCircle*)render_object)->setCenter(((PhysCircle*)phys_object)->getCenter());
    }

    virtual const Vector& getReactionCenter() {
        return ((PhysCircle*)(phys_object))->getCenter();
    }

    virtual float getReactionRadius() {
        return ((PhysCircle*)(phys_object))->getR();
    }

    virtual ~Circle() {
        delete phys_object;
        delete render_object;
    }
};

class Rect : public Shape {
public:
    float potential_energy = 0;
    Rect() : Shape(MoleculeType::RectangleMolecule) {
        type_id = MoleculeType::RectangleMolecule;
        render_object = new RenderableRect();
        phys_object = new PhysCircle();
    }

    Rect(const Vector& pos, const Vector& size, const Vector& speed, float mass, float init_potential_energy = 0) : Shape(MoleculeType::RectangleMolecule) {
        type_id = MoleculeType::RectangleMolecule;
        phys_object = new PhysCircle(pos + (size / 2), size.getX() / 2, speed, mass);
        render_object = new RenderableRect(pos, size);
        potential_energy = init_potential_energy;
    }

    virtual void translateCoords() {
        ((RenderableRect*)render_object)->setPos(((PhysCircle*)phys_object)->getCenter() - Vector(((PhysCircle*)phys_object)->getR(), ((PhysCircle*)phys_object)->getR()));
        ((RenderableRect*)render_object)->setSize(2 * Vector((((PhysCircle*)phys_object)->getR()), (((PhysCircle*)phys_object)->getR())));
    }

    virtual Vector getReactionPos() {
        float r = ((PhysCircle*)(phys_object))->getR();
        return ((PhysCircle*)(phys_object))->getCenter() - Vector(r, r);
    }

    virtual Vector getReactionSize() {
        float r = ((PhysCircle*)(phys_object))->getR();
        return Vector(r, r) * 2;
    }

    virtual ~Rect() {
        delete phys_object;
        delete render_object;
    }
};

class Wall : public Shape {
public:
    Wall () : Shape(MoleculeType::Wall){
        type_id = MoleculeType::Wall;
    }

    Wall (const Vector& pos1, const Vector& pos2) : Shape(MoleculeType::Wall) {
        type_id = MoleculeType::Wall;
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

#endif