#include "CollisionManager.h"

bool RectsIntersec(const Vector& r1_pos, const Vector& r1_size, const Vector& r2_pos, const Vector& r2_size) {
    Vector r1_p2 = r1_pos + r1_size;
    Vector r2_p2 = r2_pos + r2_size;
    float p1_x = Max(r1_pos.getX(), r2_pos.getX());
    float p2_x = Min(r1_p2.getX(), r2_p2.getX());
    float width = p2_x - p1_x;
    if (width > 0) {
        return true;
    }
    float p1_y = Max(r1_pos.getY(), r2_pos.getY());
    float p2_y = Min(r1_p2.getY(), r2_p2.getY());
    float height = p2_y - p1_y;
    if (height > 0) {
        return true;
    }
    return false;
}

bool CirclesIntersec(const Vector& c1, const float r1, const Vector& c2, const float r2) {
    float rsum_2 = (r1 + r2) * (r1 + r2);
    return dist_quad(c1, c2) <= rsum_2;
}

bool detectCollisionCC(PhysCircle* c1, PhysCircle* c2, const float dt) {
    Vector new_c1 = c1->getCenter() + dt * c1->getSpeed();
    Vector new_c2 = c2->getCenter() + dt * c2->getSpeed();
    if (dist_quad(new_c1, new_c2) <= (c1->getR() + c2->getR()) * (c1->getR() + c2->getR())) {
        // they do collide in dt
        return true;
        // Vector imp1 = c1->getImpulse();
        // Vector imp2 = c2->getImpulse();
        // Vector center_line = new_c2 - new_c1;
        // Vector imp1_pr = imp1.getProjection(center_line);
        // Vector imp2_pr = imp2.getProjection(center_line);
        // imp1 -= imp1_pr;
        // imp1 += imp2_pr;
        // imp2 -= imp2_pr;
        // imp2 += imp1_pr;
        // c1->setSpeed(imp1 / c1->getMass());
        // c2->setSpeed(imp2 / c2->getMass());


        // const Vector old_c1_speed = c1->getSpeed();
        // c1->setSpeed(c2->getSpeed());
        // c2->setSpeed(old_c1_speed);
    }
    return false;
}

bool detectCollisionRR(PhysRect* r1, PhysRect* r2, const float dt) {
    Vector new_r1 = r1->getPos() + dt * r1->getSpeed();
    Vector new_r2 = r2->getPos() + dt * r2->getSpeed();
    if (RectsIntersec(new_r1, r1->getSize(), new_r2, r2->getSize())) {
        return true;
        // const Vector old_r1_speed = r1->getSpeed();
        // r1->setSpeed(r2->getSpeed());
        // r2->setSpeed(old_r1_speed);
    }
    return false;
}

bool detectCollisionCR(PhysCircle* c1, PhysCircle* c2, const float dt) {
    Vector new_c1 = c1->getCenter() + dt * c1->getSpeed();
    Vector new_c2 = c2->getCenter() + dt * c2->getSpeed();
    if (dist_quad(new_c1, new_c2) <= (c1->getR() + c2->getR()) * (c1->getR() + c2->getR())) {
        return true;;
        // const Vector old_c1_speed = c1->getSpeed();
        // c1->setSpeed(c2->getSpeed());
        // c2->setSpeed(old_c1_speed);
    }
    return false;
}

bool detectCollisionCW(PhysCircle* circle, PhysWall* wall, const float dt) {
    Vector new_c = circle->getCenter() + dt * circle->getSpeed();
    Vector v1 = wall->getFirstPoint();
    Vector v2 = wall->getSecondPoint();
    Vector v3 = v2 - v1;
    Vector a = new_c - v1;
    Vector b = a.getProjection(v3);
    // Vector b = ((a * v3) / v3.getLength()) * v3 * (1 / v3.getLength()); // is projection on segment
    if (crossProduct(b, v3) >= 0 && b.getLength() <= v3.getLength()) {
        // projection is inside the segment
        float dist = (a - b).getLength();
        if (dist <= circle->getR()) {
            // they collide
            // need to do speed vector reflection
            // cos = (speed * v3) / |v3| / |speed|
            // projection = |speed| * cos * v3 / |v3| = |speed| * (speed * v3) / |v3| / |speed| / |v3| * v3
            // projection = v3 * ((speed * v3) / |v3|^2)
            // Vector projection = ((circle->getSpeed() * v3) / (v3.getLength() * v3.getLength())) * v3;
            return true;
            // Vector projection = (circle->getSpeed()).getProjection(v3);
            // Vector speed_reflection = (projection - circle->getSpeed()) + projection;
            // circle->setSpeed(speed_reflection);
        }
    }
    return false;
}

bool detectCollisionRW(PhysRect* rectangle, PhysWall* wall, const float dt) {
    // bruh
    return false;
}

bool detectCollisionWW(PhysWall* w1, PhysWall* w2, const float dt) {
    // no
    return false;
}