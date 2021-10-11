#ifndef INCLUDE_COLLISION_MANAGER
#define INCLUDE_COLLISION_MANAGER

#include "PhysShapes.h"

#define Max(a, b) a > b ? a : b
#define Min(a, b) a < b ? a : b

bool RectsIntersec(const Vector& r1_pos, const Vector& r1_size, const Vector& r2_pos, const Vector& r2_size);

bool CirclesIntersec(const Vector& c1, const float r1, const Vector& c2, const float r2);

bool detectCollisionCC(PhysCircle* c1, PhysCircle* c2, const float dt);

bool detectCollisionRR(PhysRect* r1, PhysRect* r2, const float dt);

bool detectCollisionCR(PhysCircle* c1, PhysCircle* c2, const float dt);

bool detectCollisionCW(PhysCircle* circle, PhysWall* wall, const float dt);

bool detectCollisionRW(PhysRect* rectangle, PhysWall* wall, const float dt);

bool detectCollisionWW(PhysWall* w1, PhysWall* w2, const float dt);

typedef bool (*func)(PhysShape*, PhysShape*, const float);

class CollisionManager {
private:
    func detect_collision_virtual_table[3][3] = {
        {reinterpret_cast<func>(detectCollisionCC), reinterpret_cast<func>(detectCollisionCR), reinterpret_cast<func>(detectCollisionCW)},
        {                                        0, reinterpret_cast<func>(detectCollisionRR), reinterpret_cast<func>(detectCollisionRW)},
        {                                        0,                                         0, reinterpret_cast<func>(detectCollisionWW)}
    };

public:    
    float detectCollision(PhysShape* s1, PhysShape* s2, const float dt) {
        if (s1->getType() > s2->getType()) {
            PhysShape* temp = s1;
            s1 = s2;
            s2 = temp;
        }
        // printf("delim %d, %d\n", s1->getType(), s2->getType());
        
        bool dt_collison = (*detect_collision_virtual_table[(int)s1->getType()][(int)s2->getType()])(s1, s2, dt);
        // printf("collision in dt? %d\n", (int)dt_collison);
        const float EPS = 1e-6;
        if (dt_collison) {
            float left = 0;
            float right = dt;
            while (right - left > EPS) {
                float middle = (right + left) / 2;
                if ((*detect_collision_virtual_table[(int)s1->getType()][(int)s2->getType()])(s1, s2, middle)) {
                    right = middle;
                } else {
                    left = middle;
                }
            }
            return left;
        }
        return -1;
    }
};

#endif
