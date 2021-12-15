#include <vector>
#include <map>
#include <cfloat>
#include <algorithm>
#include "MBR.h"

struct Node
{
    vector<point_t> data;
    vector<Node*> children;
    // map<string, Node*> children;
    Node* parent;
    MBR mbr;
    // map<string, MBR> MBRs;

    Node() { parent = nullptr; };

    // MBR que contiene a todo el nodo
    // MBR getMBR()
    // {
    //     distance_t minX = DBL_MAX, minY = DBL_MAX, maxX = -DBL_MAX, maxY = -DBL_MAX;
    //     if (isLeaf())
    //     {
    //         for (auto it : data)
    //         {
    //             if (it.get(0) < minX) minX = it.get(0);
    //             if (it.get(1) < minY) minY = it.get(1);
    //             if (it.get(0) > maxX) maxX = it.get(0);
    //             if (it.get(1) > maxY) maxY = it.get(1);
    //         }
    //     }
    //     else
    //     {
    //         for (auto it : MBRs)
    //         {
    //             auto mbr = it.second;
    //             if (mbr.minX < minX) minX = mbr.minX;
    //             if (mbr.minY < minY) minY = mbr.minY;
    //             if (mbr.maxX > maxX) maxX = mbr.maxX;
    //             if (mbr.maxY > maxY) maxY = mbr.maxY;
    //         }
    //     }
    //     return MBR{minX, minY, maxX, maxY};
    // }

    void addData(point_t elem)
    {
        data.push_back(elem);
        mbr.updateMBRwithPoint(elem);
    }
    void deleteValueData(point_t elem)
    {
        data.erase(remove(data.begin(), data.end(), elem), data.end());
    }

    void addNode(Node* node)
    {
        children.push_back(node);
        mbr.updateMBRwithMBR(node->mbr);
        node->parent = this;
    }

    bool contains(point_t elem)
    {
        if (mbr.getMinPoint() <= elem && elem <= mbr.getMaxPoint()) return true;
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

    // int needBorrow(size_t m)
    // {
    //     if (isLeaf()) return m-data.size();
    //     return m-children.size();
    // }

    // string getKeyFromParent()
    // {
    //     for (auto it : parent->children)
    //     {
    //         if (it.second == this)
    //         {
    //             return it.first;
    //         }
    //     }
    //     return "";
    // }

    pair<int, int> pickSeeds()
    {
        // EA: EmptyArea
        distance_t maxEA = 0, currentEA;
        int firstID, secondID;
        if (isLeaf())
        {
            for (int i = 0; i < data.size()-1; i++)
            {
                for (int j = i+1; j < data.size(); j++)
                {
                    maxEA = valorAbs(data[i].get(0)-data[i].get(0))*(data[i].get(1)-data[i].get(1));
                    if (maxEA < currentEA)
                    {
                        maxEA = currentEA;
                        firstID = i;
                        secondID = j;
                    }
                }
            }
        }
        else
        {
            for (int i = 0; i < children.size()-1; i++)
            {
                for (int j = i+1; j < children.size(); j++)
                {
                    maxEA = valorAbs(children[i]->mbr.areaMBRwithMBR(children[j]->mbr) - children[i]->mbr.getArea() - children[j]->mbr.getArea());
                    if (maxEA < currentEA)
                    {
                        maxEA = currentEA;
                        firstID = i;
                        secondID = j;
                    }
                }
            }
        }
        return make_pair(firstID, secondID);
    }

    pair<Node*, int> pickNext(Node *firstNode, Node *secondNode, vector<int> aux)
    {
        distance_t maxDiffArea = 0;
        int maxID;
        distance_t firstArea  = firstNode->mbr.getArea();
        distance_t secondArea = secondNode->mbr.getArea();
        distance_t firstDiffArea, secondDiffArea;
        if (isLeaf())
        {
            for (auto &currentID : aux)
            {
                firstDiffArea  = firstNode->mbr.areaMBRwithPoint(data[currentID]) - firstArea;
                secondDiffArea = secondNode->mbr.areaMBRwithPoint(data[currentID]) - secondArea;
                if (valorAbs(firstDiffArea-secondDiffArea) >= maxDiffArea)
                {
                    maxDiffArea = valorAbs(firstDiffArea-secondDiffArea);
                    maxID = currentID;
                }
            }
            firstDiffArea  = firstNode->mbr.areaMBRwithPoint(data[maxID]) - firstArea;
            secondDiffArea = secondNode->mbr.areaMBRwithPoint(data[maxID]) - secondArea;
            if (firstDiffArea == secondDiffArea)
            {
                if (firstArea == secondArea)
                {
                    if (firstNode->data.size() < secondNode->data.size())
                    {
                        return make_pair(firstNode, maxID);
                    }
                    return make_pair(secondNode, maxID);
                }
                if (firstNode->mbr.getArea() < secondNode->mbr.getArea())
                {
                    return make_pair(firstNode, maxID);
                }
                return make_pair(secondNode, maxID);
            }
            if (firstDiffArea < secondDiffArea)
            {
                return make_pair(firstNode, maxID);
            }
            return make_pair(secondNode, maxID);
        }
        else
        {
            for (auto &currentID : aux)
            {
                firstDiffArea  = firstNode->mbr.areaMBRwithMBR(children[currentID]->mbr) - firstArea;
                secondDiffArea = secondNode->mbr.areaMBRwithMBR(children[currentID]->mbr) - secondArea;
                if (valorAbs(firstDiffArea-secondDiffArea) >= maxDiffArea)
                {
                    maxDiffArea = valorAbs(firstDiffArea-secondDiffArea);
                    maxID = currentID;
                }
            }
            firstDiffArea  = firstNode->mbr.areaMBRwithMBR(children[maxID]->mbr) - firstArea;
            secondDiffArea = secondNode->mbr.areaMBRwithMBR(children[maxID]->mbr) - secondArea;
            if (firstDiffArea == secondDiffArea)
            {
                if (firstArea == secondArea)
                {
                    if (firstNode->children.size() < secondNode->children.size())
                    {
                        return make_pair(firstNode, maxID);
                    }
                    return make_pair(secondNode, maxID);
                }
                if (firstNode->mbr.getArea() < secondNode->mbr.getArea())
                {
                    return make_pair(firstNode, maxID);
                }
                return make_pair(secondNode, maxID);
            }
            if (firstDiffArea < secondDiffArea)
            {
                return make_pair(firstNode, maxID);
            }
            return make_pair(secondNode, maxID);
        }
    }

    // pair<point_t, point_t> twoFurthestAwayPoint(point_t elem)
    // {
    //     point_t firstPoint;
    //     point_t secondPoint;
    //     distance_t maxDistance = -1;

    //     vector<point_t> dataAux = data;
    //     dataAux.push_back(elem);

    //     for (int i = 0; i < dataAux.size(); i++)
    //     {
    //         for (int j = i+1; j < dataAux.size(); j++)
    //         {
    //             distance_t currentDistance = dataAux[i].distance(dataAux[j]);
    //             if (currentDistance > maxDistance)
    //             {
    //                 firstPoint = dataAux[i];
    //                 secondPoint = dataAux[j];
    //                 maxDistance = currentDistance;
    //             }
    //         }
    //     }
    //     return make_pair(firstPoint, secondPoint);
    // }

    // pair<string, string> twoFurthestAwayMBR()
    // {
    //     string firstMBRKey;
    //     string secondMBRKey;
    //     distance_t maxDistance = -1;

    //     for (auto it : MBRs)
    //     {
    //         for (auto it2 : MBRs)
    //         {
    //             if (it.first == it2.first) continue;
    //             distance_t currentDistance = it.second.getCentralPoint().distance(it2.second.getCentralPoint());
    //             if (currentDistance > maxDistance)
    //             {
    //                 firstMBRKey = it.first;
    //                 secondMBRKey = it2.first;
    //                 maxDistance = currentDistance;
    //             }
    //         }
    //     }

    //     return make_pair(firstMBRKey, secondMBRKey);
    // }

    // void deleteChildren(string key)
    // {
    //     children.erase(key);
    //     MBRs.erase(key);
    // }

    void printData()
    {
        for (auto it : data)
        {
            cout << it << " ";
        }
        cout << endl;
    }
};