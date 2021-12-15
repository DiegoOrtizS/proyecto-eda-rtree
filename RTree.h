#include "Node.h"
#include <assert.h>

class RTree
{
    private:
        Node *root;
        size_t M;
        size_t m;
        size_t cont;

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

        void rangeSearch(Node *current, MBR mbrSearch, vector<point_t> &result)
        {
            if (current->isLeaf()) 
            {
                if (current->mbr.intersect(mbrSearch)) 
                {
                    for (auto &it: current->data) 
                    {
                        if (mbrSearch.contains(it))
                        {
                            result.push_back(it);
                        }
                    }
                }
            }
            else
            {
                for (auto &it: current->children) 
                {
                    if (it->mbr.intersect(mbrSearch)) 
                    {
                        rangeSearch(it, mbrSearch, result);
                    }
                }
            }
        }

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
                // cout << "PICK SEEDS:\n";

                firstNode->mbr = current->children[pairIDs.first]->mbr;
                secondNode->mbr = current->children[pairIDs.second]->mbr;
                firstNode->addNodeOverflow(current->children[pairIDs.first]);
                secondNode->addNodeOverflow(current->children[pairIDs.second]);
                // cout << firstNode->mbr << endl;
                // cout << secondNode->mbr << endl;
                
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
                    auto tmp = new Node();
                    tmp->addNode(firstNode);
                    tmp->addNode(secondNode);
                    root=tmp;
                    delete current;
                    return make_pair(firstNode, nullptr);
                }
                return make_pair(firstNode, secondNode);
            }
        }
        
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
                // cout << "1er insert\n";
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
                // cout << "tiene espacio\n";
                updateAllTopMBR(current, elem);
                current->addData(elem);
            }
            else
            {
                current->addDataOverflow(elem);
                // cout << "start split\n";
                auto par = splitNode(current);
                // cout << "finish split\n";
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