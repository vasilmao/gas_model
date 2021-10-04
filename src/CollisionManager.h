#ifndef INCLUDE_COLLISION_MANAGER
#define INCLUDE_COLLISION_MANAGER

#include "PhysShapes.h"

#define Max(a, b) a > b ? a : b
#define Min(a, b) a < b ? a : b

bool RectsIntersec(const Vector& r1_pos, const Vector& r1_size, const Vector& r2_pos, const Vector& r2_size);

bool CirclesIntersec(const Vector& c1, const float r1, const Vector& c2, const float r2);

void collideCC(PhysCircle* c1, PhysCircle* c2, const float dt);

void collideRR(PhysRect* r1, PhysRect* r2, const float dt);

void collideCR(PhysCircle* c1, PhysCircle* c2, const float dt);

void collideCW(PhysCircle* circle, PhysWall* wall, const float dt);

void collideRW(PhysRect* rectangle, PhysWall* wall, const float dt);

void collideWW(PhysWall* w1, PhysWall* w2, const float dt);

typedef void (*func)(PhysShape*, PhysShape*, const float);

class CollisionManager {
private:
    func virtual_table[3][3] = {
        {reinterpret_cast<func>(collideCC), reinterpret_cast<func>(collideCR), reinterpret_cast<func>(collideCW)},
        {0                                , reinterpret_cast<func>(collideRR), reinterpret_cast<func>(collideRW)},
        {0                                ,                                 0, reinterpret_cast<func>(collideWW)}};
public:    
    void collide(PhysShape* s1, PhysShape* s2, const float dt) {
        if (s1->type_id > s2->type_id) {
            PhysShape* temp = s1;
            s1 = s2;
            s2 = temp;
        }
        // printf("delim %d, %d\n", (int)s1->type_id, (int)s2->type_id);
        (*virtual_table[(int)s1->type_id][(int)s2->type_id])(s1, s2, dt);
    }
};

#endif
