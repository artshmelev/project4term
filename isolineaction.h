#ifndef ISOLINEACTION_H
#define ISOLINEACTION_H

#include <mutex>

#include "structures.h"

class IsolineAction {
public:
    void run(std::vector<Point*> &isolinePoints,
             std::vector<Edge*> &isolineEdges,
             std::vector<Triangle*> &triangles);

private:
    Point* edgeDivide(Point *p1, Point *p2, float delim);
    void runThread(std::vector<Point*> &isolinePoints,
                   std::vector<Edge*> &isolineEdges,
                   std::vector<Triangle*> &triangles,
                   float isolineValue);
    std::mutex threadMutex;
};

#endif // ISOLINEACTION_H
