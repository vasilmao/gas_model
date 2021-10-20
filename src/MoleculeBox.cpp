#include "MoleculeBox.h"

const float heat_wall_energy_addition = 1000;

const float EPS = 1e-6;

struct CollisionInfo {
    List<Shape*>::Iterator it1;
    List<Shape*>::Iterator it2;
    float collision_time;
};

MoleculeBox::MoleculeBox(Vector pos, Vector size) : screen_place{pos.getX(), pos.getY(), size.getX(), size.getY()} {
    manager = new CollisionManager();
    chem_manager = new ReactionManager();
    objects = new List<Shape*>;
    Rect2f range_rect = {0, 0, size.getX(), size.getY()};
    coord_system = new CoordSystem(range_rect, screen_place);

    objects->add(new Wall({0, 0}, {range_rect.width + range_rect.x, 0}));
    objects->add(new Wall({0, 0}, {0, range_rect.height + range_rect.y}));
    objects->add(new Wall({range_rect.width + range_rect.x, 0}, {range_rect.width + range_rect.x, range_rect.height + range_rect.y}));
    objects->add(new Wall({0, range_rect.height + range_rect.y}, {range_rect.width + range_rect.x, range_rect.height + range_rect.y}));

    objects->add(new Wall({0, 0 + 50}, {0 + 50, 0 + 0}));

    objects->add(new Rect({0 + 100, 0 + 100}, {30, 30}, {40, 0}, 20));
    objects->add(new Rect({0 + 150, 0 + 100}, {30, 30}, {-30, 0}, 20));
    objects->add(new Rect({0 + 200, 0 + 200}, {30, 30}, {40, 0}, 20));
    objects->add(new Rect({0 + 220, 0 + 200}, {30, 30}, {-30, 0}, 20));
}

void MoleculeBox::update(float dt) {
    DynamicArray<CollisionInfo> collisions;
    for (List<Shape*>::Iterator i = objects->begin(); i.isValid(); ++i) {
        for (List<Shape*>::Iterator j = i + 1; j.isValid(); ++j) {
            float collision_time = manager->detectCollision(i.getNode()->data->getPhysObject(), j.getNode()->data->getPhysObject(), dt);
            if (collision_time > -EPS) {
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
            chem_manager->chemReaction(it1, it2, objects);
        }
    }
    objects->clear();
    
    float all_mass = 0;
    float nrj = 0;
    int obj_cnt = 0;

    for (List<Shape*>::Iterator i = objects->begin(); i.isValid(); ++i) {
        i.getNode()->data->getPhysObject()->move(dt);
        i.getNode()->data->translateCoords(coord_system);
        all_mass += i.getNode()->data->getPhysObject()->getMass();
        nrj += i.getNode()->data->getPhysObject()->getMass() * i.getNode()->data->getPhysObject()->getSpeed().getLength() * i.getNode()->data->getPhysObject()->getSpeed().getLength() / 2;
        if (i.getNode()->data->getType() == (int)Shape::MoleculeType::RectangleMolecule) {
            nrj += ((Rect*)(i.getNode()->data))->getPotentialEnergy();
        }
        if (i.getNode()->data->getType() == (int)Shape::MoleculeType::Wall) {
            nrj += ((Wall*)(i.getNode()->data))->getPotentialEnergy();
        }
        ++obj_cnt;
    }
}

void MoleculeBox::render(float dt, Renderer* renderer) {
    // renderer->drawFilledRect({screen_place.x, screen_place.y}, {screen_place.x + screen_place.width, screen_place.y + screen_place.height}, {0, 0, 0, 255});
    for (List<Shape*>::Iterator i = objects->begin(); i.isValid(); ++i) {
        i.getNode()->data->getRenderObject()->render(renderer);
    }
}

void MoleculeBox::heatWalls() {
    for (List<Shape*>::Iterator i = objects->begin(); i.isValid(); ++i) {
        Shape* shape = i.getNode()->data;
        if (shape->getType() == (int)Shape::MoleculeType::Wall) {
            reinterpret_cast<Wall*>(shape)->addPotentialEnergy(heat_wall_energy_addition);
        }
    }
}