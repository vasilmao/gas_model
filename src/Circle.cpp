#include "Circle.h"

float Circle::getR() const {
    return r;
}

const Vector& Circle::getCenter() const {
    return center;
}

void Circle::collide(Circle& other, float time) {
    Vector new_pos = center + speed * time;
    Vector other_new_pos = other.center + other.speed * time;
    if (dist_quad(new_pos, other_new_pos) <= (r + other.r) * (r + other.r)) {
        Swap(&speed, &(other.speed));
    }
}
void Circle::move(float time) {
    printf("prev coords: %f %f\n", center.getX(), center.getY());
    center += speed * time;
}

void Circle::collide_with_walls(const Rect2f& walls, const float time) {
    Vector new_center = center + speed * time;
    if (walls.x >= new_center.getX() - r || walls.x + walls.width <= new_center.getX() + r) {
        speed.invertX();
    }

    if (walls.y >= new_center.getY() - r || walls.y + walls.height <= new_center.getY() + r) {
        speed.invertY();
    }
}