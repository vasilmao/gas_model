#ifndef RENDERER_INCLUDE
#define RENDERER_INCLUDE

#include <SDL2/SDL.h>
#include "Circle.h"
#include "CoordSystem.h"
#include "Tools.h"

enum Events {
    NO_EVENT,
    QUIT_EVENT,
    OTHER_EVENTS
};

class Renderer {
private:
    SDL_Window* window     = NULL;
    SDL_Renderer* renderer = NULL;
    // SDL_Surface* surface   = NULL;

    CoordSystem* coord_system;

    // int32_t* pixels = NULL;
    int width = 0;
    int height = 0;

    Color bg_color;
    Color current_color;


public:

    Renderer(){}

    Renderer(int width, int height, Color bg_color);
    void setColor(Color new_color);
    void draw(const Circle& circle, const CoordSystem* coord_system) const;
    void drawCircle(const Vector& center, const float r, const CoordSystem* coord_system) const; // circle too
    void render();
    int  getEvent() const;
};

#endif