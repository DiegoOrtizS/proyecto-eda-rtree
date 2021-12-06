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
            auto MBR = node->getMBR();
            auto minPoint = MBR.getMinPoint();
            auto maxPoint = MBR.getMaxPoint();

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
                cout << current->MBRs[it.first];
                if (!it.second->isLeaf()) cout << endl;
                printRec(it.second);
                cout << endl;
            }
        }

        void borrow(Node *first, Node *second)
        {
            int n = first->needBorrow(m);
            if (n > 0)
            {
                while (n--)
                {
                    point_t current;
                    distance_t minAreaDiff = DBL_MAX;
                    for (auto it : second->data)
                    {
                        distance_t currentAreaDiff = areaDiff(first, it);
                        if (currentAreaDiff < minAreaDiff) 
                        {
                            current = it;
                            minAreaDiff = currentAreaDiff;
                        }
                    }
                    first->addData(current);
                    second->deleteValueData(current);
                }
                return;
            }
            n = second->needBorrow(m);
            if (n > 0)
            {
                while (n--)
                {
                    point_t current;
                    distance_t minAreaDiff = DBL_MAX;
                    for (auto it : first->data)
                    {
                        distance_t currentAreaDiff = areaDiff(second, it);
                        if (currentAreaDiff < minAreaDiff) 
                        {
                            current = it;
                            minAreaDiff = currentAreaDiff;
                        }
                    }
                    second->addData(current);
                    first->deleteValueData(current);
                }
            }
        }

        void rangeSearch(Node *current, MBR mbrSearch, vector<point_t> &result)
        {
            if (current->isLeaf())
            {
                for (auto it : current->data)
                {
                    if (mbrSearch.contains(it)) result.push_back(it);
                }
            }
            else
            {
                for (auto it : current->MBRs)
                {
                    if (mbrSearch.intersect(it.second))
                    {
                        rangeSearch(current->children[it.first], mbrSearch, result);
                    }
                }
            }
        }

        void getLeaves(Node *current, vector<Node*> &leaves)
        {
            if (current->isLeaf()) leaves.push_back(current);
            else
            {
                for (auto it : current->children)
                {
                    getLeaves(it.second, leaves);
                }
            }
        }

        void updateAllTopMBR(Node *current, point_t elem)
        {
            if (current->parent != nullptr)
            {
                current->parent->MBRs[current->getKeyFromParent()].updateMBR(elem);
                updateAllTopMBR(current->parent, elem);
            }
        }

        void deleteByKey(Node *current, string key)
        {
            current->children.erase(key);
            current->MBRs.erase(key);
            if (current->parent != nullptr) deleteByKey(current->parent, current->getKeyFromParent());
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

        Node* getRoot() { return root; }

        Node* search(Node* current, point_t elem)
        {
            if (current->isLeaf()) 
            {
                return current;
            }
            for (auto it : current->MBRs)
            {
                if (it.second.contains(elem))
                {
                    return search(current->children[it.first], elem);
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
                cout << "no localizado\n";
                vector<Node*> leaves;
                getLeaves(root, leaves);
                distance_t minAreaDiff = DBL_MAX;
                for (auto it : leaves)
                {
                    distance_t currentAreaDiff = areaDiff(it, elem);
                    if (currentAreaDiff < minAreaDiff) 
                    {
                        current = it;
                        minAreaDiff = currentAreaDiff;
                    }
                }
            }

            if (current->hasSpace(M))
            {
                cout << "tiene espacio\n";
                updateAllTopMBR(current, elem);
                current->addData(elem);
            }
            else
            {
                cout << "split ";
                pair<point_t, point_t> pairPoints = current->twoFurtherAway(elem);
                // cout << pairPoints.first << " " << pairPoints.second << endl;
                Node* firstNode = new Node();
                firstNode->addData(pairPoints.first);
                Node* secondNode = new Node();
                secondNode->addData(pairPoints.second);
                auto aux = current->data;
                aux.push_back(elem);
                for (auto it : aux)
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
                borrow(firstNode, secondNode);
                if (current == root)
                {
                    cout << "root\n";
                    current->data.clear();
                    current->addNode("e"+to_string(cont++), firstNode);
                    current->addNode("e"+to_string(cont++), secondNode);
                    root = current;
                }
                else
                {
                    string key = current->getKeyFromParent();
                    if (current->parent->hasSpace(M))
                    {
                        cout << "con espacio\n";
                        current->parent->addNode(key, firstNode);
                        current->parent->addNode("e"+to_string(cont++), secondNode);
                    }
                    else
                    {
                        cout << "sin espacio\n";
                        current->addNode(key, firstNode);
                        current->addNode("e"+to_string(cont++), secondNode);
                        Node *parent = current->parent;
                        if (parent == root) root = new Node();
                        deleteByKey(parent, key);
                        root->addNode("e"+to_string(cont++), current);
                        root->addNode("e"+to_string(cont++), parent);
                    }
                }
            }
        }

        vector<point_t> rangeSearch(MBR mbrSearch)
        {
            vector<point_t> result;
            rangeSearch(root, mbrSearch, result);
            return result;
        }

        void print()
        {
            printRec(root);
        }
};