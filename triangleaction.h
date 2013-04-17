#ifndef TRIANGLEACTION_H
#define TRIANGLEACTION_H

#include "structures.h"

class TriangleAction {
public:
    void run(std::vector<Point*> &points, std::vector<Triangle*> &triangles);

private:
    bool isBuildPossible(Point *p1, Point *p2, Point *p3, Triangle *trian);
    bool isCrossed(Point *p1, Point *p2, Triangle *trian);
    bool isInTriangle(Point *p, Triangle *trian);

    int dotProduct(int x1, int y1, int x2, int y2);
    int dotProduct(Point *p1, Point *p2, Point *p3);
    int getArea(Point *p1, Point *p2, Point *p3);
};

#endif // TRIANGLEACTION_H
