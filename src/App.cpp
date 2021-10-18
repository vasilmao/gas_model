#include "App.h"

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
    plot_button = new Button({460, 5}, {100, 190}, plot_button_functor);

    AbstractFunctor* walls_button_functor = reinterpret_cast<AbstractFunctor*>(new WallHeaterButtonFunctor(box));
    wall_heat_button = new Button({460, 200}, {100, 390}, walls_button_functor);
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
    int cnt = 99;
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
        float dt = 0.002;
        box->update(dt);
        printf("kek\n");
        box->render(dt, renderer);
        ++cnt;
        if (cnt % 100 == 0) {
            plot->computeNewPoint(box);
            cnt = 0;
        }
        plot->render(dt, renderer);
        plot_button->render(dt, renderer);
        wall_heat_button->render(dt, renderer);
        
        renderer->render();
        // SDL_Delay(10);
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
    default:
        break;
    }
}