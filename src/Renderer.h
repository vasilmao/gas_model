#ifndef RENDERER_INCLUDE
#define RENDERER_INCLUDE

#include <SDL2/SDL.h>
#include "CoordSystem.h"
#include "Tools.h"

enum EventTypes {
    NO_EVENT,
    QUIT_EVENT,
    MOUSE_BUTTON_DOWN,
    OTHER_EVENTS
};

struct MousePos {
    Vector pos;
};

struct BigEvent {
    char data[64];
};

struct SystemEvent {
    int event_type;
    union
    {
        MousePos mouse_pos;
        BigEvent big_event;
    };
    SystemEvent(){}
    SystemEvent(const SystemEvent& other_event){
        event_type = other_event.event_type;
        for (int i = 0; i < 64; ++i) {
            big_event.data[i] = other_event.big_event.data[i];
        }
    }
    ~SystemEvent(){}
};

class Renderer {
private:
    SDL_Window* window     = NULL;
    SDL_Renderer* renderer = NULL;
    // SDL_Surface* surface   = NULL;

    PixelCoordSystem* coord_system;

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
    void drawFilledCircle(const Vector& center, const float r, Color color) const;
    void drawRect(const Vector& p1, const Vector& p2, Color color) const;
    void drawFilledRect(const Vector& p1, const Vector& p2, Color color) const;
    void drawSegment(const Vector& p1, const Vector& p2, Color color) const;
    void drawLine(const Vector& p1, const Vector& p2, Color color) const;
    void render();
    SystemEvent getEvent() const;
};

#endif