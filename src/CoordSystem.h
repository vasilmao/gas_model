#ifndef INCLUDE_COORD_SYSTEM
#define INCLUDE_COORD_SYSTEM
#include "Tools.h"
#include "Vector.h"

class PixelCoordSystem {

private:
    float x_min = 0;
    float y_min = 0;
    float x_max = 0;
    float y_max = 0;

    float pixel_x = 0;
    float pixel_y = 0;
    float pixel_width  = 0;
    float pixel_height = 0;

    float point_to_pixel_coeff_x = 0;
    float point_to_pixel_coeff_y = 0;

public:
    PixelCoordSystem(){}
    ~PixelCoordSystem(){
        printf("coord system destroyed!\n");
    }

    PixelCoordSystem(Rect2f range_rect, Rect2f pixel_rect);

    Rect2f getPixelRect() const;
    Rect2f getCoordRect() const;
    Vector translatePoint(const Vector& point) const;
    Vector translatePixel(const Vector& pixel) const;
};

class CoordSystem {

private:
    Vector relative_pos;
    Vector relative_size;

    Vector absolute_pos;
    Vector absolute_size;

    float rel_to_abs_coeff_x = 0;
    float rel_to_abs_coeff_y = 0;

public:
    CoordSystem(){}
    ~CoordSystem(){
        printf("coord system destroyed!\n");
    }

    CoordSystem(Rect2f range_rect, Rect2f pixel_rect);

    // Rect2f getPixelRect() const;
    // Rect2f getCoordRect() const;
    Vector translateToRelative(const Vector& point) const;
    Vector translateToAbsolute(const Vector& pixel) const;
};

#endif