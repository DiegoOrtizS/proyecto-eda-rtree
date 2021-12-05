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
    rtree->insert(point_t(letra++, {0.75, 2.75}));
    rtree->print();
    auto res = rtree->rangeSearch(MBR{0,0,2.5,2.5});
    for (auto it : res) cout << it << " ";
    cout << endl;
    res = rtree->rangeSearch(MBR{2.5,2.5,5,5});
    for (auto it : res) cout << it << " ";
    cout << endl;
    return 0;
}