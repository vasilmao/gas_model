#include "CoordSystem.h"

Rect2f CoordSystem::getPixelRect() const{
    return {pixel_x, pixel_y, pixel_width, pixel_height};
}

Rect2f CoordSystem::getCoordRect() const {
    return {x_min, y_min, x_max - x_min, y_max - y_min};
}

CoordSystem::CoordSystem(Rect2f range_rect, Rect2f pixel_rect) {
    x_min = range_rect.x;
    y_min = range_rect.y;
    x_max = range_rect.x + range_rect.width;
    y_max = range_rect.y + range_rect.height;

    pixel_x = pixel_rect.x;
    pixel_y = pixel_rect.y;
    pixel_width = pixel_rect.width;
    pixel_height = pixel_rect.height;

    point_to_pixel_coeff_x = pixel_width   / (x_max - x_min);
    point_to_pixel_coeff_y = pixel_height  / (y_max - y_min);
}

Vector CoordSystem::translatePoint(const Vector& point) const {
    // printf("(%f %f %f %f) ___ (%f %f %f %f) (%f %f)\n", x_min, y_min, x_max, y_max, pixel_x, pixel_y, pixel_width, pixel_height, point_to_pixel_coeff_x, point_to_pixel_coeff_y);
    float delta_x  = point.getX() - x_min;
    float delta_y  = point.getY() - y_min;
    float result_x = pixel_x +                 delta_x * point_to_pixel_coeff_x;
    float result_y = pixel_y + (pixel_height - delta_y * point_to_pixel_coeff_y); // (pixel_height - ...) because y counts for top
    return Vector(result_x, result_y);
}

Vector CoordSystem::translatePixel(const Vector& pixel) const {
    // printf("(%f %f %f %f) ___ (%f %f %f %f) (%f %f)\n", x_min, y_min, x_max, y_max, pixel_x, pixel_y, pixel_width, pixel_height, point_to_pixel_coeff_x, point_to_pixel_coeff_y);
    float delta_x  = pixel.getX() - pixel_x;
    float delta_y  = pixel.getY() - pixel_y;
    float result_x = x_min +                    delta_x / point_to_pixel_coeff_x;
    float result_y = y_min + ((y_max - y_min) - delta_y / point_to_pixel_coeff_y); // (pixel_height - ...) because y counts for top
    return Vector(result_x, result_y);
}