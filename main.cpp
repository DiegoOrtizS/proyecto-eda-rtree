#include <iostream>
#include "RTree.h"

int main()
{
    RTree *rtree = new RTree(2, 3);
    for (distance_t i = 0; i < 4; i++)
    {
        rtree->insert(point_t({i, i}));
    }
    rtree->insert(point_t({0.5, 0.5}));
    rtree->print();
    cout << endl;
    return 0;
}