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
    void runThread(std::vector<Point*> &isolinePoints,
                   std::vector<Edge*> &isolineEdges,
                   std::vector<Triangle*> &triangles,
                   float isolineValue);
    Point* edgeDivide(Point *p1, Point *p2, float delim);
    std::mutex threadMutex;
};

#endif // ISOLINEACTION_H
