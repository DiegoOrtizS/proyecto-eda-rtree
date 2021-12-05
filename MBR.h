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
};

ostream& operator<<(ostream& os, MBR &mbr) {
    return os << "{(" << mbr.minX << ", " << mbr.minY << "), (" << mbr.maxX << ", " << mbr.maxY << ")}: ";
}
