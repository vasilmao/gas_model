#ifndef INCLUDE_MOLECULE_BOX
#define INCLUDE_MOLECULE_BOX

#include "Shapes.h"
#include "CollisionManager.h"
#include "CollisionReaction.h"
#include "CoordSystem.h"
#include "Tools.h"
#include "DynamicArray.h"



class MoleculeBox {
private:
    Rect2f screen_place;
    CoordSystem* coord_system;
    CollisionManager* manager;
    ReactionManager* chem_manager;
    // DynamicArray<Renderable*> renderable_objects;
    // DynamicArray<PhysShape*>  phys_objects;
    List<Shape*>* objects;
public:
    MoleculeBox(Vector pos, Vector size);
    List<Shape*>* getObjects() {
        return objects;
    }
    void heatWalls();
    void update(float dt);
    void render(float dt, Renderer* renderer);
};

#endif