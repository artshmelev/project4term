#ifndef TRIANGLEACTION_H
#define TRIANGLEACTION_H

#include <vector>
#include "structures.h"

//TODO: singleton class
class TriangleAction {
public:
    TriangleAction(std::vector<Point*>& pnts) : points(pnts) {}

    void run();

    bool isBuildPossible(Point *p1, Point *p2, Point *p3, Triangle *trian);
    bool isCrossed(Point *p1, Point *p2, Triangle *trian);
    bool isInTriangle(Point *p, Triangle *trian);
    int dotProduct(int x1, int y1, int x2, int y2);
    int dotProduct(Point *p1, Point *p2, Point *p3);
    int getArea(Point *p1, Point *p2, Point *p3);
    std::vector<Triangle*> getTriangles();
private:
    std::vector<Point*> points;
    //std::vector<Edge*> edges;
    std::vector<Triangle*> triangles;
};

#endif // TRIANGLEACTION_H
