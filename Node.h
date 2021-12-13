#include <vector>
#include <map>
#include <cfloat>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "MBR.h"

struct Node
{
    long address{-1};
    vector<point_t> data;
    map<string, Node*> children;
    map<string, long > childrenAddress;
    map<string, MBR> MBRs;
    Node* parent;
    bool isleaf;
    long parentAddress{-1};

   long write(std::fstream &stream) {
    long pos_begin=stream.tellg();
    isleaf=isLeaf();
    int sizePoint=data.size();
    int sizeChildren=children.size();
    int sizeMBR=MBRs.size();
    double pX;
    double pY;
    char key;
    long toaddresChild=sizeof(isleaf);
    stream.write((char *) &isleaf, sizeof(isleaf));
    stream.write((char *) &parentAddress, sizeof(parentAddress));
    if(isleaf){
        stream.write((char *) &sizePoint, sizeof(sizePoint));
        for(int i=0;i<sizePoint;i++){
             auto tmp=data[i];
             key=tmp.getKey();
             pX=tmp.get(0);
             pY=tmp.get(1);
             stream.write((char *) &key, sizeof(key));
             stream.write((char *) &pX, sizeof(pX));
             stream.write((char *) &pY, sizeof(pY));     
        } 
    }else{
        stream.write((char *) &sizeChildren, sizeof(sizeChildren));
        vector<long> addresToInsert;
        for (auto & element : children) {
            string keymap=element.first;
            long currentAddressChild=element.second->address;
            addresToInsert.push_back(currentAddressChild);
            // stream.write((char *) &keymap, sizeof(keymap));
            writeString(stream,keymap);
            stream.write((char *) &currentAddressChild, sizeof(currentAddressChild));
        }
        stream.write((char *) &sizeMBR, sizeof(sizeMBR));
        for (auto & element : MBRs) {
            string mbrKey=element.first;
            auto mbrNode=element.second;
            double minX=mbrNode.minX;
            double minY=mbrNode.minY;
            double maxX=mbrNode.maxX;
            double maxY=mbrNode.maxY;
            //stream.write((char *) &mbrKey, sizeof(mbrKey));
            writeString(stream,mbrKey);
            stream.write((char *) &minX, sizeof(minX));
            stream.write((char *) &minY, sizeof(minY));
            stream.write((char *) &maxX, sizeof(maxX));
            stream.write((char *) &maxY, sizeof(maxY));
        }

        for (auto & element : addresToInsert) {
            stream.seekp(element+toaddresChild);
            stream.write((char *) &pos_begin, sizeof(parentAddress));
        }

    }
    return pos_begin;
   }

   void writeString(fstream &stream, string str) {
        int len = str.size();
        stream.write((char *) &len, sizeof(len));
        stream.write(str.c_str(), len);
   }

   string readString(fstream &stream) {
        int len;
        stream.read((char *) &len, sizeof(len));
        char *buffer = new char[len + 1];
        stream.read(buffer, len);
        buffer[len] = '\0';
        string result = buffer;
        delete buffer;
        return result;
    }   


   bool read(std::fstream &stream){
        stream.read((char *) &isleaf, sizeof(isleaf));
        stream.read((char *) &parentAddress, sizeof(parentAddress));
        if(isleaf){
            int sizePoint=0;
            stream.read((char *) &sizePoint, sizeof(sizePoint));
            for(int i=0;i<sizePoint;i++){
                double pX;
                double pY;
                char key;
                stream.write((char *) &key, sizeof(key));
                stream.write((char *) &pX, sizeof(pX));
                stream.write((char *) &pY, sizeof(pY));
                data.push_back(point_t(key, {pX,pY}));
            } 
        }else{
            int sizeChildren=0;
            stream.read((char *) &sizeChildren, sizeof(sizeChildren));
            for(int i=0;i<sizeChildren;i++){
                string keymap;
                long currentAddressChild;
                stream.read((char *) &keymap, sizeof(keymap));
                stream.read((char *) &currentAddressChild, sizeof(currentAddressChild));
                childrenAddress[keymap] = currentAddressChild;
            }
            int sizeMBR=0;
            stream.read((char *) &sizeMBR, sizeof(sizeMBR));
            for(int i=0;i<sizeMBR;i++){
                string mbrKey=readString(stream);
                double minX;
                double minY;
                double maxX;
                double maxY;
                stream.write((char *) &minX, sizeof(minX));
                stream.write((char *) &minY, sizeof(minY));
                stream.write((char *) &maxX, sizeof(maxX));
                stream.write((char *) &maxY, sizeof(maxY));
                MBRs[mbrKey]=MBR{minX, minY, maxX, maxY};
            }
        }

        if (stream.fail()) return false;
        return true;
   }

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

    pair<point_t, point_t> twoFurthestAwayPoint(point_t elem)
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
                distance_t currentDistance = dataAux[i].distance(dataAux[j]);
                if (currentDistance > maxDistance)
                {
                    firstPoint = dataAux[i];
                    secondPoint = dataAux[j];
                    maxDistance = currentDistance;
                }
            }
        }
        return make_pair(firstPoint, secondPoint);
    }

    pair<string, string> twoFurthestAwayMBR()
    {
        string firstMBRKey;
        string secondMBRKey;
        distance_t maxDistance = -1;

        for (auto it : MBRs)
        {
            for (auto it2 : MBRs)
            {
                if (it.first == it2.first) continue;
                distance_t currentDistance = it.second.getCentralPoint().distance(it2.second.getCentralPoint());
                if (currentDistance > maxDistance)
                {
                    firstMBRKey = it.first;
                    secondMBRKey = it2.first;
                    maxDistance = currentDistance;
                }
            }
        }

        return make_pair(firstMBRKey, secondMBRKey);
    }

    void deleteChildren(string key)
    {
        children.erase(key);
        MBRs.erase(key);
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