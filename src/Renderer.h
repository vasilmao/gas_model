#ifndef RENDERER_INCLUDE
#define RENDERER_INCLUDE

#include <SDL2/SDL.h>
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

    ~Renderer();
    Renderer(int width, int height, Color bg_color, const Rect2f& range_rect);
    void setColor(Color new_color);
    void drawFilledCircle(const Vector& center, const float r) const;
    void drawRect(const Vector& p1, const Vector& p2) const;
    void drawFilledRect(const Vector& p1, const Vector& p2) const;
    void drawSegment(const Vector& p1, const Vector& p2) const;
    void drawLine(const Vector& p1, const Vector& p2) const;
    void render();
    int  getEvent() const;
};

#endif