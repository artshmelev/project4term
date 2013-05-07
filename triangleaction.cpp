#include <algorithm>
#include <thread>

#include<QDebug>

#include "triangleaction.h"
#include "structures.h"

void TriangleAction::run(std::vector<Point*> &points,
                         std::vector<Triangle*> &triangles) {
    int numThreads = 1;
    std::thread *threads = new std::thread[numThreads];
    std::vector<std::vector<Triangle*> > triansLine(numThreads + 1);
    std::vector<std::vector<Point*> > pointsLine(numThreads + 1);
    for (int i = 0; i < points.size(); ++i) {
        if (points[i]->getX() < 500)
            pointsLine[0].push_back(points[i]);
        else
            pointsLine[1].push_back(points[i]);
    }
    for (int i = 0; i < numThreads; ++i)
        threads[i] = std::thread(&TriangleAction::runThread, this,
                                 std::ref(pointsLine[i]),
                                 std::ref(triansLine[i]));

    runThread(pointsLine[1], triansLine[1]);

    for (int i = 0; i < numThreads; ++i)
        threads[i].join();

    delete [] threads;

    for (int i = 0; i < triansLine.size(); ++i)
        for (int j = 0; j < triansLine[i].size(); ++j)
            triangles.push_back(triansLine[i][j]);

    for (int i = 1; i < triansLine.size(); ++i) {
        std::vector<Point*> rightSide;
        for (int j = 0; j < pointsLine[i - 1].size(); ++j)
            if (isRightSide(pointsLine[i - 1][j], triansLine[i - 1]))
                rightSide.push_back(pointsLine[i - 1][j]);
        for (int j = 0; j < pointsLine[i].size(); ++j)
            if (isLeftSide(pointsLine[i][j], triansLine[i]))
                rightSide.push_back(pointsLine[i][j]);
        runThread(rightSide, triangles);
    }
}

void TriangleAction::runThread(std::vector<Point*> &points,
                               std::vector<Triangle*> &triangles) {
    sort(points.begin(), points.end(), classCmp());
    Triangle *current = new Triangle(points[0], points[1], points[2]);
    triangles.push_back(current);
    for (int i = 3; i < points.size(); ++i) {
        std::vector<Point*> pnts = current->getPoints();
        bool b1 = isBuildPossible(points[i], pnts[2], pnts[0], current),
             b2 = isBuildPossible(points[i], pnts[2], pnts[1], current);
        //qDebug() << b1 << " " << b2;
        if (b1)
            triangles.push_back(new Triangle(pnts[0], pnts[2], points[i]));
        else
            triangles.push_back(new Triangle(pnts[1], pnts[2], points[i]));
        current = triangles.back();
        pnts = current->getPoints();
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
        float maxX = std::max(std::min(p1->getX(), p2->getX()),
                              std::min(edgeP1->getX(), edgeP2->getX()));
        float minX = std::min(std::max(p1->getX(), p2->getX()),
                              std::max(edgeP1->getX(), edgeP2->getX()));
        float maxY = std::max(std::min(p1->getY(), p2->getY()),
                              std::min(edgeP1->getY(), edgeP2->getY()));
        float minY = std::min(std::max(p1->getY(), p2->getY()),
                              std::max(edgeP1->getY(), edgeP2->getY()));
        float area1 = getArea(p1, p2, edgeP1),
              area2 = getArea(p1, p2, edgeP2),
              area3 = getArea(edgeP1, edgeP2, p1),
              area4 = getArea(edgeP1, edgeP2, p2);
        if (minX > maxX && minY > maxY &&
            area1 * area2 <= 0 &&
            area3 * area4 <= 0)
            return 1;
    }
    return 0;
}

bool TriangleAction::isInTriangle(Point *p, Triangle *trian) {
    std::vector<Point*> points = trian->getPoints();
    float dp1 = dotProduct(p, points[0], points[1]),
          dp2 = dotProduct(p, points[1], points[2]),
          dp3 = dotProduct(p, points[2], points[0]);
    return (dp1 > 0 && dp2 > 0 && dp3 > 0 ||
            dp1 < 0 && dp2 < 0 && dp3 < 0);
}

bool TriangleAction::isRightSide(Point *p, std::vector<Triangle*> &triansLine) {
    Point p1(p->getX() + 1.5f, p->getY(), p->getT());
    for (int i = 0; i < triansLine.size(); ++i)
        if (isInTriangle(&p1, triansLine[i]))
            return 0;
    return 1;
}

bool TriangleAction::isLeftSide(Point *p, std::vector<Triangle*> &triansLine) {
    Point p1(p->getX() - 1.5f, p->getY(), p->getT());
    for (int i = 0; i < triansLine.size(); ++i)
        if (isInTriangle(&p1, triansLine[i]))
            return 0;
    return 1;
}

float TriangleAction::dotProduct(float x1, float y1, float x2, float y2) {
    return (x1 * y2 - x2 * y1);
}

float TriangleAction::dotProduct(Point *p1, Point *p2, Point *p3) {
    return dotProduct(p2->getX() - p1->getX(), p2->getY() - p1->getY(),
                      p3->getX() - p2->getX(), p3->getY() - p2->getY());
}

float TriangleAction::getArea(Point *p1, Point *p2, Point *p3) {
    return dotProduct(p2->getX() - p1->getX(), p2->getY() - p1->getY(),
                      p3->getX() - p1->getX(), p3->getY() - p1->getY());
}
