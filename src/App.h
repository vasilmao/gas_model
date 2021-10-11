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

class App {
private:
    friend Renderer;
    Renderer* renderer;
    CollisionManager* manager;
    ReactionManager* chem_manager;
    // DynamicArray<Renderable*> renderable_objects;
    // DynamicArray<PhysShape*>  phys_objects;
    List<Shape*> objects;
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