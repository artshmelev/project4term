#include <algorithm>

#include "triangleaction.h"
#include "structures.h"

void TriangleAction::run() {
    sort(points.begin(), points.end(), classCmp());
    Triangle *current = new Triangle(points[0], points[1], points[2]);
    triangles.push_back(current);
    for (int i = 3; i < points.size(); ++i) {
        std::vector<Point*> pnts = current->getPoints();
        bool b1 = isBuildPossible(points[i], pnts[2], pnts[0], current),
             b2 = isBuildPossible(points[i], pnts[2], pnts[1], current);
        if (b1)
            triangles.push_back(new Triangle(points[i], pnts[2], pnts[0]));
        else
            triangles.push_back(new Triangle(points[i], pnts[2], pnts[1]));
        current = triangles.back();
        /*
        triangles.push_back(new Triangle(points[0], points[1], points[i]));
        current = triangles.back();
        */
    }
}

bool TriangleAction::isBuildPossible(Point *p1, Point *p2, Point *p3,
                                     Triangle *trian) {
    return (!isCrossed(p1, p3, trian));
}

bool TriangleAction::isCrossed(Point *p1, Point *p2, Triangle *trian) {
    std::vector<Edge> edges = trian->getEdges();
    for (int i = 0; i < edges.size(); ++i) {
        int maxX = std::max(std::min(p1->getX(), p2->getX()),
                            std::min(edges[i].getPoint1()->getX(),
                                     edges[i].getPoint2()->getX()));
        int minX = std::min(std::max(p1->getX(), p2->getX()),
                            std::max(edges[i].getPoint1()->getX(),
                                     edges[i].getPoint2()->getX()));
        int maxY = std::max(std::min(p1->getY(), p2->getY()),
                            std::min(edges[i].getPoint1()->getY(),
                                     edges[i].getPoint2()->getY()));
        int minY = std::min(std::max(p1->getY(), p2->getY()),
                            std::max(edges[i].getPoint1()->getY(),
                                     edges[i].getPoint2()->getY()));
        if ((maxX - minX) >= 0 && (maxY - minY) >= 0)
            return 1;
    }
    return 0;
}

std::vector<Triangle*> TriangleAction::getTriangles() {
    return triangles;
}
