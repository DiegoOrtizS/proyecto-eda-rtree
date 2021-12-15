#include <vector>
#include <map>
#include <cfloat>
#include <algorithm>
#include "MBR.h"

struct Node
{
    vector<point_t> data;
    vector<Node*> children;
    Node* parent;
    MBR mbr;

    Node() { parent = nullptr; };

    void addData(point_t elem)
    {
        data.push_back(elem);
        mbr.updateMBRwithPoint(elem);
    }

    void addDataOverflow(point_t elem){
        data.push_back(elem);
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

    void addNodeOverflow(Node * node){
        children.push_back(node);
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
                    currentEA = valorAbs((data[i].get(0)-data[j].get(0))*(data[i].get(1)-data[j].get(1)));
                    // cout << data[i] << " " << data[j] << ": " << currentEA << endl;
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
                    currentEA = valorAbs(children[i]->mbr.areaMBRwithMBR(children[j]->mbr) - children[i]->mbr.getArea() - children[j]->mbr.getArea());
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
        // cout << firstArea << endl;
        distance_t secondArea = secondNode->mbr.getArea();
        // cout << secondArea << endl;
        distance_t firstDiffArea, secondDiffArea;
        if (isLeaf())
        {
            for (auto &currentID : aux)
            {
                firstDiffArea  = firstNode->mbr.areaMBRwithPoint(data[currentID]) - firstArea;
                secondDiffArea = secondNode->mbr.areaMBRwithPoint(data[currentID]) - secondArea;
                // cout << firstDiffArea << " " << secondDiffArea << endl;
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
            // cout << "+++++\n";
            // cout << secondNode->data[0] << endl;
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

    void printData()
    {
        for (auto it : data)
        {
            cout << it << " ";
        }
        cout << endl;
    }
};