#include "App.h"

Shape::~Shape() {}

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
    // DynamicArray<Shape*> shapes;
    // DynamicArray<PhysShape*> phys_shapes;
    // DynamicArray<Renderable*> render_shapes;
    renderer = new Renderer(width, height, bg_color, range_rect);
    box = new MoleculeBox({5, 5}, {350, 290});
    // objects.push_back(new Circle({10, 10}, 5, {1, 1}, 1));
    // objects.push_back(new Circle({40, 40}, 5, {-1, -1}, 1));


    
    // renderer->setColor({255, 255, 255, 255});
}

App::~App() {
    delete renderer;
    delete box;
    printf("app destroyed!\n");
    return;
}

void App::run() {
    running = true;
    while (running) {
        int event_result = renderer->getEvent();
        while (event_result > 0) {
            parseEvent(event_result);
            event_result = renderer->getEvent();
        }
        float dt = 0.002;
        
        box->update(dt);
        box->render(dt, renderer);
        // printf("%6d - cnt; %6f - mass; %8f - energy\n", obj_cnt, objects.getSize(), all_mass, nrj);
        renderer->render();
        // SDL_Delay(1);
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