#ifndef TRIANGLEACTION_H
#define TRIANGLEACTION_H

#include <vector>
#include "structures.h"

class TriangleAction {
public:
    TriangleAction(std::vector<Point*>& pnts) : points(pnts) {}

    void run();
    std::vector<Triangle*> getTriangles();
private:
    std::vector<Point*> points;
    std::vector<Triangle*> triangles;
};

struct classCmp {
    bool operator()(Point *p1, Point *p2) {
        return p1->getY() < p2->getY() ||
                p1->getY() == p2->getY() && p1->getX() < p2->getX();
    }
};

#endif // TRIANGLEACTION_H
