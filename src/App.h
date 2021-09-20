#include <SDL2/SDL.h>
#include <time.h>
#include <stdio.h>
#include "Renderer.h"
#include "DynamicArray.h"
#include "Tools.h"
#include "CircleManager.h"

class App {
private:
    friend Renderer;
    Renderer* renderer;
    CircleManager* manager;
    bool running;

    int width;
    int height;

    void parseEvent(int event);

public:
    App();

    void run();

    ~App();
};