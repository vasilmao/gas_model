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

    manager = new CollisionManager();

    // objects.push_back(new Circle({10, 10}, 5, {1, 1}, 1));
    // objects.push_back(new Circle({40, 40}, 5, {-1, -1}, 1));

    objects.push_back(new Wall({5, 5}, {395, 5}));
    objects.push_back(new Wall({5, 5}, {5, 295}));
    objects.push_back(new Wall({395, 5}, {395, 295}));
    objects.push_back(new Wall({5, 295}, {395, 295}));

    //shapes.push_back(new Wall())

    objects.push_back(new Circle({20, 10}, 4, {1, 1}, 4));
    objects.push_back(new Circle({20, 40}, 4, {-1, -1}, 4));
    objects.push_back(new Circle({20, 60}, 4, {1, 0}, 4));
    objects.push_back(new Circle({20, 80}, 4, {0, -1}, 4));
    objects.push_back(new Circle({20, 100}, 5, {1, 0.3}, 5));
    objects.push_back(new Circle({20, 120}, 6, {0.5, 0.7}, 6));
    objects.push_back(new Circle({20, 140}, 7, {0.6, -0.4}, 7));
    objects.push_back(new Circle({20, 160}, 8, {0.5, -0.3}, 8));
    renderer->setColor({255, 255, 255, 255});
}

App::~App() {
    delete renderer;
    delete manager;
    printf("app destroyed!\n");
    return;
}

void App::run() {
    running = true;
    clock_t current_time = 0;
    clock_t previous_time = clock();

    while (running) {
        // printf("cycle started\n");
        int event_result = renderer->getEvent();
        while (event_result > 0) {
            parseEvent(event_result);
            event_result = renderer->getEvent();
        }
        current_time = clock();
        // printf("updating...\n");
        float dt = (current_time - previous_time) / 1000;
        // float dt = 1;
        for (int i = 0; i < objects.length(); ++i) {
            // printf("%d start\n", i);
            for (int j = i + 1; j < objects.length(); ++j) {
                // printf("j: %d \n", j);
                // printf("%p, %p\n", objects[i]->getPhysObject(), objects[j]->getPhysObject());
                manager->collide(objects[i]->getPhysObject(), objects[j]->getPhysObject(), dt);
                // printf("j: %d collided! \n", j);
            }
            // printf("%d end\n", i);
            objects[i]->getPhysObject()->move(dt);
            objects[i]->translateCoords();
        }

        // printf("rendering...\n");
        for (int i = 0; i < objects.length(); ++i) {
            // printf("e\n");
            objects[i]->getRenderObject()->render(renderer);
        }
        renderer->render();
        printf("%lf\n", CLOCKS_PER_SEC / ((double)(current_time - previous_time)));
        previous_time = current_time;
        SDL_Delay(10);
        // running = false;
    }
    printf("uh!\n");
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