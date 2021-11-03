#include <iostream>
#include "RTree.h"

int main()
{
    RTree *rtree = new RTree(2, 3);
    char letra = 'A';
    for (distance_t i = 0; i < 4; i++)
    {
        rtree->insert(point_t(letra++, {i, i}));
    }
    rtree->insert(point_t(letra++, {0.5, 0.5}));
    rtree->insert(point_t(letra++, {4, 2.5}));
    rtree->insert(point_t(letra++, {4, 4}));
    rtree->print();
    cout << endl;
    return 0;
}