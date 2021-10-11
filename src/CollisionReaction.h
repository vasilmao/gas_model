#ifndef INCLUDE_CHEM_REACT_MANAGER
#define INCLUDE_CHEM_REACT_MANAGER

#include "Shapes.h"
#include "List.h"

#define Max(a, b) a > b ? a : b
#define Min(a, b) a < b ? a : b

void ReactCC(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects);

void ReactRR(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects);

void ReactCR(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects);

void ReactCW(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects);

void ReactRW(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects);

void ReactWW(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects);

typedef void (*chem_react)(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects);

class ReactionManager {
private:
    chem_react reactions_virtual_table[3][3] = {
        {ReactCC, ReactCR, ReactCW},
        {      0, ReactRR, ReactRW},
        {      0,       0, ReactWW}
    };

public:
    ReactionManager(){}
    void chemReaction(List<Shape*>::Iterator it1, List<Shape*>::Iterator it2, List<Shape*>* objects) {
        // printf("chem reaction got!\n");
        // printf("types: %d and %d\n", it1.getNode()->data->getType(), it2.getNode()->data->getType());
        if (it1.getNode()->data->getType() > it2.getNode()->data->getType()) {
            List<Shape*>::Iterator temp = it1;
            it1 = it2;
            it2 = temp;
        }
        int type1 = it1.getNode()->data->getType();
        int type2 = it2.getNode()->data->getType();
        if (type1 < 0 || type2 < 0) {
            return;
        }
        (*reactions_virtual_table[it1.getNode()->data->getType()][it2.getNode()->data->getType()])(it1, it2, objects);
    }
};

#endif