#ifndef TRIANGLEACTION_H
#define TRIANGLEACTION_H

#include "structures.h"

class TriangleAction {
public:
    void run(std::vector<Point*> &points, std::vector<Triangle*> &triangles);

private:
    void runThread(std::vector<Point*> &points,
                   std::vector<Triangle*> &triangles);
    void doConvex(std::vector<Point*> &points,
                  std::vector<Triangle*> &triangles);

    bool isBuildPossible(Point *p1, Point *p2, Point *p3, Triangle *trian);
    bool isCrossed(Point *p1, Point *p2, Triangle *trian);
    bool isCrossed(Point *p1, Point *p2, Point *p3, Point *p4);
    bool isInTriangle(Point *p, Triangle *trian);
    bool isRightSide(Point *p, std::vector<Triangle*> &triansLine);
    bool isLeftSide(Point *p, std::vector<Triangle*> &triansLine);

    float dotProduct(float x1, float y1, float x2, float y2);
    float dotProduct(Point *p1, Point *p2, Point *p3);
    float getArea(Point *p1, Point *p2, Point *p3);
};

#endif // TRIANGLEACTION_H
