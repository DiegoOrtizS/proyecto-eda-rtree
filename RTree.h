#include "Node.h"
#include <assert.h>

class RTree
{
    private:
        Node *root;
        size_t M;
        size_t m;
        size_t cont;

        // distance_t areaDiffPoint(Node *node, point_t it)
        // {
        //     auto MBR = node->getMBR();
        //     auto minPoint = MBR.getMinPoint();
        //     auto maxPoint = MBR.getMaxPoint();

        //     auto A = minPoint.getArea(maxPoint);
        //     // cout << minPoint << " " << maxPoint << endl;
        //     // cout << "ÁREA A: " << A << endl;
        //     if (it.get(0) > maxPoint.get(0))
        //     {
        //         maxPoint.set(0, it.get(0));
        //     }
        //     if (it.get(1) > maxPoint.get(1))
        //     {
        //         maxPoint.set(1, it.get(1));
        //     }
        //     if (it.get(0) < minPoint.get(0))
        //     {
        //         minPoint.set(0, it.get(0));
        //     }
        //     if (it.get(1) < minPoint.get(1))
        //     {
        //         minPoint.set(1, it.get(1));
        //     }
        //     auto APrima = minPoint.getArea(maxPoint);
        //     // cout << "ÁREA A': " << APrima << endl;
        //     return APrima - A;
        // }

        // distance_t checkAreaPoint(Node *node, point_t it)
        // {
        //     distance_t area = -1;
        //     if (node->data.size() == 1)
        //     {
        //         area = node->data[0].getArea(it);
        //     }
        //     else
        //     {
        //         if (node->contains(it))
        //         {
        //             node->addData(it);
        //         }
        //         else
        //         {
        //             area = areaDiffPoint(node, it);
        //         }
        //     }
        //     return area;
        // }

        // distance_t areaDiffMBR(Node *node, MBR it)
        // {
        //     auto MBR = node->getMBR();
        //     auto minPoint = MBR.getMinPoint();
        //     auto maxPoint = MBR.getMaxPoint();
        //     auto A = minPoint.getArea(maxPoint);
        //     if (it.maxX > maxPoint.get(0))
        //     {
        //         maxPoint.set(0, it.maxX);
        //     }
        //     if (it.maxY > maxPoint.get(1))
        //     {
        //         maxPoint.set(1, it.maxY);
        //     }
        //     if (it.minX < minPoint.get(0))
        //     {
        //         minPoint.set(0, it.minX);
        //     }
        //     if (it.minY < minPoint.get(1))
        //     {
        //         minPoint.set(1, it.minY);
        //     }
        //     auto APrima = minPoint.getArea(maxPoint);
        //     return APrima - A;
        // }

        void printRec(Node *current)
        {
            if (current->isLeaf())
            {
                // cout << current->mbr << "\t";
                for (auto it : current->data)
                {
                    cout << it << " ";
                }
            }
            for (auto it : current->children)
            {
                // cout << it.first << " ";
                cout << it->mbr << endl;
                // if (!it.second->isLeaf()) cout << endl;
                printRec(it);
                cout << endl;
            }
        }

        // void borrowPoint(Node *first, Node *second)
        // {
        //     int n = first->needBorrow(m);
        //     if (n > 0)
        //     {
        //         while (n--)
        //         {
        //             point_t current;
        //             distance_t minAreaDiff = DBL_MAX;
        //             for (auto it : second->data)
        //             {
        //                 distance_t currentAreaDiff = areaDiffPoint(first, it);
        //                 if (currentAreaDiff < minAreaDiff) 
        //                 {
        //                     current = it;
        //                     minAreaDiff = currentAreaDiff;
        //                 }
        //             }
        //             first->addData(current);
        //             second->deleteValueData(current);
        //         }
        //         return;
        //     }
        //     n = second->needBorrow(m);
        //     if (n > 0)
        //     {
        //         while (n--)
        //         {
        //             point_t current;
        //             distance_t minAreaDiff = DBL_MAX;
        //             for (auto it : first->data)
        //             {
        //                 distance_t currentAreaDiff = areaDiffPoint(second, it);
        //                 if (currentAreaDiff < minAreaDiff) 
        //                 {
        //                     current = it;
        //                     minAreaDiff = currentAreaDiff;
        //                 }
        //             }
        //             second->addData(current);
        //             first->deleteValueData(current);
        //         }
        //     }
        // }

        // void borrowMBR(Node *first, Node *second)
        // {
        //     int n = first->needBorrow(m);
        //     if (n > 0)
        //     {
        //         while (n--)
        //         {
        //             string currentKey;
        //             distance_t minAreaDiff = DBL_MAX;
        //             for (auto it : second->MBRs)
        //             {
        //                 distance_t currentAreaDiff = areaDiffMBR(first, it.second);
        //                 if (currentAreaDiff < minAreaDiff) 
        //                 {
        //                     currentKey = it.first;
        //                     minAreaDiff = currentAreaDiff;
        //                 }
        //             }
        //             first->addNode(currentKey, second->children[currentKey]);
        //             second->deleteChildren(currentKey);
        //         }
        //         return;
        //     }
        //     n = second->needBorrow(m);
        //     if (n > 0)
        //     {
        //         while (n--)
        //         {
        //             string currentKey;
        //             distance_t minAreaDiff = DBL_MAX;
        //             for (auto it : first->MBRs)
        //             {
        //                 distance_t currentAreaDiff = areaDiffMBR(second, it.second);
        //                 if (currentAreaDiff < minAreaDiff) 
        //                 {
        //                     currentKey = it.first;
        //                     minAreaDiff = currentAreaDiff;
        //                 }
        //             }
        //             second->addNode(currentKey, first->children[currentKey]);
        //             first->deleteChildren(currentKey);
        //         }
        //     }
        // }

        void rangeSearch(Node *current, MBR mbrSearch, vector<point_t> &result)
        {
            // if (current->isLeaf())
            // {
            //     for (auto it : current->data)
            //     {
            //         if (mbrSearch.contains(it)) result.push_back(it);
            //     }
            // }
            // else
            // {
            //     for (auto it : current->MBRs)
            //     {
            //         if (mbrSearch.intersect(it.second))
            //         {
            //             rangeSearch(current->children[it.first], mbrSearch, result);
            //         }
            //     }
            // }
        }

        // void getLeaves(Node *current, vector<Node*> &leaves)
        // {
        //     if (current->isLeaf()) leaves.push_back(current);
        //     else
        //     {
        //         for (auto it : current->children)
        //         {
        //             getLeaves(it.second, leaves);
        //         }
        //     }
        // }

        void updateAllTopMBR(Node *current, point_t elem)
        {
            if (current->parent != nullptr)
            {
                current->parent->mbr.updateMBRwithPoint(elem);
                updateAllTopMBR(current->parent, elem);
            }
        }

        pair<Node*, Node*> splitNode(Node *current)//, point_t &elem)
        {
            Node* firstNode = new Node();
            Node* secondNode = new Node();    
            // cout << "enter pick seeds\n";
            pair<int, int> pairIDs = current->pickSeeds();      
            // cout << "finish pick seeds\n";
            vector<int> aux;
            
            if (current->isLeaf())
            {
                for (int i = 0; i < current->data.size(); i++)
                {
                    if (i != pairIDs.first && i != pairIDs.second)
                    {
                        aux.push_back(i);
                    }
                }
                firstNode->mbr.set(current->data[pairIDs.first]);
                firstNode->addData(current->data[pairIDs.first]);
                secondNode->mbr.set(current->data[pairIDs.second]);
                secondNode->addData(current->data[pairIDs.second]);
                // cout << "enter while true\n";
                while (true)
                {
                    if (aux.empty()) break;
                    if (firstNode->data.size() == m - aux.size())
                    {
                        for (auto &it : aux)
                        {
                            firstNode->addData(current->data[it]);
                        }
                        break;
                    }
                    if (secondNode->data.size() == m - aux.size())
                    {
                        for (auto &it : aux)
                        {
                            secondNode->addData(current->data[it]);
                        }
                        break;
                    }
                    // en el par.first se va a insertar el valor de aux en la posición par.second
                    pair<Node*, int> par = current->pickNext(firstNode, secondNode, aux);
                    // cout << par.first->data[0] << " " << par.second << endl;
                    par.first->addData(current->data[par.second]);
                    aux.erase(remove(aux.begin(), aux.end(), par.second), aux.end());
                }
                if (current->parent != nullptr)
                {
                    current->parent->children.erase(remove(current->parent->children.begin(), current->parent->children.end(), current), current->parent->children.end());
                    current->parent->addNode(firstNode);
                    delete current;
                }
                else
                {
                    current->addNode(firstNode);
                }
                return make_pair(firstNode, secondNode);
            }
            else
            {
                // auto aux = current->children;
                for (int i = 0; i < current->children.size(); i++)
                {
                    if (i != pairIDs.first && i != pairIDs.second)
                    {
                        aux.push_back(i);
                    }
                }
                // aux.erase(remove(aux.begin(), aux.end(), pairIDs.first), aux.end());
                // aux.erase(remove(aux.begin(), aux.end(), pairIDs.second), aux.end());
                // firstNode set mbr
                // firstNode->mbr.minX = current->children[pairIDs.first]->mbr.minX;
                // firstNode->mbr.maxX = current->children[pairIDs.first]->mbr.maxX;
                // firstNode->mbr.minY = current->children[pairIDs.first]->mbr.minY;
                // firstNode->mbr.maxY = current->children[pairIDs.first]->mbr.maxY;
                firstNode->addNode(current->children[pairIDs.first]);
                secondNode->addNode(current->children[pairIDs.second]);
                // secondNode set mbr
                // secondNode->mbr.minX = current->children[pairIDs.second]->mbr.minX;
                // secondNode->mbr.maxX = current->children[pairIDs.second]->mbr.maxX;
                // secondNode->mbr.minY = current->children[pairIDs.second]->mbr.minY;
                // secondNode->mbr.maxY = current->children[pairIDs.second]->mbr.maxY;
                
                // cout << "F LAU\n";
                while (true)
                {
                    if (aux.empty()) break;
                    if (firstNode->children.size() == m - aux.size())
                    {
                        for (auto &it : aux)
                        {
                            firstNode->addNode(current->children[it]);
                        }
                        break;
                    }
                    if (secondNode->children.size() == m - aux.size())
                    {
                        for (auto &it : aux)
                        {
                            secondNode->addNode(current->children[it]);
                        }
                        break;
                    }
                    // en el par.first se va a insertar el valor de aux en la posición par.second
                    pair<Node*, int> par = current->pickNext(firstNode, secondNode, aux);
                    par.first->addNode(current->children[par.second]);
                    aux.erase(remove(aux.begin(), aux.end(), par.second), aux.end());
                }
                if (current->parent != nullptr)
                {
                    current->parent->children.erase(remove(current->parent->children.begin(), current->parent->children.end(), current), current->parent->children.end());
                    current->parent->addNode(firstNode);
                    delete current;
                }
                else
                {
                    ///delete root;
                   // root = new Node();
                    auto tmp = new Node();
                    tmp->addNode(firstNode);
                    tmp->addNode(secondNode);
                    root=tmp;
                    delete current;
                    return make_pair(root, nullptr);
                }
                return make_pair(firstNode, secondNode);
            }
        }

        // void recursiveSplit(Node *current, point_t &elem)
        // {
        //     // cout << "split ";
        //     Node* firstNode = new Node();
        //     Node* secondNode = new Node();
        //     if (current->isLeaf())
        //     {
        //         // cout << "hoja ";
        //         pair<point_t, point_t> pairPoints = current->pickSeeds();
        //         firstNode->addData(pairPoints.first);
        //         secondNode->addData(pairPoints.second);
        //         auto aux = current->data;
        //         aux.push_back(elem);
        //         for (auto it : aux)
        //         {
        //             if (it == pairPoints.first || it == pairPoints.second) continue;

        //             distance_t firstArea = checkAreaPoint(firstNode, it);
        //             if (firstArea == -1) continue;
        //             distance_t secondArea = checkAreaPoint(secondNode, it);
        //             if (secondArea == -1) continue;

        //             if (firstArea < secondArea)
        //                 firstNode->addData(it);
        //             else
        //                 secondNode->addData(it);
        //         }
        //         // borrowPoint(firstNode, secondNode);
        //     }
        //     else
        //     {
        //         // cout << "nodo interno ";
        //         auto pairMBRKey = current->twoFurthestAwayMBR();
        //         firstNode->addNode(pairMBRKey.first, current->children[pairMBRKey.first]);
        //         secondNode->addNode(pairMBRKey.second, current->children[pairMBRKey.second]);
        //         for (auto it : current->MBRs)
        //         {
        //             if (it.first == pairMBRKey.first || it.first == pairMBRKey.second) continue;

        //             distance_t firstArea = areaDiffMBR(firstNode, it.second);
        //             distance_t secondArea = areaDiffMBR(secondNode, it.second);

        //             if (firstArea < secondArea)
        //                 firstNode->addNode(it.first, current->children[it.first]);
        //             else
        //                 secondNode->addNode(it.first, current->children[it.first]); 
        //         }
        //         borrowMBR(firstNode, secondNode);
        //     }
        //     if (current == root)
        //     {
        //         // cout << "root\n";
        //         root = new Node();
        //         root->addNode("e"+to_string(cont++), firstNode);
        //         root->addNode("e"+to_string(cont++), secondNode);
        //         delete current;
        //         return;
        //     }
        //     Node *parent = current->parent;
        //     string key = current->getKeyFromParent();
        //     delete current;
        //     if (parent->hasSpace(M))
        //     {
        //         // cout << "padre con espacio\n";
        //         parent->addNode(key, firstNode);
        //         parent->addNode("e"+to_string(cont++), secondNode);
        //         return;
        //     }
        //     // cout << "padre sin espacio\n";
        //     parent->addNode(key, firstNode);
        //     parent->addNode("e"+to_string(cont++), secondNode);
        //     recursiveSplit(parent, elem);
        // }

        void adjustTree(Node *&firstNode, Node *secondNode)
        {
            Node *N = firstNode;
            Node *NN = secondNode;
            if (N == root) return;
            if (secondNode == nullptr)
            {
                N->parent->mbr.updateMBRwithMBR(N->mbr);
                adjustTree(N->parent, nullptr);
            }
            else
            {
                if (N->parent->children.size() < M)
                {
                    N->parent->addNode(NN);
                    adjustTree(N->parent, nullptr);
                }
                else
                {
                    N->parent->addNodeOverflow(NN);
                    // cout << "start split MBR\n";
                    auto par = splitNode(N->parent);
                    //N->parent->mbr.updateMBRwithMBR(NN->mbr);
                    // cout << "finish split MBR\n";
                    adjustTree(par.first, par.second);
                }
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

        Node* getRoot() { return root; }

        Node* search(Node* current, point_t elem)
        {
            if (current->isLeaf()) 
            {
                return current;
            }
            distance_t diffArea, minDiffArea = DBL_MAX;
            Node *bestChildren = nullptr;
            for (auto it : current->children)
            {
                diffArea = it->mbr.areaMBRwithPoint(elem) - it->mbr.getArea();
                if (diffArea < minDiffArea)
                {
                    minDiffArea = diffArea;
                    bestChildren = it;
                }
                // caso de empate
                else if (diffArea == minDiffArea)
                {
                    if (bestChildren->mbr.getArea() > it->mbr.getArea())
                    {
                        bestChildren = it;
                    }
                }
            }
            return search(bestChildren, elem);
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
            // cout << current->data[0] << endl;
            // if (current == nullptr)
            // {
            //     // cout << "no localizado\n";
            //     vector<Node*> leaves;
            //     getLeaves(root, leaves);
            //     distance_t minAreaDiff = DBL_MAX;
            //     for (auto it : leaves)
            //     {
            //         distance_t currentAreaDiff = areaDiffPoint(it, elem);
            //         if (currentAreaDiff < minAreaDiff) 
            //         {
            //             current = it;
            //             minAreaDiff = currentAreaDiff;
            //         }
            //     }
            // }

            if (current->hasSpace(M))
            {
                cout << "tiene espacio\n";
                updateAllTopMBR(current, elem);
                current->addData(elem);
            }
            else
            {
                current->addData(elem);
                cout << "start split\n";
                auto par = splitNode(current);
                cout << "finish split\n";
                // for (auto &it : par.first->data)
                // {
                //     cout << it << " ";   
                // }
                // cout << endl;
                // for (auto &it : par.second->data)
                // {
                //     cout << it << " ";   
                // }
                // cout << endl;
                adjustTree(par.first, par.second);
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
            cout<<root->mbr<<endl;
            printRec(root);
        }
};