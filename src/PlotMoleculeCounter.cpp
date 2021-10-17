#include "PlotMoleculeCounter.h"

const float plot_step = 3;

PlotMoleculeCounter::PlotMoleculeCounter(Vector pos, Vector size) : screen_place{pos.getX(), pos.getY(), size.getX(), size.getY()} {
    data = new List<PlotInfo>();
    coord_system = new CoordSystem({0, 0, screen_place.width, screen_place.height}, screen_place);
}

void PlotMoleculeCounter::computeNewPoint(MoleculeBox* box) {
    if ((data->getSize() + 1) * plot_step >= screen_place.width) {
        data->eraseHead();
        data->clear();
    }
    int circles_cnt = 0;
    int rects_cnt = 0;
    List<Shape*>* objects = box->getObjects();
    for (List<Shape*>::Iterator it = objects->begin(); it.isValid(); ++it) {
        int type = it.getNode()->data->getType();
        switch (type)
        {
        case static_cast<int>(Shape::MoleculeType::CircleMolecule):
            ++circles_cnt;
            break;
        case static_cast<int>(Shape::MoleculeType::RectangleMolecule):
            ++rects_cnt;
            break;

        default:
            break;
        }
    }
    data->add({circles_cnt, rects_cnt});
    printf("%d, %d\n", circles_cnt, rects_cnt);
    // printf("%d\n", (int)(data->getSize()));
}

void PlotMoleculeCounter::render(float dt, Renderer* renderer) {
    renderer->drawRect({screen_place.x, screen_place.y}, {screen_place.x + screen_place.width, screen_place.y +  screen_place.height}, {0, 0, 0, 255});
    List<PlotInfo>::Iterator it = data->begin();
    if (!it.isValid()) {
        return;
    }
    PlotInfo last_values = it.getNode()->data;
    float x_coord = screen_place.x;
    ++it;
    // printf("keks!\n");
    for (; it.isValid(); ++it) {
        PlotInfo current_values = it.getNode()->data;
        Vector circle_point1(x_coord, last_values.circles_cnt);
        Vector circle_point2(x_coord + plot_step, current_values.circles_cnt);
        // Vector new_circle_point1 = coord_system->translatePoint(circle_point1);
        circle_point1 += Vector(screen_place.x, screen_place.y);
        // Vector new_circle_point2 = coord_system->translatePoint(circle_point2);
        circle_point2 += Vector(screen_place.x, screen_place.y);
        // printf("(%f, %f) and (%f, %f), green\n", circle_point1.getX(), circle_point1.getY(), circle_point2.getX(), circle_point2.getY());
        renderer->drawLine(circle_point1, circle_point2, {0, 255, 0, 255});

        Vector rect_point1(x_coord, last_values.rects_cnt);
        Vector rect_point2(x_coord + plot_step, current_values.rects_cnt);
        // Vector rect_point1 = coord_system->translatePoint(rect_point1);
        // Vector rect_point2 = coord_system->translatePoint(rect_point2);
        rect_point1 += Vector(screen_place.x, screen_place.y);
        rect_point2 += Vector(screen_place.x, screen_place.y);
        // printf("(%f, %f) and (%f, %f), red\n", rect_point1.getX(), rect_point1.getY(), rect_point2.getX(), rect_point2.getY());
        renderer->drawLine(rect_point1, rect_point2, {255, 0, 0, 255});

        x_coord += plot_step;
        last_values = current_values;
    }
}

PlotMoleculeCounter::~PlotMoleculeCounter() {
    delete coord_system;
    delete data;
}