#include "Point.h"

using namespace std;

struct MBR
{
    distance_t minX;
    distance_t minY;
    distance_t maxX;
    distance_t maxY;

    point_t getMinPoint() { return point_t({minX, minY}); };
    point_t getMaxPoint() { return point_t({maxX, maxY}); };

    point_t getCentralPoint()
    {
        return point_t({(minX+maxX)/2, (minY+maxY)/2});
    }

    bool intersect(MBR other)
    {
        if (minX >= other.maxX || other.minX >= maxX)
            return false;

        if (minY >= other.maxY || other.minY >= maxY)
            return false;

        return true;
    }

    bool contains(point_t elem)
    {
        if (getMinPoint() <= elem && elem <= getMaxPoint()) return true;
        return false;
    }
    
    void updateMBRwithPoint(point_t elem)
    {
        if (elem.get(0) < minX) minX = elem.get(0);
        if (elem.get(1) < minY) minY = elem.get(1);
        if (elem.get(0) > maxX) maxX = elem.get(0);
        if (elem.get(1) > maxY) maxY = elem.get(1);
    }

    void updateMBRwithMBR(MBR mbr)
    {
        if (mbr.minX < minX) minX = mbr.minX;
        if (mbr.minY < minY) minY = mbr.minY;
        if (mbr.maxX > maxX) maxX = mbr.maxX;
        if (mbr.maxY > maxY) maxY = mbr.maxY;
    }

    distance_t areaMBRwithPoint(point_t elem)
    {
        MBR mbrAux = *this;
        if (elem.get(0) < mbrAux.minX) mbrAux.minX = elem.get(0);
        if (elem.get(1) < mbrAux.minY) mbrAux.minY = elem.get(1);
        if (elem.get(0) > mbrAux.maxX) mbrAux.maxX = elem.get(0);
        if (elem.get(1) > mbrAux.maxY) mbrAux.maxY = elem.get(1);
        return mbrAux.getArea();
    }

    distance_t areaMBRwithMBR(MBR mbr)
    {
        MBR mbrAux = *this;
        if (mbr.minX < mbrAux.minX) mbrAux.minX = mbr.minX;
        if (mbr.minY < mbrAux.minY) mbrAux.minY = mbr.minY;
        if (mbr.maxX > mbrAux.maxX) mbrAux.maxX = mbr.maxX;
        if (mbr.maxY > mbrAux.maxY) mbrAux.maxY = mbr.maxY;
        return mbrAux.getArea();
    }

    distance_t getArea()
    {
        return valorAbs((maxX-minX)*(maxY-minY));
    }

    void set(point_t elem)
    {
        minX = maxX = elem.get(0);
        minY = maxY = elem.get(1);
    }
};

ostream& operator<<(ostream& os, MBR &mbr) {
    return os << "{(" << mbr.minX << ", " << mbr.minY << "), (" << mbr.maxX << ", " << mbr.maxY << ")}: ";
}
