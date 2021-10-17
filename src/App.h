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

class App {
private:
    friend Renderer;
    Renderer* renderer;

    MoleculeBox* box;
    PlotMoleculeCounter* plot;
    
    bool running;

    int width;
    int height;

    void parseEvent(int event);

public:
    App();

    void run();

    ~App();
};

#endif