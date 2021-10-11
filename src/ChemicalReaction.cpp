#include "ChemicalReaction.h"

const float new_ball_r = 3.0;

void ReactCC(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects) {
    printf("c + c\n");
    Circle* circle1 = reinterpret_cast<Circle*> (it1.getNode()->data);
    Circle* circle2 = reinterpret_cast<Circle*> (it2.getNode()->data);

    PhysCircle* pc1 = reinterpret_cast<PhysCircle*>(circle1->getPhysObject());
    PhysCircle* pc2 = reinterpret_cast<PhysCircle*>(circle2->getPhysObject());

    Vector center_line = pc2->getCenter()- pc1->getCenter();
    Vector speed1 = pc1->getSpeed();
    Vector speed2 = pc2->getSpeed();
    float m1 = pc1->getMass();
    float m2 = pc2->getMass();

    Vector imp1_pr = speed1.getProjection(center_line) * m1;
    Vector imp2_pr = speed1.getProjection(center_line) * m2;
    Vector final_imp_pr = imp1_pr - imp2_pr;
    Vector final_speed_pr = final_imp_pr / (m1 + m2);

    float potential_energy_loss = (imp1_pr.getLength() * speed1.getLength() + imp2_pr.getLength() * speed2.getLength()) - (final_imp_pr.getLength() * final_speed_pr.getLength());

    Vector imp1_perp = speed1 * m1 - imp1_pr;
    Vector imp2_perp = speed2 * m2 - imp2_pr;

    Vector final_imp_perp = imp1_perp + imp2_pr;

    Vector center1 = circle1->getReactionCenter();
    Vector center2 = circle2->getReactionCenter();
    float d = sqrt(new_ball_r * m1 * new_ball_r * m2 * 4);
    Vector new_size(d, d);
    objects->add(new Rect((center1 + center2) / 2 - (new_size / 2), new_size, (final_imp_pr + final_imp_perp) / (m1 + m2), m1 + m2, potential_energy_loss));
    objects->erase(it1);
    objects->erase(it2);
}

void ReactRR(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects) {
    Rect* rect1 = reinterpret_cast<Rect*> (it1.getNode()->data);
    Rect* rect2 = reinterpret_cast<Rect*> (it2.getNode()->data);
    // const PhysRect* pr1 = (reinterpret_cast<PhysRect*>(rect1->getPhysObject()));
    // const PhysRect* pr2 = (reinterpret_cast<PhysRect*>(rect2->getPhysObject()));
    Vector pos1 = rect1->getReactionPos();
    Vector pos2 = rect2->getReactionPos();

    Vector explosion_center = (pos1 + pos2) / 2;
    float m1 = rect1->getPhysObject()->getMass();
    float m2 = rect2->getPhysObject()->getMass();
    int balls_cnt = (int)(m1 + m2);

    float angle = 2 * M_PI / balls_cnt;
    float dist = new_ball_r / sin(angle / 2) * 1.1;

    float current_angle = 0;
    float speed1 = rect1->getPhysObject()->getSpeed().getLength();
    float speed2 = rect2->getPhysObject()->getSpeed().getLength();
    float kinetic_energy = m1 * speed1 * speed1 + m2 * speed2 * speed2 + rect1->potential_energy + rect2->potential_energy;
    float ball_speed_value = sqrt(kinetic_energy / balls_cnt);

    printf("r + r\n");
    // printf("explosion center: %f %f\n", explosion_center.getX(), explosion_center.getY());
    // printf("ball cnt: %d\n", balls_cnt);
    // printf("energy: %f %f\n", rect1->potential_energy, rect2->potential_energy);

    for (int i = 0; i < balls_cnt; ++i) {
        Vector ball_pos = explosion_center + Vector(dist * cos(current_angle), dist * sin(current_angle));
        // printf("ball center: %f %f\n", ball_pos.getX(), ball_pos.getY());
        Vector ball_speed(ball_speed_value * cos(current_angle), ball_speed_value * sin(current_angle));
        objects->add(new Circle(ball_pos, new_ball_r, ball_speed, 1));
        current_angle += angle;
    }
    // assert(0);
    objects->erase(it1);
    objects->erase(it2);
}

void ReactCR(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects) {
    Circle* circle = reinterpret_cast<Circle*> (it1.getNode()->data);
    Rect* rect = reinterpret_cast<Rect*> (it2.getNode()->data);
    Vector speed1 = circle->getPhysObject()->getSpeed();
    Vector speed2 = rect->getPhysObject()->getSpeed();
    float m1 = circle->getPhysObject()->getMass();
    float m2 = rect->getPhysObject()->getMass();
    

    Vector center_line = rect->g - circle->getReactionCenter();

    Vector imp1_pr = speed1.getProjection(center_line) * m1;
    Vector imp2_pr = speed1.getProjection(center_line) * m2;
    Vector final_imp_pr = imp1_pr - imp2_pr;
    Vector final_speed_pr = final_imp_pr / (m1 + m2);

    float potential_energy_loss = (imp1_pr.getLength() * speed1.getLength() + imp2_pr.getLength() * speed2.getLength()) - (final_imp_pr.getLength() * final_speed_pr.getLength());

    Vector imp1_perp = speed1 * m1 - imp1_pr;
    Vector imp2_perp = speed2 * m2 - imp2_pr;

    Vector final_imp_perp = imp1_perp + imp2_pr;


    printf("c + r\n");

    Vector new_rect_speed = (speed1 * m1 + speed2 * m2) / (m1 + m2);
    // Vector center1 = (reinterpret_cast<PhysCircle*>(circle->getPhysObject()))->getCenter();
    // Vector pos2 = (reinterpret_cast<PhysCircle*>(rect->getPhysObject()))->getCenter();
    (reinterpret_cast<PhysCircle*>(rect->getPhysObject()))->setR((m1 + m2) * new_ball_r * 2);
    (reinterpret_cast<PhysCircle*>(rect->getPhysObject()))->setMass(m1 + m2);
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