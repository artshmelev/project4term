#include <algorithm>
#include <cmath>
#include <thread>

#include <QDebug>

#include "triangleaction.h"
#include "structures.h"

void TriangleAction::run(std::vector<Point*> &points,
                         std::vector<Triangle*> &triangles) {
    float minX, maxX, minY, maxY;
    minX = maxX = points[0]->getX();
    minY = maxY = points[0]->getY();
    for (int i = 1; i < points.size(); ++i) {
        float tmpX = points[i]->getX(), tmpY = points[i]->getY();
        if (tmpX > maxX) maxX = tmpX;
        if (tmpX < minX) minX = tmpX;
        if (tmpY > maxY) maxY = tmpY;
        if (tmpY < minY) minY = tmpY;
    }
    int numLines = (int)sqrt(0.13f * (maxX - minX) / (maxY - minY) *
                             points.size()) + 1;
    float lineWidth = (maxX - minX) / numLines + 0.5f;
    qDebug() << numLines;

    std::thread *threads = new std::thread[numLines - 1];
    std::vector<std::vector<Triangle*> > triansLine(numLines);
    std::vector<std::vector<Point*> > pointsLine(numLines);
    for (int i = 0; i < numLines; ++i) {
        for (int j = 0; j < points.size(); ++j) {
            if (points[j]->getX() >= minX + lineWidth * i &&
                points[j]->getX() < minX + lineWidth * (i + 1))
                pointsLine[i].push_back(points[j]);
        }
    }

    for (int i = 0; i < numLines - 1; ++i)
        threads[i] = std::thread(&TriangleAction::runThread, this,
                                 std::ref(pointsLine[i]),
                                 std::ref(triansLine[i]));
    runThread(pointsLine[numLines - 1], triansLine[numLines - 1]);
    for (int i = 0; i < numLines - 1; ++i)
        threads[i].join();

    for (int i = 0; i < numLines - 1; ++i)
        threads[i] = std::thread(&TriangleAction::doConvex, this,
                                 std::ref(pointsLine[i]),
                                 std::ref(triansLine[i]));
    doConvex(pointsLine[numLines - 1], triansLine[numLines - 1]);
    for (int i = 0; i < numLines - 1; ++i)
        threads[i].join();

    delete [] threads;

    for (int i = 0; i < triansLine.size(); ++i)
        for (int j = 0; j < triansLine[i].size(); ++j)
            triangles.push_back(triansLine[i][j]);

    for (int i = 1; i < triansLine.size(); ++i) {
        std::vector<Point*> border;
        for (int j = 0; j < pointsLine[i - 1].size(); ++j)
            if (isRightSide(pointsLine[i - 1][j], triansLine[i - 1]))
                border.push_back(pointsLine[i - 1][j]);
        for (int j = 0; j < pointsLine[i].size(); ++j)
            if (isLeftSide(pointsLine[i][j], triansLine[i]))
                border.push_back(pointsLine[i][j]);

        sort(border.begin(), border.end(), classCmp());
        Triangle *current = new Triangle(border[0], border[1], border[2]);
        triangles.push_back(current);
        for (int j = 3; j < border.size(); ++j) {
            std::vector<Point*> pnts = current->getPoints();
            bool b1 = isBuildPossible(border[j], pnts[2], pnts[0], current),
                 b2 = isBuildPossible(border[j], pnts[2], pnts[1], current),
                 ic1 = 0, ic2 = 0;
            for (int k = 0; k < triansLine[i - 1].size(); ++k) {
                if (isCrossed(pnts[0], border[j], triansLine[i - 1][k]))
                    ic1 = 1;
                if (isCrossed(pnts[1], border[j], triansLine[i - 1][k]))
                    ic2 = 1;
            }
            for (int k = 0; k < triansLine[i].size(); ++k) {
                if (isCrossed(pnts[0], border[j], triansLine[i][k]))
                    ic1 = 1;
                if (isCrossed(pnts[1], border[j], triansLine[i][k]))
                    ic2 = 1;
            }
            //qDebug() << b1 << " " << ic1 << " " << b2 << " " << ic2;
            if (b1 && !ic1)
                triangles.push_back(new Triangle(pnts[0], pnts[2], border[j]));
            else
                triangles.push_back(new Triangle(pnts[1], pnts[2], border[j]));
            current = triangles.back();
        }
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
    }
}

void TriangleAction::doConvex(std::vector<Point*> &points,
                              std::vector<Triangle*> &triangles) {
    std::vector<Point*> leftBorder, rightBorder;
    for (int i = 0; i < points.size(); ++i) {
        if (isRightSide(points[i], triangles))
            rightBorder.push_back(points[i]);
        if (isLeftSide(points[i], triangles))
            leftBorder.push_back(points[i]);
    }
    for (int i = 2; i < rightBorder.size(); ++i)
        if (dotProduct(rightBorder[i - 2], rightBorder[i - 1],
                       rightBorder[i]) < 0) {
            triangles.push_back(new Triangle(rightBorder[i - 2],
                                             rightBorder[i - 1],
                                             rightBorder[i]));
            rightBorder[i - 1] = rightBorder[i - 2];
        }

    for (int i = 2; i < leftBorder.size(); ++i)
        if (dotProduct(leftBorder[i - 2], leftBorder[i - 1],
                       leftBorder[i]) > 0) {
            triangles.push_back(new Triangle(leftBorder[i - 2],
                                             leftBorder[i - 1],
                                             leftBorder[i]));
            leftBorder[i - 1] = leftBorder[i - 2];
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
        /*qDebug() << i << " " << minX << " " << maxX << " " << minY << " " << maxY << " " <<
                " " << area1 << " " << area2 << " " << area3 << " " << area4;*/
        if (minX > maxX && minY > maxY &&
            area1 * area2 < 0 &&
            area3 * area4 < 0)
            return 1;
    }
    return 0;
}

bool TriangleAction::isCrossed(Point *p1, Point *p2, Point *p3, Point *p4) {
    float maxX = std::max(std::min(p1->getX(), p2->getX()),
                          std::min(p3->getX(), p4->getX()));
    float minX = std::min(std::max(p1->getX(), p2->getX()),
                          std::max(p3->getX(), p4->getX()));
    float maxY = std::max(std::min(p1->getY(), p2->getY()),
                          std::min(p3->getY(), p4->getY()));
    float minY = std::min(std::max(p1->getY(), p2->getY()),
                          std::max(p3->getY(), p4->getY()));
    float area1 = getArea(p1, p2, p3),
          area2 = getArea(p1, p2, p4),
          area3 = getArea(p3, p4, p1),
          area4 = getArea(p3, p4, p2);
    if (minX > maxX && minY > maxY &&
        area1 * area2 < 0 &&
        area3 * area4 < 0)
        return 1;
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
