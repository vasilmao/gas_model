#include "App.h"

const float time_delta_coefficient = 0.000003;

Shape::~Shape() {}

class PlotShrinkerButtonFunctor : AbstractFunctor {
private:
    PlotMoleculeCounter* plot;
public:
    PlotShrinkerButtonFunctor(PlotMoleculeCounter* plot) : plot(plot) {}
    virtual void function() {
        plot->shrinkToFit();
    }
    virtual void operator()(){
        function();
    }
};

class WallHeaterButtonFunctor : AbstractFunctor {
private:
    MoleculeBox* box;
public:
    WallHeaterButtonFunctor(MoleculeBox* box) : box(box) {}
    virtual void function() {
        box->heatWalls();
    }
    virtual void operator()(){
        function();
    }
};

const float new_ball_r = 2.0;

class BallSpawnerButtonFunctor : AbstractFunctor {
private:
    MoleculeBox* box;
public:
    BallSpawnerButtonFunctor(MoleculeBox* box) : box(box) {}
    virtual void function() {
        Rect2f borders = box->getBox();
        float x = (random() % (int)(borders.width - 4 * new_ball_r))  + borders.x + 2 * new_ball_r;
        float y = (random() % (int)(borders.height - 4 * new_ball_r)) + borders.y + 2 * new_ball_r;
        Circle* circle = new Circle({x, y}, new_ball_r, {0, 0}, 1);
        if (!(box->addMolecule(circle))) {
            delete circle;
        }
    }
    virtual void operator()(){
        function();
    }
};

const int WIDTH  = 1000;
const int HEIGHT = 750;

const Color bg_color = {185, 226, 235, 255};
const Rect2f range_rect = {0, 0, 800, 600};
const Rect2f pixel_rect = {0, 0, WIDTH, HEIGHT};

App::App() {
    running = false;

    width = WIDTH;
    height = HEIGHT;
    renderer = new Renderer(width, height, bg_color, range_rect);
    box = new MoleculeBox({5, 200}, {450, 390});
    plot = new PlotMoleculeCounter({5, 5}, {450, 190});

    AbstractFunctor* plot_button_functor = reinterpret_cast<AbstractFunctor*>(new PlotShrinkerButtonFunctor(plot));
    plot_button = new Button({460, 5}, {120, 190}, plot_button_functor, "Shrink to fit plot");

    AbstractFunctor* walls_button_functor = reinterpret_cast<AbstractFunctor*>(new WallHeaterButtonFunctor(box));
    wall_heat_button = new Button({460, 500}, {120, 90}, walls_button_functor, "Heat walls");

    AbstractFunctor* ball_spawn_functor = reinterpret_cast<AbstractFunctor*>(new BallSpawnerButtonFunctor(box));
    ball_spawn_button = new Button({460, 400}, {120, 90}, ball_spawn_functor, "Spawn ball");
}

App::~App() {
    delete renderer;
    delete box;
    delete plot;
    delete plot_button;
    delete wall_heat_button;
    delete ball_spawn_button;
    printf("app destroyed!\n");
    return;
}

void App::run() {
    running = true;
    int cnt = 19;
    float last_time = clock();
    float cur_time = 0;
    while (running) {
        SystemEvent event_result = renderer->getEvent();
        while (event_result.event_type > 0) {
            parseEvent(event_result);
            if (!running) {
                break;
            }
            event_result = renderer->getEvent();
        }
        // printf("events passed!\n");
        if (!running) {
            break;
        }

        // float dt = 0.008;
        cur_time = clock();
        float dt = (cur_time - last_time) * time_delta_coefficient;
        box->update(dt);
        box->render(dt, renderer);
        ++cnt;
        if (cnt % 20 == 0) {
            plot->computeNewPoint(box);
            cnt = 0;
        }
        plot->render(dt, renderer);
        plot_button->render(dt, renderer);
        wall_heat_button->render(dt, renderer);
        ball_spawn_button->render(dt, renderer);
        
        renderer->render();
        last_time = cur_time;
    }
}

void App::parseEvent(const SystemEvent& event) {
    switch (event.event_type)
    {
    case QUIT_EVENT:
        running = false;
        break;
    case MOUSE_BUTTON_DOWN:
        if (plot_button->onMouseTest(event.mouse_pos.pos)) {
            plot_button->onClick(event.mouse_pos.pos);
        }
        if (wall_heat_button->onMouseTest(event.mouse_pos.pos)) {
            wall_heat_button->onClick(event.mouse_pos.pos);
        }
        if (ball_spawn_button->onMouseTest(event.mouse_pos.pos)) {
            ball_spawn_button->onClick(event.mouse_pos.pos);
        }
    default:
        break;
    }
}