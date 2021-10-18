#include "CollisionReaction.h"

const float new_ball_r = 2.0;

void ReactCC(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects) {
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

    Vector center1 = pc1->getCenter();
    Vector center2 = pc2->getCenter();
    Vector new_size(new_ball_r * 2, new_ball_r * 2) ;
    objects->add(new Rect((center1 + center2) / 2 - (new_size / 2), new_size, final_speed, m1 + m2, potential_energy_loss));
    objects->erase(it1);
    objects->erase(it2);
}

void ReactRR(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects) {
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
    float kinetic_energy = m1 * speed1 * speed1 + m2 * speed2 * speed2 + 2 * rect1->potential_energy + 2 * rect2->potential_energy;
    float ball_speed_value = sqrt(kinetic_energy / balls_cnt);

    // printf("r + r\n");
    // printf("explosion center: %f %f\n", explosion_center.getX(), explosion_center.getY());
    // printf("ball cnt: %d\n", balls_cnt);
    // printf("energy: %f %f\n", rect1->potential_energy, rect2->potential_energy);

    for (int i = 0; i < balls_cnt; ++i) {
        Vector ball_pos = explosion_center + Vector(dist * cos(current_angle), dist * sin(current_angle));
        Vector ball_speed(ball_speed_value * cos(current_angle), ball_speed_value * sin(current_angle));
        objects->add(new Circle(ball_pos, new_ball_r, ball_speed, 1));
        current_angle += angle;
    }
    objects->erase(it1);
    objects->erase(it2);
}

void ReactCR(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects) {
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
    // Vector imp1_pr = speed1.getProjection(center_line) * m1;
    // Vector imp2_pr = speed2.getProjection(center_line) * m2;
    // Vector final_imp_pr = imp2_pr + imp1_pr;


    // Vector imp1_perp = speed1 * m1 - imp1_pr;
    // Vector imp2_perp = speed2 * m2 - imp2_pr;

    // Vector final_imp_perp = imp1_perp + imp2_perp;

    // Vector final_imp = final_imp_perp + final_imp_pr;

    float potential_energy_loss = (speed1.getLength() * speed1.getLength() * m1 + speed2.getLength() * speed2.getLength() * m2) / 2 - (final_speed.getLength() * final_speed.getLength() * (m1 + m2)) / 2;

    // printf("c + r\n");
    // Vector center1 = (reinterpret_cast<PhysCircle*>(circle->getPhysObject()))->getCenter();
    // Vector pos2 = (reinterpret_cast<PhysCircle*>(rect->getPhysObject()))->getCenter();
    rect->potential_energy += potential_energy_loss;
    float area = phys_circle2->getR() * phys_circle2->getR() * 4;
    area += phys_circle1->getR() * phys_circle1->getR() * 4;
    float side_len = sqrt(area);
    phys_circle2->setR(side_len / 2);
    phys_circle2->setMass(m1 + m2);
    phys_circle2->setSpeed(final_speed);
    // (reinterpret_cast<PhysCircle*>(rect->getPhysObject()))->setR((m1 + m2) * new_ball_r * 2);
    // (reinterpret_cast<PhysCircle*>(rect->getPhysObject()))->setMass(m1 + m2);
    // (reinterpret_cast<PhysCircle*>(rect->getPhysObject()))->setCenter(pos2);
    objects->erase(it1);
}

void ReactCW(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects) {
    // need to do speed vector reflection
    // cos = (speed * v3) / |v3| / |speed|
    // projection = |speed| * cos * v3 / |v3| = |speed| * (speed * v3) / |v3| / |speed| / |v3| * v3
    // projection = v3 * ((speed * v3) / |v3|^2)

    PhysCircle* circle = reinterpret_cast<PhysCircle*> (it1.getNode()->data->getPhysObject());
    PhysWall*   wall   = reinterpret_cast<PhysWall*>   (it2.getNode()->data->getPhysObject());

    Vector circle_c = circle->getCenter();
    Vector v1 = wall->getFirstPoint();
    Vector v2 = wall->getSecondPoint();
    Vector v3 = v2 - v1;
    Vector a = circle_c - v1;
    Vector b = a.getProjection(v3);

    // Vector projection = ((circle->getSpeed() * v3) / (v3.getLength() * v3.getLength())) * v3;
    Vector projection = (circle->getSpeed()).getProjection(v3);
    Vector speed_reflection = (projection - circle->getSpeed()) + projection;
    circle->setSpeed(speed_reflection);
}

void ReactRW(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects) {
    ReactCW(it1, it2, objects);
}

void ReactWW(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects) {
    assert(0);
}