#include "Renderer.h"

#define open_color(color) color.r, color.g, color.b, color.a
#define int32_color(color) (((uint32_t)color.r) << 24) + (((uint32_t)color.g) << 16) + (((uint32_t)color.b) << 8) + (uint32_t)color.a

Renderer::Renderer(int width, int height, Color bg_color) : width(width), height(height), bg_color(bg_color) {
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
}

void Renderer::setColor(Color new_color) {
    current_color = new_color;
    SDL_SetRenderDrawColor(renderer, open_color(current_color));
}

void Renderer::draw(const Circle& circle, const CoordSystem* coord_system) const {
    drawCircle(circle.getCenter(), circle.getR(), coord_system);
}

void Renderer::drawCircle(const Vector& center, const float r, const CoordSystem* coord_system) const {
    float r2 = r * r;
    Vector pixel_left_up_point = coord_system->translatePoint(center - Vector(r, -r));
    Vector pixel_right_down_point = coord_system->translatePoint(center + Vector(r, -r));
    // printf("points: (%f, %f), (%f, %f)\n", pixel_left_up_point.getX(), pixel_left_up_point.getY(), pixel_right_down_point.getX(), pixel_right_down_point.getY());
    for (int py = pixel_left_up_point.getY(); py <= pixel_right_down_point.getY(); ++py) {
        for (int px = pixel_left_up_point.getX(); px <= pixel_right_down_point.getX(); ++px) {
            Vector coord_point = coord_system->translatePixel({px, py});
            // printf("(%d %d) -> (%f %f)\n", px, py, coord_point.getX(), coord_point.getY());
            float x_c = (coord_point.getX() - center.getX());
            float y_c = (coord_point.getY() - center.getY()); 
            if (x_c * x_c + y_c * y_c <= r2) {
                SDL_RenderDrawPointF(renderer, px, py);
            }
        }
    }

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