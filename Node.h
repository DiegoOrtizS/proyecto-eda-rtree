#include <vector>
#include <map>
#include <cfloat>
#include "Point.h"

using namespace std;

struct Node
{
    vector<point_t> data;
    // vector<Node*> children;
    map<string, Node*> children;

    Node() {};

    point_t getMinPointMBR()
    {
        distance_t minX = DBL_MAX;
        distance_t minY = DBL_MAX;
        for (auto it : data)
        {
            if (it.get(0) < minX)
            {
                minX = it.get(0);
            }
            if (it.get(1) < minY)
            {
                minY = it.get(1);
            }
        }
        return point_t({minX, minY});
    }

    point_t getMaxPointMBR()
    {
        distance_t maxX = DBL_MIN;
        distance_t maxY = DBL_MIN;
        for (auto it : data)
        {
            if (it.get(0) > maxX)
            {
                maxX = it.get(0);
            }
            if (it.get(1) > maxY)
            {
                maxY = it.get(1);
            }
        }
        return point_t({maxX, maxY});
    }    

    void addData(point_t elem)
    {
        data.push_back(elem);
    }
    void addNode(string key, Node* node)
    {
        children[key] = node;
    }

    bool contains(point_t elem)
    {
        if (getMinPointMBR() <= elem && elem <= getMaxPointMBR()) return true;
        return false;
    }

    bool isLeaf()
    {
        return children.size() == 0;
    }

    bool hasSpace(size_t M)
    {
        return children.size() < M && data.size() < M;
    }

    pair<point_t, point_t> twoFurtherAway(point_t elem)
    {
        point_t firstPoint;
        point_t secondPoint;
        distance_t maxDistance = -1;

        vector<point_t> dataAux = data;
        dataAux.push_back(elem);

        for (int i = 0; i < dataAux.size(); i++)
        {
            for (int j = i+1; j < dataAux.size(); j++)
            {
                if (dataAux[i].distance(dataAux[j]) > maxDistance)
                {
                    firstPoint = dataAux[i];
                    secondPoint = dataAux[j];
                    maxDistance = dataAux[i].distance(dataAux[j]);
                }
            }
        }
        return make_pair(firstPoint, secondPoint);
    }

    void printData()
    {
        for (auto it : data)
        {
            cout << it << " ";
        }
        cout << endl;
    }
};