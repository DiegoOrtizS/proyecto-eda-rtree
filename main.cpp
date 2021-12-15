#include <iostream>
#include "RTree.h"

int main()
{
    RTree *rtree = new RTree(2, 3);
    char letra = 'A';
    // A, B, C, D
    for (distance_t i = 0; i < 4; i++)
    {
        rtree->insert(point_t(to_string(letra++), {i, i}));
    }
    // rtree->print();

    // rtree->insert(point_t(to_string(letra++), {0.5, 0.5})); // E
    // rtree->insert(point_t(to_string(letra++), {4, 2.5})); // F
    // rtree->insert(point_t(to_string(letra++), {4, 4})); // G, split
    // rtree->insert(point_t(to_string(letra++), {3.5, 5.5})); // H
    // rtree->insert(point_t(to_string(letra++), {0.75, 2.75})); // I
    // rtree->insert(point_t(to_string(letra++), {-1, -1})); // J, split
    // rtree->insert(point_t(to_string(letra++), {1.25, 1.25})); // K
    // rtree->insert(point_t(to_string(letra++), {0.1, 0.1})); // L
    // rtree->insert(point_t(to_string(letra++), {0.8, 0.8})); // M, split
    // rtree->insert(point_t(to_string(letra++), {-0.5, -0.5})); // N, split
    // rtree->insert(point_t(to_string(letra++), {-0.25, -0.25})); // O
    // rtree->insert(point_t(to_string(letra++), {-0.15, -0.15})); // P
    // rtree->insert(point_t(to_string(letra++), {-0.1, -0.1})); // Q
    // rtree->insert(point_t(to_string(letra++), {-0.05, -0.05})); // R
    // rtree->insert(point_t(to_string(letra++), {-2, -2})); // S
    // rtree->insert(point_t(to_string(letra++), {1.05, 1.05})); // T
    // rtree->insert(point_t(to_string(letra++), {1.15, 1.15})); // U
    // rtree->insert(point_t(to_string(letra++), {-0.75, -0.75})); // V

    // rtree->print();
    // auto res = rtree->rangeSearch(MBR{0,0,2.5,2.5});
    // for (auto it : res) cout << it << " ";
    // cout << endl;
    // res = rtree->rangeSearch(MBR{2.5,2.5,5,5});
    // for (auto it : res) cout << it << " ";
    // cout << endl;
    
    return 0;
}