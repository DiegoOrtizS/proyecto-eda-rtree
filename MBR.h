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
};

ostream& operator<<(ostream& os, MBR &mbr) {
    return os << "{(" << mbr.minX << ", " << mbr.minY << "), (" << mbr.maxX << ", " << mbr.maxY << ")}: ";
}
