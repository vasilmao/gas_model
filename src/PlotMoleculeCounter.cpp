#include "PlotMoleculeCounter.h"

const float plot_step = 1;
const float axis_caption_size = 25;
const float axis_caption_step = 5;

PlotMoleculeCounter::PlotMoleculeCounter(Vector pos, Vector size) : screen_place{pos.getX(), pos.getY(), size.getX(), size.getY()} {
    data = new List<PlotInfo>(1000);
    coord_system = new CoordSystem({0, 0, screen_place.width, screen_place.height}, screen_place);
}

void PlotMoleculeCounter::computeNewPoint(MoleculeBox* box) {
    if ((data->getSize() + 5 + axis_caption_size) * plot_step >= screen_place.width) {
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
    // printf("%d, %d\n", circles_cnt, rects_cnt);
    // printf("%d\n", (int)(data->getSize()));
}

void PlotMoleculeCounter::drawAxis(Renderer* renderer) {
    Vector point1 = coord_system->translateToAbsolute({0, axis_caption_step});
    Vector point2 = coord_system->translateToAbsolute({0, 0});
    float axis_step_abs = Min((point1 - point2).getY(), (axis_caption_size) / 3);
    float relative_height = coord_system->getRelativeSize().getY();
    for (float i = axis_caption_step; i < relative_height; i += axis_caption_step) {
        Vector point = coord_system->translateToAbsolute({0, i});
        renderer->drawLine(point, point + Vector(3, 0), {0, 0, 0, 255});
        Vector point_lower = coord_system->translateToAbsolute({0, i - axis_caption_step});
        Vector text_rect_point = point_lower + Vector(3, (point1 - point2).getY() - axis_step_abs);

        char text[5] = {0};
        snprintf(text, 5, "%d", (int)i);
        int text_length = strlen(text);

        Vector text_rect_size = {(axis_step_abs) * (text_length), axis_step_abs};
        renderer->drawText(text_rect_point, text_rect_size, text, {0, 0, 0, 255});
    }
}

void PlotMoleculeCounter::render(float dt, Renderer* renderer) {
    renderer->drawFilledRect({screen_place.x, screen_place.y}, {screen_place.x + screen_place.width, screen_place.y +  screen_place.height}, {255, 255, 255, 255});
    renderer->drawRect({screen_place.x, screen_place.y}, {screen_place.x + screen_place.width, screen_place.y +  screen_place.height}, {0, 0, 0, 255});

    List<PlotInfo>::Iterator it = data->begin();
    if (!it.isValid()) {
        return;
    }
    PlotInfo last_values = it.getNode()->data;
    float x_coord = screen_place.x + axis_caption_size;
    ++it;
    max_value = last_values;
    for (; it.isValid(); ++it) {
        PlotInfo current_values = it.getNode()->data;
        max_value.circles_cnt = Max(max_value.circles_cnt, current_values.circles_cnt);
        max_value.rects_cnt = Max(max_value.rects_cnt, current_values.rects_cnt);

        Vector circle_point1(x_coord, last_values.circles_cnt);
        Vector circle_point2(x_coord + plot_step, current_values.circles_cnt);

        cutAndDrawLine(circle_point1, circle_point2, renderer, {0, 255, 0, 255});

        Vector rect_point1(x_coord, last_values.rects_cnt);
        Vector rect_point2(x_coord + plot_step, current_values.rects_cnt);

        cutAndDrawLine(rect_point1, rect_point2, renderer, {255, 0, 0, 0});

        x_coord += plot_step;
        last_values = current_values;
    }
    drawAxis(renderer);
}

void PlotMoleculeCounter::cutAndDrawLine(Vector p1, Vector p2, Renderer* renderer, Color color) {
    p1 = coord_system->translateToAbsolute(p1);
    p2 = coord_system->translateToAbsolute(p2);
    bool point1_is_ok = rectHasPoint(screen_place, p1);
    bool point2_is_ok = rectHasPoint(screen_place, p2);
    if (!point1_is_ok && !point2_is_ok) {
        // not drawing
    } else if (!point1_is_ok) {
        // drawing part
        p1.setY(Min(p1.getY(), screen_place.height + screen_place.y));
        renderer->drawLine(p1, p2, color);
    } else if (!point2_is_ok) {
        // drawing part
        p2.setY(Min(p2.getY(), screen_place.height + screen_place.y));
        renderer->drawLine(p1, p2, color);
    } else {
        // just draw
        renderer->drawLine(p1, p2, color);
    }
}

void PlotMoleculeCounter::shrinkToFit() {
    // Vector abs_size = coord_system->getAbsoluteSize();
    // abs_size.setY((float)Max(max_value.circles_cnt + 2, max_value.rects_cnt + 2));
    // coord_system->setAbsoluteSize(abs_size);

    Vector rel_size = coord_system->getRelativeSize();
    rel_size.setY((float)Max(max_value.circles_cnt + 2, max_value.rects_cnt + 2));
    coord_system->setRelativeSize(rel_size);
}

PlotMoleculeCounter::~PlotMoleculeCounter() {
    delete coord_system;
    delete data;
}