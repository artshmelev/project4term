#include <algorithm>
#include <thread>
#include <functional>
#include <QDebug>

#include "isolineaction.h"

void IsolineAction::run(std::vector<Point*> &isolinePoints,
                        std::vector<Edge*> &isolineEdges,
                        std::vector<Triangle*> &triangles) {
    int numThreads = 11;
    std::thread *threads = new std::thread[numThreads - 1];
    float isolineValue = 0.0f;
    for (int i = 0; i < numThreads - 1; ++i) {
        threads[i] = std::thread(&IsolineAction::runThread, this,
                                 std::ref(isolinePoints),
                                 std::ref(isolineEdges),
                                 std::ref(triangles), isolineValue);
        isolineValue += 3.0f;
    }

    runThread(isolinePoints, isolineEdges, triangles, isolineValue);

    for (int i = 0; i < numThreads - 1; ++i)
        threads[i].join();

    delete [] threads;

}

void IsolineAction::runThread(std::vector<Point*> &isolinePoints,
                              std::vector<Edge*> &isolineEdges,
                              std::vector<Triangle*> &triangles,
                              float isolineValue) {
    float h = isolineValue;
    for (int i = 0; i < triangles.size(); ++i) {
        std::vector<Point*> pnts = triangles[i]->getPoints();
        float t1 = pnts[0]->getT(),
              t2 = pnts[1]->getT(),
              t3 = pnts[2]->getT();
        Point *delimPoint1, *delimPoint2;

        if (std::min(t1, std::min(t2, t3)) < h &&
            h < std::max(t1, std::max(t2, t3))) {
            //qDebug() << t1 << " " << t2 << " " << t3;
            if (t1 < h && h < t2) {
                delimPoint1 = edgeDivide(pnts[0], pnts[1], h);
                if (t2 < h && h < t3)
                    delimPoint2 = edgeDivide(pnts[1], pnts[2], h);
                else if (t3 < h && h < t2)
                    delimPoint2 = edgeDivide(pnts[2], pnts[1], h);
                else if (t1 < h && h < t3)
                    delimPoint2 = edgeDivide(pnts[0], pnts[2], h);
                else if (t3 < h && h < t1)
                    delimPoint2 = edgeDivide(pnts[2], pnts[0], h);
            }
            else if (t2 < h && h < t1) {
                delimPoint1 = edgeDivide(pnts[1], pnts[0], h);
                if (t2 < h && h < t3)
                    delimPoint2 = edgeDivide(pnts[1], pnts[2], h);
                else if (t3 < h && h < t2)
                    delimPoint2 = edgeDivide(pnts[2], pnts[1], h);
                else if (t1 < h && h < t3)
                    delimPoint2 = edgeDivide(pnts[0], pnts[2], h);
                else if (t3 < h && h < t1)
                    delimPoint2 = edgeDivide(pnts[2], pnts[0], h);
            }
            else  if (t2 < h && h < t3) {
                delimPoint1 = edgeDivide(pnts[1], pnts[2], h);
                delimPoint2 = edgeDivide(pnts[0], pnts[2], h);
            }
            else if (t3 < h && h < t2) {
                delimPoint1 = edgeDivide(pnts[2], pnts[0], h);
                delimPoint2 = edgeDivide(pnts[2], pnts[1], h);
            }

            threadMutex.lock();
            isolinePoints.push_back(delimPoint1);
            isolinePoints.push_back(delimPoint2);
            isolineEdges.push_back(new Edge(delimPoint1, delimPoint2));
            threadMutex.unlock();
        }
    }
}

Point* IsolineAction::edgeDivide(Point *p1, Point *p2, float delim) {
    float len1 = delim - p1->getT(),
          len = p2->getT() - p1->getT(),
          x1 = p1->getX(), y1 = p1->getY(),
          x2 = p2->getX(), y2 = p2->getY();
    float xNew = x1 + (x2 - x1) / len * len1,
          yNew = y1 + (y2 - y1) / len * len1;

    return new Point(xNew, yNew, delim);
}
