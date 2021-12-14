#include <vector>
#include <map>
#include <cfloat>
#include <algorithm>
#include "MBR.h"

struct Node
{
    vector<point_t> data;
    map<string, Node*> children;
    map<string, MBR> MBRs;
    Node* parent;

    Node() { parent = nullptr; };

    // MBR que contiene a todo el nodo
    MBR getMBR()
    {
        distance_t minX = DBL_MAX, minY = DBL_MAX, maxX = -DBL_MAX, maxY = -DBL_MAX;
        if (isLeaf())
        {
            for (auto it : data)
            {
                if (it.get(0) < minX) minX = it.get(0);
                if (it.get(1) < minY) minY = it.get(1);
                if (it.get(0) > maxX) maxX = it.get(0);
                if (it.get(1) > maxY) maxY = it.get(1);
            }
        }
        else
        {
            for (auto it : MBRs)
            {
                auto mbr = it.second;
                if (mbr.minX < minX) minX = mbr.minX;
                if (mbr.minY < minY) minY = mbr.minY;
                if (mbr.maxX > maxX) maxX = mbr.maxX;
                if (mbr.maxY > maxY) maxY = mbr.maxY;
            }
        }
        return MBR{minX, minY, maxX, maxY};
    }

    void addData(point_t elem)
    {
        data.push_back(elem);
    }
    void deleteValueData(point_t elem)
    {
        data.erase(remove(data.begin(), data.end(), elem), data.end());
    }

    void addNode(string key, Node* node)
    {
        children[key] = node;
        MBRs[key] = node->getMBR();
        node->parent = this;
    }

    bool contains(point_t elem)
    {
        auto MBR = getMBR();
        if (MBR.getMinPoint() <= elem && elem <= MBR.getMaxPoint()) return true;
        return false;
    }

    bool isLeaf()
    {
        return children.size() == 0;
    }

    bool hasSpace(size_t M)
    {
        if (isLeaf()) return data.size() < M;
        return children.size() < M;
    }

    int needBorrow(size_t m)
    {
        if (isLeaf()) return m-data.size();
        return m-children.size();
    }

    string getKeyFromParent()
    {
        for (auto it : parent->children)
        {
            if (it.second == this)
            {
                return it.first;
            }
        }
        return "";
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