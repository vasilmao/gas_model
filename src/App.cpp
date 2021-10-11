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
    chem_manager = new ChemistryManager();

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

    objects.add(new Rect({100, 100}, {30, 30}, {40, 0}, 8));
    objects.add(new Rect({150, 100}, {30, 30}, {-30, 0}, 8));

    objects.add(new Rect({200, 200}, {30, 30}, {40, 0}, 8));
    objects.add(new Rect({250, 200}, {30, 30}, {-30, 0}, 8));


    // objects.add(new Circle({140, 140}, 5, {3, 0}, 1));
    // objects.add(new Circle({160, 160}, 5, {0, -3}, 1));

    // objects.add(new Circle({190, 110}, 5, {-3, 3}, 1));

    // objects.add(new Rect({220 - 10, 80 - 10}, 10, {-3, 3}, 2));


    //shapes.push_back(new Wall())
    // printf("adding circle\n");
    // objects.add(new Circle({20, 20}, 4, {1, 1}, 4));
    // objects.add(new Circle({20, 40}, 4, {-1, -1}, 4));
    // objects.add(new Circle({20, 60}, 4, {1, 0}, 4));
    // objects.add(new Circle({20, 80}, 4, {0, -1}, 4));
    // objects.add(new Circle({20, 100}, 5, {1, 0.3}, 5));
    // objects.add(new Circle({20, 120}, 6, {0.5, 0.7}, 6));
    // objects.add(new Circle({20, 140}, 7, {0.6, -0.4}, 7));
    // objects.add(new Circle({20, 160}, 8, {0.5, -0.3}, 8));
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
    //clock_t current_time = 0;
    //clock_t previous_time = clock();
    while (running) {
        // printf("checking events...\n");
        int event_result = renderer->getEvent();
        while (event_result > 0) {
            parseEvent(event_result);
            event_result = renderer->getEvent();
        }
        //current_time = clock();
        //float dt = (current_time - previous_time) / 1000;
        float dt = 0.001;
        // printf("colliding...\n");
        DynamicArray<CollisionInfo> collisions;
        for (List<Shape*>::Iterator i = objects.begin(); i.isValid(); ++i) {
            // printf("iterating i!\n");
            // printf("i.getnode = %p\n", i.getNode());
            // printf("i.right = %p\n", i.getNode()->getNext());
            // printf("da!\n");
            for (List<Shape*>::Iterator j = i + 1; j.isValid(); ++j) {
                // printf("iterating j!\n");
                // printf("j.getnode = %p\n", j.getNode());
                // printf("da!\n");
                float collision_time = manager->detectCollision(i.getNode()->data->getPhysObject(), j.getNode()->data->getPhysObject(), dt);
                // printf("collided in %f\n", collision_time);
                if (collision_time > -(1e-6)) {
                    // they collided
                    // printf("they collided! in %f\n", collision_time);
                    // printf("%p %p ya1\n", i.getNode(), j.getNode());
                    CollisionInfo info = {i, j, collision_time};
                    collisions.push_back(info);
                    // printf("%p %p %f\n", collisions[collisions.length() - 1].it1.getNode(), collisions[collisions.length() - 1].it1.getNode(), collisions[collisions.length() - 1].collision_time);
                    // SDL_Delay(5000);
                    // running = false;
                }
            }
            // objects[i]->getPhysObject()->move(dt);
            // objects[i]->translateCoords();
        }
        // printf("sorting...\n");
        // for (int i = 0; i < collisions.length(); ++i) {
        //     printf("%p %p %f\n", collisions[i].it1.getNode(), collisions[i].it1.getNode(), collisions[i].collision_time);
        // }
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
        // printf("sorted!\n");
        // for (int i = 0; i < collisions.length(); ++i) {
        //     printf("%p %p %f\n", collisions[i].it1.getNode(), collisions[i].it1.getNode(), collisions[i].collision_time);
        // }
        // printf("responding...\n");
        for (int i = 0; i < collisions.length(); ++i) {
            // printf("collision number %d\n", i);
            List<Shape*>::Iterator it1 = collisions[i].it1;
            List<Shape*>::Iterator it2 = collisions[i].it2;
            if (it1.isValid() && it2.isValid()) {
                // printf("they are valid!\n");
                chem_manager->chemReaction(it1, it2, &objects);
            }
        }
        // printf("clearing...\n");
        objects.clear();
        // printf("moving... dt is %f\n", dt);
        
        for (List<Shape*>::Iterator i = objects.begin(); i.isValid(); ++i) {
            i.getNode()->data->getPhysObject()->move(dt);
            i.getNode()->data->translateCoords();
        }
        // printf("rendering...\n");
        float all_mass = 0;
        float nrj = 0;
        int obj_cnt = 0;
        for (List<Shape*>::Iterator i = objects.begin(); i.isValid(); ++i) {
            all_mass += i.getNode()->data->getPhysObject()->getMass();
            nrj += i.getNode()->data->getPhysObject()->getMass() * i.getNode()->data->getPhysObject()->getSpeed().getLength() * i.getNode()->data->getPhysObject()->getSpeed().getLength();
            if (i.getNode()->data->getPhysObject()->getType() == 1) {
                nrj += ((Rect*)i.getNode()->data)->potential_energy;
            }
            ++obj_cnt;
            i.getNode()->data->getRenderObject()->render(renderer);
        }

        printf("%6d - cnt; %6f - mass; %8f - energy\n", obj_cnt, objects.getSize(), all_mass, nrj);

        renderer->render();
        // printf("%lf\n", CLOCKS_PER_SEC / ((double)(current_time - previous_time)));
        // previous_time = current_time;
        // if (objects.getSize() > 6) {
        //     SDL_Delay(5000);
        //     running = false;
        // }
    }
    // printf("uh!\n");
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