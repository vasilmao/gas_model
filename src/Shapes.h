#ifndef INCLUDE_SHAPES
#define INCLUDE_SHAPES

#include "RenderableShapes.h"
#include "PhysShapes.h"
#include "CoordSystem.h"

const float MAX_POTENTIAL_ENERGY = 5000;

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
    virtual void translateCoords(const CoordSystem* coord_system) = 0;

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
        render_object = new RenderableCircle(coords, r, {255, 255, 255, 255});
        // printf("new circle! it has renderableCircle and type_id = %d\n", type_id);
    }

    virtual void translateCoords(const CoordSystem* coord_system) {
        ((RenderableCircle*)render_object)->setR(((PhysCircle*)phys_object)->getR());
        ((RenderableCircle*)render_object)->setCenter(coord_system->translateToAbsolute(((PhysCircle*)phys_object)->getCenter()));
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
        render_object = new RenderableRect(pos, size, {255, 255, 255, 255});
        potential_energy = init_potential_energy;
    }

    virtual void translateCoords(const CoordSystem* coord_system) {
        ((RenderableRect*)render_object)->setPos(coord_system->translateToAbsolute(((PhysCircle*)phys_object)->getCenter() - Vector(((PhysCircle*)phys_object)->getR(), ((PhysCircle*)phys_object)->getR())));
        ((RenderableRect*)render_object)->setSize(2 * Vector((((PhysCircle*)phys_object)->getR()), (((PhysCircle*)phys_object)->getR())));
        float color_percent = potential_energy > MAX_POTENTIAL_ENERGY ? 1 : potential_energy / MAX_POTENTIAL_ENERGY;
        ((RenderableRect*)render_object)->color = {255, (unsigned char)(255 * (1 - color_percent)), (unsigned char)(255 * (1 - color_percent)), 255};

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
    float potential_energy = 0;
    Wall () : Shape(MoleculeType::Wall){
        type_id = MoleculeType::Wall;
    }

    Wall (const Vector& pos1, const Vector& pos2) : Shape(MoleculeType::Wall) {
        type_id = MoleculeType::Wall;
        phys_object = new PhysWall(pos1, pos2, Vector(0, 0), 0);
        render_object = new RenderLine(pos1, pos2, {0, 0, 0, 255});
    }

    virtual void translateCoords(const CoordSystem* coord_system) {
        ((RenderLine*)render_object)->setFirstPoint(coord_system->translateToAbsolute(((PhysWall*)phys_object)->getFirstPoint()));
        ((RenderLine*)render_object)->setSecondPoint(coord_system->translateToAbsolute(((PhysWall*)phys_object)->getSecondPoint()));
        float color_percent = potential_energy > MAX_POTENTIAL_ENERGY ? 1 : potential_energy / MAX_POTENTIAL_ENERGY;
        ((RenderLine*)render_object)->color = {(unsigned char)(255 * color_percent), 0, 0, 255};
    }

    virtual ~Wall() {
        delete phys_object;
        delete render_object;
    }
};

#endif