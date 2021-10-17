#include "App.h"

Shape::~Shape() {}

void parseEvent(App* app, SDL_Event event);

const int WIDTH  = 1000;
const int HEIGHT = 750;

const Color bg_color = {185, 226, 235, 255};
const Rect2f range_rect = {0, 0, 800, 600};
const Rect2f pixel_rect = {0, 0, WIDTH, HEIGHT};

float function_to_draw(float x) {
    return sin(x);
}

App::App() {
    running = false;

    width = WIDTH;
    height = HEIGHT;
    renderer = new Renderer(width, height, bg_color, range_rect);
    box = new MoleculeBox({5, 200}, {450, 390});
    plot = new PlotMoleculeCounter({5, 5}, {450, 190});
    // objects.push_back(new Circle({10, 10}, 5, {1, 1}, 1));
    // objects.push_back(new Circle({40, 40}, 5, {-1, -1}, 1));


    
    // renderer->setColor({255, 255, 255, 255});
}

App::~App() {
    delete renderer;
    delete box;
    delete plot;
    printf("app destroyed!\n");
    return;
}

void App::run() {
    running = true;
    int cnt = 10;
    while (running) {
        int event_result = renderer->getEvent();
        while (event_result > 0) {
            parseEvent(event_result);
            event_result = renderer->getEvent();
        }
        float dt = 0.002;
        
        box->update(dt);
        box->render(dt, renderer);
        ++cnt;
        if (cnt % 100 == 0) {
            plot->computeNewPoint(box);
            cnt = 0;
        }
        plot->render(dt, renderer);
        
        // printf("%6d - cnt; %6f - mass; %8f - energy\n", obj_cnt, objects.getSize(), all_mass, nrj);
        renderer->render();
        // SDL_Delay(10);
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