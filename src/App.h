#ifndef INCLUDE_APP
#define INCLUDE_APP

#include <time.h>
#include <stdio.h>
#include "RenderableShapes.h"
#include "PhysShapes.h"
#include "Renderer.h"
#include "DynamicArray.h"
#include "Tools.h"
#include "CollisionManager.h"
#include "CollisionReaction.h"
#include "Shapes.h"
#include "List.h"
#include "MoleculeBox.h"
#include "PlotMoleculeCounter.h"
#include "Button.h"

class App {
private:
    friend Renderer;
    Renderer* renderer;

    MoleculeBox* box;
    PlotMoleculeCounter* plot;
    Button* plot_button;
    Button* wall_heat_button;
    
    bool running;

    int width;
    int height;

    void parseEvent(const SystemEvent& event);

public:
    App();

    void run();

    ~App();
};

#endif