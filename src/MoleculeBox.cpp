#include "MoleculeBox.h"

struct CollisionInfo {
    List<Shape*>::Iterator it1;
    List<Shape*>::Iterator it2;
    float collision_time;
};

MoleculeBox::MoleculeBox(Vector pos, Vector size) : box{0, 0, size.getX(), size.getY()}, screen_place{pos.getX(), pos.getY(), size.getX(), size.getY()} {
    manager = new CollisionManager();
    chem_manager = new ReactionManager();
    printf("adding wall\n");
    // objects.add(new Wall({5, 5}, {395, 5}));
    // printf("adding wall\n");
    // objects.add(new Wall({5, 5}, {5, 295}));
    // printf("adding wall\n");
    // objects.add(new Wall({395, 5}, {395, 295}));
    // printf("adding wall\n");
    // objects.add(new Wall({5, 295}, {395, 295}));
    float pos1_x = screen_place.x;
    float pos1_y = screen_place.y;
    float pos2_x = pos1_x + screen_place.width;
    float pos2_y = pos1_y + screen_place.height;
    objects.add(new Wall({pos1_x, pos1_y}, {pos2_x, pos1_y}));
    objects.add(new Wall({pos1_x, pos1_x}, {pos1_x, pos2_y}));
    objects.add(new Wall({pos2_x, pos1_x}, {pos2_x, pos2_y}));
    objects.add(new Wall({pos1_x, pos2_y}, {pos2_x, pos2_y}));
    printf("ye\n");
    objects.add(new Wall({pos1_x, pos1_y + 50}, {pos1_x + 50, pos1_y + 0}));
    objects.add(new Rect({pos1_x + 100, pos1_y + 100}, {30, 30}, {40, 0}, 20));
    objects.add(new Rect({pos1_x + 150, pos1_y + 100}, {30, 30}, {-30, 0}, 20));
    objects.add(new Rect({pos1_x + 200, pos1_y + 200}, {30, 30}, {40, 0}, 20));
    objects.add(new Rect({pos1_x + 220, pos1_y + 200}, {30, 30}, {-30, 0}, 20));
}

void MoleculeBox::update(float dt) {
    DynamicArray<CollisionInfo> collisions;
    for (List<Shape*>::Iterator i = objects.begin(); i.isValid(); ++i) {
        for (List<Shape*>::Iterator j = i + 1; j.isValid(); ++j) {
            float collision_time = manager->detectCollision(i.getNode()->data->getPhysObject(), j.getNode()->data->getPhysObject(), dt);
            if (collision_time > -(1e-6)) {
                CollisionInfo info = {i, j, collision_time};
                collisions.push_back(info);
            }
        }
    }
    qsort(collisions.begin(), collisions.length(), sizeof(CollisionInfo), [](const void* el1, const void* el2){
        CollisionInfo* col1 = (CollisionInfo*) el1;
        CollisionInfo* col2 = (CollisionInfo*) el2;
        if (fabs(col1->collision_time - col2->collision_time) < 1e-6) {
            return 0;
        }
        if (col1->collision_time < col2->collision_time) {
            return -1;
        }
        return 1;
    });
    for (int i = 0; i < collisions.length(); ++i) {
        List<Shape*>::Iterator it1 = collisions[i].it1;
        List<Shape*>::Iterator it2 = collisions[i].it2;
        if (it1.isValid() && it2.isValid()) {
            chem_manager->chemReaction(it1, it2, &objects);
        }
    }
    objects.clear();
    
    float all_mass = 0;
    float nrj = 0;
    int obj_cnt = 0;

    for (List<Shape*>::Iterator i = objects.begin(); i.isValid(); ++i) {
        i.getNode()->data->getPhysObject()->move(dt);
        i.getNode()->data->translateCoords();
        all_mass += i.getNode()->data->getPhysObject()->getMass();
        nrj += i.getNode()->data->getPhysObject()->getMass() * i.getNode()->data->getPhysObject()->getSpeed().getLength() * i.getNode()->data->getPhysObject()->getSpeed().getLength() / 2;
        if (i.getNode()->data->getType() == 1) {
            nrj += ((Rect*)(i.getNode()->data))->potential_energy;
        }
        ++obj_cnt;
    }
    // for (List<Shape*>::Iterator i = objects.begin(); i.isValid(); ++i) {
    //     i.getNode()->data->getRenderObject()->render(renderer);
    // }
}

void MoleculeBox::render(float dt, Renderer* renderer) {
    for (List<Shape*>::Iterator i = objects.begin(); i.isValid(); ++i) {
        i.getNode()->data->getRenderObject()->render(renderer);
    }
}