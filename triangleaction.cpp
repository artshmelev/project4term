#include <algorithm>

#include "triangleaction.h"
#include "structures.h"

#include <QDebug>

void TriangleAction::run() {
    sort(points.begin(), points.end(), classCmp());
    Triangle *current = new Triangle(points[0], points[1], points[2]);
    triangles.push_back(current);
    for (int i = 3; i < points.size(); ++i) {
        std::vector<Point*> pnts = current->getPoints();
        bool b1 = isBuildPossible(points[i], pnts[2], pnts[0], current),
             b2 = isBuildPossible(points[i], pnts[2], pnts[1], current);
        if (b1)
            triangles.push_back(new Triangle(pnts[0], pnts[2], points[i]));
        else
            triangles.push_back(new Triangle(pnts[1], pnts[2], points[i]));
        current = triangles.back();
        pnts = current->getPoints();

        /*
        triangles.push_back(new Triangle(points[0], points[1], points[i]));
        current = triangles.back();
        */
    }
}

bool TriangleAction::isBuildPossible(Point *p1, Point *p2, Point *p3,
                                     Triangle *trian) {
    Triangle newTrian = Triangle(p1, p2, p3);
    std::vector<Point*> points = trian->getPoints();
    return (!isCrossed(p1, p3, trian) &&
            !isInTriangle(points[0], &newTrian) &&
            !isInTriangle(points[1], &newTrian) &&
            !isInTriangle(points[2], &newTrian));
}

bool TriangleAction::isCrossed(Point *p1, Point *p2, Triangle *trian) {
    std::vector<Edge> edges = trian->getEdges();
    for (int i = 0; i < edges.size(); ++i) {
        Point *edgeP1 = edges[i].getPoint1(),
              *edgeP2 = edges[i].getPoint2();
        int maxX = std::max(std::min(p1->getX(), p2->getX()),
                            std::min(edgeP1->getX(), edgeP2->getX()));
        int minX = std::min(std::max(p1->getX(), p2->getX()),
                            std::max(edgeP1->getX(), edgeP2->getX()));
        int maxY = std::max(std::min(p1->getY(), p2->getY()),
                            std::min(edgeP1->getY(), edgeP2->getY()));
        int minY = std::min(std::max(p1->getY(), p2->getY()),
                            std::max(edgeP1->getY(), edgeP2->getY()));
        if (minX >= maxX && minY >= maxY &&
            getArea(p1, p2, edgeP1) * getArea(p1, p2, edgeP2) <= 0 &&
            getArea(edgeP1, edgeP2, p1) * getArea(edgeP1, edgeP2, p2) <= 0)
            return 1;
    }
    return 0;
}

bool TriangleAction::isInTriangle(Point *p, Triangle *trian) {
    std::vector<Point*> points = trian->getPoints();
    int dp1 = dotProduct(p, points[0], points[1]),
        dp2 = dotProduct(p, points[1], points[2]),
        dp3 = dotProduct(p, points[2], points[0]);
    return (dp1 > 0 && dp2 > 0 && dp3 > 0 ||
            dp1 < 0 && dp2 < 0 && dp3 < 0);
}

int TriangleAction::dotProduct(int x1, int y1, int x2, int y2) {
    return (x1 * y2 - x2 * y1);
}

int TriangleAction::dotProduct(Point *p1, Point *p2, Point *p3) {
    return dotProduct(p2->getX() - p1->getX(), p2->getY() - p1->getY(),
                      p3->getX() - p2->getX(), p3->getY() - p2->getY());
}

int TriangleAction::getArea(Point *p1, Point *p2, Point *p3) {
    return dotProduct(p2->getX() - p1->getX(), p2->getY() - p1->getY(),
                      p3->getX() - p1->getX(), p3->getY() - p1->getY());
}

std::vector<Triangle*> TriangleAction::getTriangles() {
    return triangles;
}
