#include "Node.h"

class RTree
{
    private:
        Node *root;
        size_t M;
        size_t m;

        distance_t areaDiff(Node *node, point_t it)
        {
            auto minPoint = node->getMinPointMBR();
            auto maxPoint = node->getMaxPointMBR();
            auto A = minPoint.getArea(maxPoint);

            if (it.get(0) > maxPoint.get(0))
            {
                maxPoint.set(0, it.get(0));
            }
            if (it.get(1) > maxPoint.get(1))
            {
                maxPoint.set(1, it.get(1));
            }
            if (it.get(0) > minPoint.get(0))
            {
                minPoint.set(0, it.get(0));
            }
            if (it.get(1) > minPoint.get(1))
            {
                minPoint.set(1, it.get(1));
            }

            auto APrima = minPoint.getArea(maxPoint);
            return APrima - A;
        }

    public:
        RTree(size_t M, size_t m) 
        { 
            root = nullptr; 
            this->M = M;
            this->m = m;
        };
        ~RTree() {};

        Node* search(Node* current, point_t elem)
        {
            if (current->isLeaf()) return current;
            for (auto it : current->children)
            {
                if (it->contains(elem))
                {
                    return search(it, elem);
                }
            }
        }

        void insert(point_t elem)
        {
            Node* current = search(root, elem);

            if (current->hasSpace(M))
            {
                current->addData(elem);
            }
            else
            {
                pair<point_t, point_t> pairPoints = current->twoFurtherAway(elem);
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
                        }
                    }

                    if (secondNode->data.size() == 1)
                    {
                        secondArea = secondNode->data[0].getArea(it);
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
                auto parentNode = new Node();
                parentNode->addNode(firstNode);
                parentNode->addNode(secondNode);
            }
        }
};