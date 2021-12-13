#include "Node.h"
#include <assert.h>

class RTree
{
    private:
        Node *root;
        size_t M;
        size_t m;
        size_t cont;

        distance_t areaDiffPoint(Node *node, point_t it)
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

        distance_t checkAreaPoint(Node *node, point_t it)
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
                    area = areaDiffPoint(node, it);
                }
            }
            return area;
        }

        distance_t areaDiffMBR(Node *node, MBR it)
        {
            auto MBR = node->getMBR();
            auto minPoint = MBR.getMinPoint();
            auto maxPoint = MBR.getMaxPoint();
            auto A = minPoint.getArea(maxPoint);
            if (it.maxX > maxPoint.get(0))
            {
                maxPoint.set(0, it.maxX);
            }
            if (it.maxY > maxPoint.get(1))
            {
                maxPoint.set(1, it.maxY);
            }
            if (it.minX < minPoint.get(0))
            {
                minPoint.set(0, it.minX);
            }
            if (it.minY < minPoint.get(1))
            {
                minPoint.set(1, it.minY);
            }
            auto APrima = minPoint.getArea(maxPoint);
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
                cout << current->MBRs[it.first];
                if (!it.second->isLeaf()) cout << endl;
                printRec(it.second);
                cout << endl;
            }
        }

        void borrowPoint(Node *first, Node *second)
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
                        distance_t currentAreaDiff = areaDiffPoint(first, it);
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
                        distance_t currentAreaDiff = areaDiffPoint(second, it);
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

        void borrowMBR(Node *first, Node *second)
        {
            int n = first->needBorrow(m);
            if (n > 0)
            {
                while (n--)
                {
                    string currentKey;
                    distance_t minAreaDiff = DBL_MAX;
                    for (auto it : second->MBRs)
                    {
                        distance_t currentAreaDiff = areaDiffMBR(first, it.second);
                        if (currentAreaDiff < minAreaDiff) 
                        {
                            currentKey = it.first;
                            minAreaDiff = currentAreaDiff;
                        }
                    }
                    first->addNode(currentKey, second->children[currentKey]);
                    second->deleteChildren(currentKey);
                }
                return;
            }
            n = second->needBorrow(m);
            if (n > 0)
            {
                while (n--)
                {
                    string currentKey;
                    distance_t minAreaDiff = DBL_MAX;
                    for (auto it : first->MBRs)
                    {
                        distance_t currentAreaDiff = areaDiffMBR(second, it.second);
                        if (currentAreaDiff < minAreaDiff) 
                        {
                            currentKey = it.first;
                            minAreaDiff = currentAreaDiff;
                        }
                    }
                    second->addNode(currentKey, first->children[currentKey]);
                    first->deleteChildren(currentKey);
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

        void recursiveSplit(Node *current, point_t &elem)
        {
            cout << "split ";
            Node* firstNode = new Node();
            Node* secondNode = new Node();
            if (current->isLeaf())
            {
                cout << "hoja ";
                pair<point_t, point_t> pairPoints = current->twoFurthestAwayPoint(elem);
                firstNode->addData(pairPoints.first);
                secondNode->addData(pairPoints.second);
                auto aux = current->data;
                aux.push_back(elem);
                for (auto it : aux)
                {
                    if (it == pairPoints.first || it == pairPoints.second) continue;

                    distance_t firstArea = checkAreaPoint(firstNode, it);
                    if (firstArea == -1) continue;
                    distance_t secondArea = checkAreaPoint(secondNode, it);
                    if (secondArea == -1) continue;

                    if (firstArea < secondArea)
                        firstNode->addData(it);
                    else
                        secondNode->addData(it);
                }
                borrowPoint(firstNode, secondNode);
            }
            else
            {
                cout << "nodo interno ";
                auto pairMBRKey = current->twoFurthestAwayMBR();
                firstNode->addNode(pairMBRKey.first, current->children[pairMBRKey.first]);
                secondNode->addNode(pairMBRKey.second, current->children[pairMBRKey.second]);
                for (auto it : current->MBRs)
                {
                    if (it.first == pairMBRKey.first || it.first == pairMBRKey.second) continue;

                    distance_t firstArea = areaDiffMBR(firstNode, it.second);
                    distance_t secondArea = areaDiffMBR(secondNode, it.second);

                    if (firstArea < secondArea)
                        firstNode->addNode(it.first, current->children[it.first]);
                    else
                        secondNode->addNode(it.first, current->children[it.first]); 
                }
                borrowMBR(firstNode, secondNode);
            }
            if (current == root)
            {
                cout << "root\n";
                root = new Node();
                root->addNode("e"+to_string(cont++), firstNode);
                root->addNode("e"+to_string(cont++), secondNode);
                return;
            }
            string key = current->getKeyFromParent();
            if (current->parent->hasSpace(M))
            {
                cout << "padre con espacio\n";
                current->parent->addNode(key, firstNode);
                current->parent->addNode("e"+to_string(cont++), secondNode);
                return;
            }
            cout << "padre sin espacio\n";
            current->parent->addNode(key, firstNode);
            current->parent->addNode("e"+to_string(cont++), secondNode);
            recursiveSplit(current->parent, elem);
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

        long  WriteNode(Node _reg,const std::string& filename){
            std::fstream outFile;
            outFile.open(filename,std::ios::in| std::ios::out| std::ios::binary | std::ofstream::app);
            long _pos;
            if(outFile.is_open()){
                outFile.seekg(0,std::ios::end);
                _pos = _reg.write(outFile);
                outFile.close();
            }
            return _pos;
        }
  

        Node readNode(long pos,const std::string& filename){
            std::fstream outFile;
            Node obj;
            outFile.open(filename,std::ios::in| std::ios::binary);
            if (outFile.is_open()) {
                outFile.seekg(pos, std::ios::beg);
                obj.read(outFile);
                outFile.close();
            }
            return obj;
        }

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
                    distance_t currentAreaDiff = areaDiffPoint(it, elem);
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
                recursiveSplit(current, elem);
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