#include "GraphicLib.h"

#define open_color(color) color.r, color.g, color.b, color.a
#define int32_color(color) (((uint32_t)color.r) << 24) + (((uint32_t)color.g) << 16) + (((uint32_t)color.b) << 8) + (uint32_t)color.a

Renderer::Renderer(int width, int height, Color bg_color, const Rect2f& range_rect) : width(width), height(height), bg_color(bg_color) {
    SDL_Init(SDL_INIT_EVERYTHING);
    int rendererFlags = SDL_RENDERER_ACCELERATED;
    int windowFlags = 0;
    window = SDL_CreateWindow("Patcher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, windowFlags);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");


    renderer = SDL_CreateRenderer(window, -1, rendererFlags);

    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, open_color(bg_color));
    SDL_RenderClear(renderer);

    TTF_Init();
    font = TTF_OpenFont("Anonymous.ttf", 10);
    assert(font);

    coord_system = new PixelCoordSystem(range_rect, {0.0f, 0.0f, (float)width, (float)height});
}

Renderer::~Renderer() {
    delete coord_system;
    printf("renderer destroyed!\n");
}

void Renderer::drawFilledCircle(const Vector& center, const float r, Color color) const {
    SDL_SetRenderDrawColor(renderer, open_color(color));
    float r2 = r * r;
    Vector center_point(center);
    Vector circle_quadr_diagonal(r, -r);
    center_point -= circle_quadr_diagonal;
    Vector pixel_left_up_point = coord_system->translatePoint(center_point);
    center_point += circle_quadr_diagonal;
    center_point += circle_quadr_diagonal;
    Vector pixel_right_down_point = coord_system->translatePoint(center_point);
    for (int py = pixel_left_up_point.getY(); py <= pixel_right_down_point.getY(); ++py) {
        for (int px = pixel_left_up_point.getX(); px <= pixel_right_down_point.getX(); ++px) {
            Vector coord_point = coord_system->translatePixel({(float)px, (float)py});
            float x_c = (coord_point.getX() - center.getX());
            float y_c = (coord_point.getY() - center.getY()); 
            if (x_c * x_c + y_c * y_c <= r2) {
                SDL_RenderDrawPointF(renderer, px, py);
            }
        }
    }
    SDL_SetRenderDrawColor(renderer, open_color(bg_color));
}

void Renderer::drawRect(const Vector& p1, const Vector& p2, Color color) const {
    Vector pixel_p1 = coord_system->translatePoint(p1);
    Vector pixel_p2 = coord_system->translatePoint(p2);
    SDL_FRect rect = {pixel_p1.getX(), pixel_p1.getY(), pixel_p2.getX() - pixel_p1.getX(), pixel_p2.getY() - pixel_p1.getY()};
    SDL_SetRenderDrawColor(renderer, open_color(color));
    SDL_RenderDrawRectF(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, open_color(bg_color));
}

void Renderer::drawFilledRect(const Vector& p1, const Vector& p2, Color color) const {
    Vector pixel_p1 = coord_system->translatePoint(p1);
    Vector pixel_p2 = coord_system->translatePoint(p2);
    SDL_SetRenderDrawColor(renderer, open_color(color));
    for (int x = pixel_p1.getX(); x <= pixel_p2.getX(); ++x) {
        SDL_RenderDrawLineF(renderer, x, pixel_p1.getY(), x, pixel_p2.getY());
    }
    SDL_SetRenderDrawColor(renderer, open_color(bg_color));
}

void Renderer::drawSegment(const Vector& p1, const Vector& p2, Color color) const {
    Vector pixel_p1 = coord_system->translatePoint(p1);
    Vector pixel_p2 = coord_system->translatePoint(p2);
    SDL_SetRenderDrawColor(renderer, open_color(color));
    SDL_RenderDrawLineF(renderer, pixel_p1.getX(), pixel_p1.getY(), pixel_p2.getX(), pixel_p2.getY());
    SDL_SetRenderDrawColor(renderer, open_color(bg_color));
}

void Renderer::drawLine(const Vector& p1, const Vector& p2, Color color) const {
    Vector pixel_p1 = coord_system->translatePoint(p1);
    Vector pixel_p2 = coord_system->translatePoint(p2);
    SDL_SetRenderDrawColor(renderer, open_color(color));
    SDL_RenderDrawLine(renderer, (int)pixel_p1.getX(), (int)pixel_p1.getY(), (int)pixel_p2.getX(), (int)pixel_p2.getY());
    SDL_RenderDrawLine(renderer, (int)pixel_p1.getX(), (int)pixel_p1.getY(), (int)pixel_p2.getX(), (int)pixel_p2.getY());
    SDL_SetRenderDrawColor(renderer, open_color(bg_color));
}

void Renderer::drawThickLine(const Vector& p1, const Vector& p2, Color color) const {
    Vector pixel_p1 = coord_system->translatePoint(p1);
    Vector pixel_p2 = coord_system->translatePoint(p2);
    SDL_SetRenderDrawColor(renderer, open_color(color));
    SDL_RenderDrawLineF(renderer, pixel_p1.getX(),     pixel_p1.getY(),     pixel_p2.getX(),     pixel_p2.getY()    );
    SDL_RenderDrawLineF(renderer, pixel_p1.getX() - 1, pixel_p1.getY(),     pixel_p2.getX() - 1, pixel_p2.getY()    );
    SDL_RenderDrawLineF(renderer, pixel_p1.getX(),     pixel_p1.getY() - 1, pixel_p2.getX(),     pixel_p2.getY() - 1);
    SDL_RenderDrawLineF(renderer, pixel_p1.getX() - 1, pixel_p1.getY() - 1, pixel_p2.getX() - 1, pixel_p2.getY() - 1);

    SDL_SetRenderDrawColor(renderer, open_color(bg_color));
}

void Renderer::render() {
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, open_color(bg_color));
    SDL_RenderClear(renderer);
    SDL_Delay(1);
    // SDL_SetRenderDrawColor(renderer, open_color(current_color));
}

SystemEvent Renderer::getEvent() const {
    SDL_Event event;
    int res = SDL_PollEvent(&event);
    SystemEvent result_event; // oh...
    if (res == 0) {
        result_event.event_type = NO_EVENT;
        return result_event;
    }
    switch (event.type)
    {
        case SDL_QUIT:
            result_event.event_type = QUIT_EVENT;
            break;

        case SDL_MOUSEBUTTONDOWN:
            result_event.event_type = MOUSE_BUTTON_DOWN;
            result_event.mouse_pos = {coord_system->translatePixel(Vector(event.button.x, event.button.y))};
            break;
        
        
        default:
            result_event.event_type = OTHER_EVENTS;
            break;
    }
    return result_event;
}

void Renderer::drawText(const Vector& pos, const Vector& size, const char* text, Color color) {
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, text, {open_color(color)});
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    Vector p1 = coord_system->translatePoint(pos);
    Vector p2 = coord_system->translatePoint(pos + size);
    float y = p1.getY();
    p1.setY(p2.getY());
    p2.setY(y);
    // printf("%f %f %f %f\n", p1.getX(), p1.getY(), p2.getX(), p2.getY());
    SDL_Rect rect = {(int)p1.getX(), (int)p1.getY(), (int)(p2 - p1).getX(), (int)(p2 - p1).getY()};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(text_surface);
}