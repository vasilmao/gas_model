#include "App.h"

void parseEvent(App* app, SDL_Event event);

const int WIDTH  = 800;
const int HEIGHT = 600;

const Color bg_color = {185, 226, 235, 255};
const Rect2f range_rect = {0, 0, 400, 300};
const Rect2f pixel_rect = {0, 0, 800, 600};

float function_to_draw(float x) {
    return sin(x);
}

App::App() {
    running = false;

    width = WIDTH;
    height = HEIGHT;

    renderer = new Renderer(width, height, bg_color);
    manager  = new CircleManager({0, 0, 400, 300}, range_rect, pixel_rect);
    const float circles_r = 5;
    for (int xx = range_rect.x + circles_r; xx < range_rect.width - range_rect.x - circles_r; xx += 150) {
        for (int yy = range_rect.y + circles_r; yy < range_rect.height - range_rect.y - circles_r; yy += 150) {
            manager->addCircle(Circle({xx, yy}, circles_r, {((float)rand()) / ((float)RAND_MAX), ((float)rand()) / ((float)RAND_MAX)}));
        }
    }
    manager->addCircle(Circle({10, 10}, 5, {1, 1}));
    manager->addCircle(Circle({10, 40}, 5, {-1, -1}));
    manager->addCircle(Circle({10, 60}, 5, {1, 0}));
    manager->addCircle(Circle({10, 80}, 5, {0, -1}));
    manager->addCircle(Circle({10, 100}, 5, {1, 0.3}));
    manager->addCircle(Circle({10, 120}, 5, {0.5, 0.7}));
    manager->addCircle(Circle({10, 140}, 5, {0.6, -0.4}));
    manager->addCircle(Circle({10, 160}, 5, {0.5, -0.3}));
    renderer->setColor({255, 255, 255, 255});
}

App::~App() {
    delete manager;
    delete renderer;
    return;
}

void App::run() {
    running = true;
    clock_t current_time = 0;
    clock_t previous_time = clock();
    while (running) {
        int event_result = renderer->getEvent();
        while (event_result > 0) {
            parseEvent(event_result);
            event_result = renderer->getEvent();
        }
        current_time = clock();
        manager->update((float)(current_time - previous_time) / 1000);
        manager->render(renderer);
        printf("%lf\n", CLOCKS_PER_SEC / ((double)(current_time - previous_time)));
        previous_time = current_time;
        SDL_Delay(10);
    }
}

void App::parseEvent(int event) {
    switch (event)
    {
    case QUIT_EVENT:
        running = false;
        break;
    
    default:
        break;
    }
}