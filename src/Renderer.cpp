#include "Renderer.h"

#define open_color(color) color.r, color.g, color.b, color.a
#define int32_color(color) (((uint32_t)color.r) << 24) + (((uint32_t)color.g) << 16) + (((uint32_t)color.b) << 8) + (uint32_t)color.a

Renderer::Renderer(int width, int height, Color bg_color, const Rect2f& range_rect) : width(width), height(height), bg_color(bg_color) {
    SDL_Init(SDL_INIT_EVERYTHING);
    int rendererFlags = SDL_RENDERER_ACCELERATED;
    int windowFlags = 0;
    window = SDL_CreateWindow("Patcher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, windowFlags);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");


    // surface = SDL_GetWindowSurface(window);
    renderer = SDL_CreateRenderer(window, -1, rendererFlags);
    // pixels = (int32_t*)surface->pixels;

    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, open_color(bg_color));
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, open_color(current_color));

    coord_system = new CoordSystem(range_rect, {0, 0, width, height});
}

Renderer::~Renderer() {
    delete coord_system;
    printf("renderer destroyed!\n");
}

void Renderer::setColor(Color new_color) {
    current_color = new_color;
    SDL_SetRenderDrawColor(renderer, open_color(current_color));
}

void Renderer::drawFilledCircle(const Vector& center, const float r) const {
    float r2 = r * r;
    Vector center_point(center);
    Vector circle_quadr_diagonal(r, -r);
    center_point -= circle_quadr_diagonal;
    // printf("%p\n", coord_system);
    Vector pixel_left_up_point = coord_system->translatePoint(center_point);
    // printf("drawing filled circle...\n");
    center_point += circle_quadr_diagonal;
    center_point += circle_quadr_diagonal;
    Vector pixel_right_down_point = coord_system->translatePoint(center_point);
    for (int py = pixel_left_up_point.getY(); py <= pixel_right_down_point.getY(); ++py) {
        for (int px = pixel_left_up_point.getX(); px <= pixel_right_down_point.getX(); ++px) {
            Vector coord_point = coord_system->translatePixel({px, py});
            //printf("created and destroyed coord_point\n");
            // printf("(%d %d) -> (%f %f)\n", px, py, coord_point.getX(), coord_point.getY());
            float x_c = (coord_point.getX() - center.getX());
            float y_c = (coord_point.getY() - center.getY()); 
            if (x_c * x_c + y_c * y_c <= r2) {
                SDL_RenderDrawPointF(renderer, px, py);
            }
        }
    }
    // for (int px = pixel_left_up_point.getX(); px < pixel_right_down_point.getX(); ++px) {
    //     Vector real_point = coord_system->translatePixel({px, center_point.getX()});
    //     float delta_x = real_point.getX() - center_point.getX();
    //     float y_up = sqrt(r * r - delta_x * delta_x) + center_point.getY();
    //     float y_down = 2 * center_point.getY() - y_up;
    //     Vector p_up_pixel = coord_system->translatePoint({real_point.getX(), y_up});
    //     Vector p_down_pixel = coord_system->translatePoint({real_point.getX(), y_down});
    //     SDL_RenderDrawLineF(renderer, p_up_pixel.getX(), p_up_pixel.getY(), p_down_pixel.getX(), p_down_pixel.getY());
    // }
}

void Renderer::drawRect(const Vector& p1, const Vector& p2) const {
    Vector pixel_p1 = coord_system->translatePoint(p1);
    Vector pixel_p2 = coord_system->translatePoint(p2);
    SDL_FRect rect = {pixel_p1.getX(), pixel_p1.getY(), pixel_p2.getX(), pixel_p2.getY()};
    SDL_RenderDrawRectF(renderer, &rect);
}

void Renderer::drawFilledRect(const Vector& p1, const Vector& p2) const {
    Vector pixel_p1 = coord_system->translatePoint(p1);
    Vector pixel_p2 = coord_system->translatePoint(p2);
    for (int x = pixel_p1.getX(); x <= pixel_p2.getX(); ++x) {
        SDL_RenderDrawLineF(renderer, x, p1.getY(), x, p2.getY());
    }
    SDL_FRect rect = {p1.getX(), p1.getY(), p2.getX(), p2.getY()};
    SDL_RenderDrawRectF(renderer, &rect);
}

void Renderer::drawSegment(const Vector& p1, const Vector& p2) const {
    Vector pixel_p1 = coord_system->translatePoint(p1);
    Vector pixel_p2 = coord_system->translatePoint(p2);
    SDL_RenderDrawLineF(renderer, pixel_p1.getX(), pixel_p1.getY(), pixel_p2.getX(), pixel_p2.getY());
}

void Renderer::drawLine(const Vector& p1, const Vector& p2) const {
    Vector pixel_p1 = coord_system->translatePoint(p1);
    Vector pixel_p2 = coord_system->translatePoint(p2);
    SDL_RenderDrawLineF(renderer, pixel_p1.getX(), pixel_p1.getY(), pixel_p2.getX(), pixel_p2.getY());
}

void Renderer::render() {
    //SDL_UpdateWindowSurface(window);
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, open_color(bg_color));
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, open_color(current_color));
    // printf("renderpresented!!\n");
}

int Renderer::getEvent() const {
    SDL_Event event;
    int res = SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) {
        return QUIT_EVENT;
    }
    if (res == 0) {
        return NO_EVENT;
    }
    return OTHER_EVENTS;
}