#include <iostream>
#include "RTree.h"

int main()
{
    RTree *rtree = new RTree(2, 3);
    char letra = 'A';
    // A, B, C, D
    for (distance_t i = 0; i < 4; i++)
    {
        rtree->insert(point_t(letra++, {i, i}));
    }
    rtree->insert(point_t(letra++, {0.5, 0.5}));
    rtree->insert(point_t(letra++, {4, 2.5}));
    rtree->insert(point_t(letra++, {4, 4}));
    rtree->insert(point_t(letra++, {3.5, 5.5}));
    /* BUG: no se actualiza el MBR cuando se inserta en una hoja */
    // rtree->insert(point_t(letra++, {3.5, 5.5}));
    // rtree->insert(point_t(letra++, {0.75, 2.75}));
    rtree->print();
    // split recursivo falta
    return 0;
}