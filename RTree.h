#include "Node.h"
#include <assert.h>

class RTree
{
    private:
        Node *root;
        size_t M;
        size_t m;
        size_t cont;

        distance_t areaDiff(Node *node, point_t it)
        {
            auto minPoint = node->getMinPointMBR();
            auto maxPoint = node->getMaxPointMBR();
            auto A = minPoint.getArea(maxPoint);
            // cout << minPoint << " " << maxPoint << endl;
            // cout << "ÁREA A: " << A << endl;
            if (it.get(0) > maxPoint.get(0))
            {
                maxPoint.set(0, it.get(0));
            }
            if (it.get(1) > maxPoint.get(1))
            {
                maxPoint.set(1, it.get(1));
            }
            if (it.get(0) < minPoint.get(0))
            {
                minPoint.set(0, it.get(0));
            }
            if (it.get(1) < minPoint.get(1))
            {
                minPoint.set(1, it.get(1));
            }
            // cout << minPoint << " " << maxPoint << endl;
            auto APrima = minPoint.getArea(maxPoint);
            // cout << "ÁREA A': " << APrima << endl;
            return APrima - A;
        }

        distance_t checkArea(Node *node, point_t it)
        {
            distance_t area = -1;
            if (node->data.size() == 1)
            {
                area = node->data[0].getArea(it);
            }
            else
            {
                if (node->contains(it))
                {
                    node->addData(it);
                }
                else
                {
                    area = areaDiff(node, it);
                }
            }
            return area;
        }

        void printRec(Node *current)
        {
            if (current->isLeaf())
            {
                for (auto it : current->data)
                {
                    cout << it << " ";
                }
            }
            for (auto it : current->children)
            {
                cout << it.first << " ";
                printRec(it.second);
                cout << endl;
            }
        }

    public:
        RTree(size_t m, size_t M) 
        { 
            assert(m <= ceil(M/2.0));
            root = nullptr; 
            this->m = m;
            this->M = M;
            cont = 1;
        };
        ~RTree() {};

        Node* search(Node* current, point_t elem)
        {
            if (current->isLeaf()) 
            {
                return current;
            }
            for (auto it : current->children)
            {
                if (it.second->contains(elem))
                {
                    return search(it.second, elem);
                }
            }
            return nullptr;
        }

        void insert(point_t elem)
        {
            if (root == nullptr)
            {
                root = new Node();
                root->addData(elem);
                cout << "1er insert\n";
                return;
            }
            Node *current = search(root, elem);
            if (current == nullptr)
            {
                distance_t minAreaDiff = DBL_MAX;
                for (auto it : root->children)
                {
                    distance_t currentAreaDiff = areaDiff(it.second, elem);
                    if (currentAreaDiff < minAreaDiff) 
                    {
                        current = it.second;
                        minAreaDiff = currentAreaDiff;
                    }
                }
            }

            if (current->hasSpace(M))
            {
                current->addData(elem);
                cout << "tiene espacio\n";
            }
            else
            {
                cout << "split node" << endl;
                pair<point_t, point_t> pairPoints = current->twoFurtherAway(elem);
                // cout << pairPoints.first << " " << pairPoints.second << endl;
                Node* firstNode = new Node();
                firstNode->addData(pairPoints.first);
                Node* secondNode = new Node();
                secondNode->addData(pairPoints.second);
              
                for (auto it : current->data)
                {
                    if (it == pairPoints.first || it == pairPoints.second)
                    {
                        continue;
                    }

                    distance_t firstArea = checkArea(firstNode, it);
                    if (firstArea == -1) continue;
                    distance_t secondArea = checkArea(secondNode, it);
                    if (secondArea == -1) continue;

                    if (firstArea < secondArea)
                        firstNode->addData(it);
                    else
                        secondNode->addData(it);
                }
                if (current == root)
                {
                    current->data.clear();
                    current->addNode("e"+to_string(cont++), firstNode);
                    current->addNode("e"+to_string(cont++), secondNode);
                    root = current;
                }
                else
                {
                    if (root->hasSpace(M))
                    {
                        root->addNode("e"+to_string(cont-1), firstNode);
                        root->addNode("e"+to_string(cont++), secondNode);
                    }
                }
            }
        }

        void print()
        {
            printRec(root);
        }
};