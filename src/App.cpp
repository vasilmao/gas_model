#include "App.h"

Shape::~Shape() {}

void parseEvent(App* app, SDL_Event event);

const int WIDTH  = 800;
const int HEIGHT = 600;

const Color bg_color = {185, 226, 235, 255};
const Rect2f range_rect = {0, 0, 400, 300};
const Rect2f pixel_rect = {0, 0, 800, 600};

struct CollisionInfo {
    List<Shape*>::Iterator it1;
    List<Shape*>::Iterator it2;
    float collision_time;
};

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
    chem_manager = new ReactionManager();

    // objects.push_back(new Circle({10, 10}, 5, {1, 1}, 1));
    // objects.push_back(new Circle({40, 40}, 5, {-1, -1}, 1));
    printf("adding wall\n");
    objects.add(new Wall({5, 5}, {395, 5}));
    printf("adding wall\n");
    objects.add(new Wall({5, 5}, {5, 295}));
    printf("adding wall\n");
    objects.add(new Wall({395, 5}, {395, 295}));
    printf("adding wall\n");
    objects.add(new Wall({5, 295}, {395, 295}));
    printf("ye\n");

    objects.add(new Rect({100, 100}, {30, 30}, {40, 0}, 20));
    objects.add(new Rect({150, 100}, {30, 30}, {-30, 0}, 20));

    objects.add(new Rect({200, 200}, {30, 30}, {40, 0}, 20));
    objects.add(new Rect({220, 200}, {30, 30}, {-30, 0}, 20));


    
    // renderer->setColor({255, 255, 255, 255});
}

App::~App() {
    delete renderer;
    delete manager;
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
        DynamicArray<CollisionInfo> collisions;
        for (List<Shape*>::Iterator i = objects.begin(); i.isValid(); ++i) {
            for (List<Shape*>::Iterator j = i + 1; j.isValid(); ++j) {
                float collision_time = manager->detectCollision(i.getNode()->data->getPhysObject(), j.getNode()->data->getPhysObject(), dt);
                if (collision_time > -(1e-6)) {
                    CollisionInfo info = {i, j, collision_time};
                    collisions.push_back(info);
                }
            }
        }
        qsort(collisions.begin(), collisions.length(), sizeof(CollisionInfo), [](const void* el1, const void* el2){
            CollisionInfo* col1 = (CollisionInfo*) el1;
            CollisionInfo* col2 = (CollisionInfo*) el2;
            if (fabs(col1->collision_time - col2->collision_time) < 1e-6) {
                return 0;
            }
            if (col1->collision_time < col2->collision_time) {
                return -1;
            }
            return 1;
        });
        for (int i = 0; i < collisions.length(); ++i) {
            List<Shape*>::Iterator it1 = collisions[i].it1;
            List<Shape*>::Iterator it2 = collisions[i].it2;
            if (it1.isValid() && it2.isValid()) {
                chem_manager->chemReaction(it1, it2, &objects);
            }
        }
        objects.clear();
        
        float all_mass = 0;
        float nrj = 0;
        int obj_cnt = 0;

        for (List<Shape*>::Iterator i = objects.begin(); i.isValid(); ++i) {
            i.getNode()->data->getPhysObject()->move(dt);
            i.getNode()->data->translateCoords();
            all_mass += i.getNode()->data->getPhysObject()->getMass();
            nrj += i.getNode()->data->getPhysObject()->getMass() * i.getNode()->data->getPhysObject()->getSpeed().getLength() * i.getNode()->data->getPhysObject()->getSpeed().getLength() / 2;
            if (i.getNode()->data->getType() == 1) {
                nrj += ((Rect*)(i.getNode()->data))->potential_energy;
            }
            ++obj_cnt;
        }
        for (List<Shape*>::Iterator i = objects.begin(); i.isValid(); ++i) {
            i.getNode()->data->getRenderObject()->render(renderer);
        }

        printf("%6d - cnt; %6f - mass; %8f - energy\n", obj_cnt, objects.getSize(), all_mass, nrj);

        renderer->render();
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