#include "CollisionReaction.h"

const float new_ball_r = 2.0;

const float energy_coef = 0.1;

const float min_energy_transition = 10;

void ReactCC(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects, Rect2f borders) {
    // printf("c + c\n");
    Circle* circle1 = reinterpret_cast<Circle*> (it1.getNode()->data);
    Circle* circle2 = reinterpret_cast<Circle*> (it2.getNode()->data);

    PhysCircle* pc1 = reinterpret_cast<PhysCircle*>(circle1->getPhysObject());
    PhysCircle* pc2 = reinterpret_cast<PhysCircle*>(circle2->getPhysObject());

    Vector center_line = pc2->getCenter()- pc1->getCenter();
    Vector speed1 = pc1->getSpeed();
    Vector speed2 = pc2->getSpeed();
    float m1 = pc1->getMass();
    float m2 = pc2->getMass();

    Vector final_imp = speed1 * m1 + speed2 * m2;
    Vector final_speed = final_imp / (m1 + m2);

    float potential_energy_loss = (speed1.getLength() * speed1.getLength() * m1 + speed2.getLength() * speed2.getLength() * m2) / 2 - (final_imp.getLength() * final_speed.getLength()) / 2;
    potential_energy_loss += circle1->getPotentialEnergy() + circle2->getPotentialEnergy();

    Vector center1 = pc1->getCenter();
    Vector center2 = pc2->getCenter();
    Vector new_size(new_ball_r * 2, new_ball_r * 2) ;
    objects->add(new Rect((center1 + center2) / 2 - (new_size / 2), new_size, final_speed, m1 + m2, potential_energy_loss));
    objects->erase(it1);
    objects->erase(it2);
}

void ReactRR(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects, Rect2f borders) {
    Rect* rect1 = reinterpret_cast<Rect*> (it1.getNode()->data);
    Rect* rect2 = reinterpret_cast<Rect*> (it2.getNode()->data);
    const PhysCircle* pr1 = (reinterpret_cast<PhysCircle*>(rect1->getPhysObject()));
    const PhysCircle* pr2 = (reinterpret_cast<PhysCircle*>(rect2->getPhysObject()));
    Vector pos1 = pr1->getCenter();
    Vector pos2 = pr2->getCenter();

    Vector explosion_center = (pos1 + pos2) / 2;
    float m1 = pr1->getMass();
    float m2 = pr2->getMass();
    int balls_cnt = (int)(m1 + m2);

    float angle = 2 * M_PI / balls_cnt;
    float dist = new_ball_r / sin(angle / 2);

    float current_angle = 0;
    float speed1 = pr1->getSpeed().getLength();
    float speed2 = pr2->getSpeed().getLength();
    float kinetic_energy = m1 * speed1 * speed1 + m2 * speed2 * speed2 + 2 * rect1->getPotentialEnergy() + 2 * rect2->getPotentialEnergy();
    float ball_speed_value = sqrt(kinetic_energy / balls_cnt);

    // printf("r + r\n");
    for (int i = 0; i < balls_cnt; ++i) {
        Vector ball_pos = explosion_center + Vector(dist * cos(current_angle), dist * sin(current_angle));
        if (ball_pos.getX() <= borders.x) {
            ball_pos.setX(ball_pos.getX() + borders.width - new_ball_r);
        }
        if (ball_pos.getY() <= borders.y) {
            ball_pos.setY(ball_pos.getY() + borders.height - new_ball_r);
        }
        if (ball_pos.getX() >= borders.x + borders.width) {
            ball_pos.setX(ball_pos.getX() - borders.width + new_ball_r);
        }
        if (ball_pos.getY() >= borders.y + borders.height) {
            ball_pos.setY(ball_pos.getY() - borders.height + new_ball_r);
        }
        Vector ball_speed(ball_speed_value * cos(current_angle), ball_speed_value * sin(current_angle));
        objects->add(new Circle(ball_pos, new_ball_r, ball_speed, 1));
        current_angle += angle;
    }
    objects->erase(it1);
    objects->erase(it2);
}

void ReactCR(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects, Rect2f borders) {
    Circle* circle = reinterpret_cast<Circle*> (it1.getNode()->data);
    Rect* rect = reinterpret_cast<Rect*> (it2.getNode()->data);
    PhysCircle* phys_circle1 = reinterpret_cast<PhysCircle*> (circle->getPhysObject());
    PhysCircle* phys_circle2 = reinterpret_cast<PhysCircle*> (rect->getPhysObject());
    Vector speed1 = phys_circle1->getSpeed();
    Vector speed2 = phys_circle2->getSpeed();

    float m1 = phys_circle1->getMass();
    float m2 = phys_circle2->getMass();
    
    // todo: fix energy loss

    Vector center_line = phys_circle2->getCenter() - phys_circle1->getCenter();
    Vector final_imp = speed1 * m1 + speed2 * m2;
    Vector final_speed = final_imp / (m1 + m2);

    float potential_energy_loss = (speed1.getLength() * speed1.getLength() * m1 + speed2.getLength() * speed2.getLength() * m2) / 2 - (final_speed.getLength() * final_speed.getLength() * (m1 + m2)) / 2;
    potential_energy_loss += circle->getPotentialEnergy();

    // printf("c + r\n");
    rect->addPotentialEnergy(potential_energy_loss);
    float area = phys_circle2->getR() * phys_circle2->getR() * 4;
    area += phys_circle1->getR() * phys_circle1->getR() * 4;
    float side_len = sqrt(area);
    phys_circle2->setR(side_len / 2);
    phys_circle2->setMass(m1 + m2);
    phys_circle2->setSpeed(final_speed);
    objects->erase(it1);
}

void ReactCW(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects, Rect2f borders) {
    // need to do speed vector reflection
    Wall* wall = reinterpret_cast<Wall*> (it2.getNode()->data);
    Shape* rect = it1.getNode()->data;
    if (wall->getPotentialEnergy() > min_energy_transition) {
        float energy_delta = wall->getPotentialEnergy() - rect->getPotentialEnergy();
        energy_delta *= energy_coef;
        if (energy_delta < -FLT_EPSILON) {
            energy_delta = -energy_delta;
        }
        rect->addPotentialEnergy(energy_delta);
        wall->addPotentialEnergy(-energy_delta);
    }

    PhysCircle* phys_circle = reinterpret_cast<PhysCircle*> (it1.getNode()->data->getPhysObject());
    PhysWall*   phys_wall   = reinterpret_cast<PhysWall*>   (it2.getNode()->data->getPhysObject());

    Vector phys_circle_c = phys_circle->getCenter();
    Vector v1 = phys_wall->getFirstPoint();
    Vector v2 = phys_wall->getSecondPoint();
    Vector v3 = v2 - v1;
    Vector a = phys_circle_c - v1;
    Vector b = a.getProjection(v3);

    Vector projection = (phys_circle->getSpeed()).getProjection(v3);
    Vector speed_reflection = (projection - phys_circle->getSpeed()) + projection;
    phys_circle->setSpeed(speed_reflection);
}

void ReactRW(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects, Rect2f borders) {
    ReactCW(it1, it2, objects, borders);
    
}

void ReactWW(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects, Rect2f borders) {
    assert(0);
}