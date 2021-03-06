#ifndef INCLUDE_MOLECULES
#define INCLUDE_MOLECULES

#include "RenderableShapes.h"
#include "PhysShapes.h"
#include "CoordSystem.h"

const float MAX_POTENTIAL_ENERGY = 10000;

class Shape {
public:
    enum class MoleculeType {
        NoTypeMolecule = -1,
        CircleMolecule,
        RectangleMolecule,
        Wall
    };
protected:
    MoleculeType type_id = MoleculeType::NoTypeMolecule;
    float potential_energy = 0;
    Renderable* render_object;
    PhysShape* phys_object;
    Shape(MoleculeType type_id, float potential_energy = 0) : type_id(type_id), potential_energy(potential_energy) {}
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

    virtual void addPotentialEnergy(float delta) {
        potential_energy += delta;
    }

    virtual float getPotentialEnergy() {
        return potential_energy;
    }

    virtual void setPotentialEnergy(float new_pe) {
        potential_energy = new_pe;
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
        (dynamic_cast<RenderableRect*>(render_object))->setPos(coord_system->translateToAbsolute((dynamic_cast<PhysCircle*>(phys_object))->getCenter() - Vector((dynamic_cast<PhysCircle*>(phys_object))->getR(), (dynamic_cast<PhysCircle*>(phys_object))->getR())));
        (dynamic_cast<RenderableRect*>(render_object))->setSize(2 * Vector(((dynamic_cast<PhysCircle*>(phys_object))->getR()), ((dynamic_cast<PhysCircle*>(phys_object))->getR())));
        float color_percent = potential_energy > MAX_POTENTIAL_ENERGY ? 1 : potential_energy / MAX_POTENTIAL_ENERGY;
        (dynamic_cast<RenderableRect*>(render_object))->color = {255, (unsigned char)(255 * (1 - color_percent)), (unsigned char)(255 * (1 - color_percent)), 255};

    }

    virtual Vector getReactionPos() {
        float r = (dynamic_cast<PhysCircle*>(phys_object))->getR();
        return (dynamic_cast<PhysCircle*>(phys_object))->getCenter() - Vector(r, r);
    }

    virtual Vector getReactionSize() {
        float r = (dynamic_cast<PhysCircle*>(phys_object))->getR();
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
        render_object = new RenderLine(pos1, pos2, {0, 0, 0, 255});
    }

    virtual void translateCoords(const CoordSystem* coord_system) {
        (dynamic_cast<RenderLine*>(render_object))->setFirstPoint(coord_system->translateToAbsolute((dynamic_cast<PhysWall*>(phys_object))->getFirstPoint()));
        (dynamic_cast<RenderLine*>(render_object))->setSecondPoint(coord_system->translateToAbsolute((dynamic_cast<PhysWall*>(phys_object))->getSecondPoint()));
        float color_percent = potential_energy > MAX_POTENTIAL_ENERGY ? 1 : potential_energy / MAX_POTENTIAL_ENERGY;
        (dynamic_cast<RenderLine*>(render_object))->color = {(unsigned char)(255 * color_percent), 0, 0, 255};
    }

    virtual ~Wall() {
        delete phys_object;
        delete render_object;
    }

    virtual void addPotentialEnergy(float delta) {
        potential_energy += delta;
    }

    virtual float getPotentialEnergy() {
        return potential_energy;
    }

    virtual void setPotentialEnergy(float new_pe) {
        potential_energy = new_pe;
    }
};

#endif