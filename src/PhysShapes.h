#ifndef INCLUDE_SHAPES
#define INCLUDE_SHAPES

#include "Tools.h"
#include "Vector.h"

class CollisionManager;

class PhysShape {
    friend CollisionManager;
    
public:

    enum class ShapeType {
        NO_TYPE = -1,
        Circle,
        Rectangle,
        Wall
    };

protected:

    float mass;
    Vector speed;
    ShapeType type_id = ShapeType::NO_TYPE;

// constructors
public:
    PhysShape(){};

protected:
    PhysShape(ShapeType type) : type_id(type) {};
    PhysShape(float mass, Vector speed, ShapeType type) : mass(mass), speed(speed), type_id(type) {};

// functions
public:
    virtual void move (float time) = 0;
    virtual ~PhysShape() = 0;

    virtual const Vector& getSpeed() const {
        return speed;
    }

    virtual void setSpeed(const Vector& new_speed) {
        speed = new_speed;
    }

    virtual float getMass() const {
        return mass;
    }

    virtual void setMass(float new_mass) {
        mass = new_mass;
    }

    virtual Vector getImpulse() {
        return speed * mass;
    }
};

class PhysCircle : public PhysShape {
private:
    Vector center;
    float r;

public:
    PhysCircle(){}
    PhysCircle(Vector center, float r, Vector speed, float mass) : PhysShape(mass, speed, ShapeType::Circle), center(center), r(r) {}

    virtual ~PhysCircle() {
        printf("circle destroyed!\n");
    }
    
    virtual void move(float time) {
        center += speed * time;
    }

    virtual float getR() const {
        return r;
    }

    virtual void setR(const float new_r) {
        r = new_r;
    }

    virtual const Vector& getCenter() const {
        return center;
    }

    virtual void setCenter(const Vector& new_center) {
        center = new_center;
    }
};

class PhysRect : public PhysShape {
private:
    Vector position; // left down corner
    Vector size;

public:
    PhysRect(){}
    PhysRect(Vector position, Vector size, Vector speed, float mass) : PhysShape(mass, speed, ShapeType::Rectangle), position(position), size(size) {}

    virtual ~PhysRect() {
        printf("rectangle destroyed!\n");
    }
    
    virtual void move(float time) {
        position += speed * time;
    }

    virtual const Vector& getSize() const {
        return size;
    }

    virtual void setSize(const Vector& new_size){
        size = new_size;
    }

    virtual const Vector& getPos() const {
        return position;
    }

    virtual void setPos(const Vector& new_pos){
        position = new_pos;
    }
};

class PhysWall: public PhysShape {
private:
    Vector pos1;
    Vector pos2;
public:
    PhysWall() {}
    PhysWall(Vector pos1, Vector pos2, Vector speed, float mass) : PhysShape(mass, speed, ShapeType::Wall), pos1(pos1), pos2(pos2) {}
    ~PhysWall() {
        printf("phys wall destroyed!\n");
    }

    virtual void move(float time) {

    }

    virtual const Vector& getFirstPoint() const {
        return pos1;
    }

    virtual const Vector& getSecondPoint() const {
        return pos2;
    }

    virtual void setFirstPoint(const Vector& new_point) {
        pos1 = new_point;
    }

    virtual void setSecondPoint(const Vector& new_point) {
        pos2 = new_point;
    }

};

#endif