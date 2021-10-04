#ifndef CIRCLE_MANAGER_INCLUDE
#define CIRCLE_MANAGER_INCLUDE

#include "Circle.h"
#include "DynamicArray.h"

class CircleManager {
private:
    DynamicArray<Circle> array;
    CoordSystem* coord_system = NULL;

public:
    CircleManager(Rect2f walls, const Rect2f& range_rect, const Rect2f& pixel_rect) {
        coord_system = new CoordSystem(range_rect, pixel_rect);
    }

    ~CircleManager() {
        printf("circle manager destroyed!\n");
    }

    void update(const float time) const {
        for (int i = 0; i < array.length(); ++i) {
            array[i].collide_with_walls(coord_system->getCoordRect(), time);
        }
        for (int i = 0; i < array.length(); ++i) {
            for (int j = i + 1; j < array.length(); ++j) {
                array[i].collide(array[j], time);
            }
        }
        for (int i = 0; i < array.length(); ++i) {
            array[i].move(time);
        }
    }

    void render(Renderer* renderer) const {
        for (int i = 0; i < array.length(); ++i) {
            renderer->draw(array[i], coord_system);
        }
        renderer->render();
    }

    void addCircle(const Circle& circle) {
        array.push_back(circle);
    }
};

#endif