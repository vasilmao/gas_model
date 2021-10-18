#ifndef INCLUDE_PLOT_MOLECULE_COUNTER
#define INCLUDE_PLOT_MOLECULE_COUNTER

#include "Shapes.h"
#include "List.h"
#include "MoleculeBox.h"

struct PlotInfo {
    int circles_cnt;
    int rects_cnt;
};

class PlotMoleculeCounter {
private:
    Rect2f screen_place;
    CoordSystem* coord_system;
    List<PlotInfo>* data;
    PlotInfo max_value = {0, 0};
    void cutAndDrawLine(Vector p1, Vector p2, Renderer* renderer, Color color);
public:
    PlotMoleculeCounter(){};
    PlotMoleculeCounter(Vector pos, Vector size);
    void computeNewPoint(MoleculeBox* box);
    void render(float dt, Renderer* renderer);
    void shrinkToFit();
    ~PlotMoleculeCounter();
};

#endif