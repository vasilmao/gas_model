#include "Button.h"

bool Button::onMouseTest(const Vector& mouse_pos) {
    Vector rect_point2 = pos + size;
    bool x_result = pos.getX() <= mouse_pos.getX() && mouse_pos.getX() <= rect_point2.getX();
    bool y_result = pos.getY() <= mouse_pos.getY() && mouse_pos.getY() <= rect_point2.getY();
    return x_result && y_result;
}

bool Button::onClick(const Vector& mouse_pos) {
    (*functor)();
    return true;
}

void Button::render(float dt, Renderer* renderer) {
    renderer->drawFilledRect(pos, pos + size, {255, 150, 150, 255});
    float text_width = size.getX() -  10;
    float pos_x = pos.getX() + 5;
    float text_height = text_width / strlen(title) * 1.5;
    float pos_y = (pos.getY() + pos.getY() + size.getY()) / 2 - text_height / 2;
    renderer->drawText({pos_x, pos_y}, {text_width, text_height}, title, {0, 0, 255, 255});
}