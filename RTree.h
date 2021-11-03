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

            if (current->hasSpace(M))
            {
                current->addData(elem);
                cout << "tiene espacio\n";
            }
            else
            {
                // cout << elem << endl;
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

                    distance_t firstArea;
                    distance_t secondArea;

                    if (firstNode->data.size() == 1)
                    {
                        firstArea = firstNode->data[0].getArea(it);
                    }
                    else
                    {
                        if (firstNode->contains(it))
                        {
                            firstNode->addData(it);
                            continue;
                        }
                        else
                        {
                            firstArea = areaDiff(firstNode, it);
                            // cout << firstArea << endl;
                        }
                    }

                    if (secondNode->data.size() == 1)
                    {
                        secondArea = secondNode->data[0].getArea(it);
                        // cout << secondArea << endl;
                    }
                    else
                    {
                        if (secondNode->contains(it))
                        {
                            secondNode->addData(it);
                            continue;
                        }
                        else
                        {
                            secondArea = areaDiff(secondNode, it);
                        }
                    }

                    if (firstArea < secondArea)
                    {
                        firstNode->addData(it);
                    }
                    else
                    {
                        secondNode->addData(it);
                    }
                }
                current->data.clear();
                current->addNode("e"+to_string(cont++), firstNode);
                current->addNode("e"+to_string(cont++), secondNode);
                root = current;
            }
        }

        void print()
        {
            printRec(root);
        }
};