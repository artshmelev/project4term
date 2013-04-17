#ifndef MAINACTION_H
#define MAINACTION_H

#include <vector>

#include "structures.h"
#include "triangleaction.h"
#include "isolineaction.h"

class MainAction {
public:
    MainAction(std::vector<Point*> &pnts);
    ~MainAction();

    void run();

    std::vector<Triangle*> getTriangles();
    std::vector<Point*> getPoints();
    std::vector<Edge*> getIsolineEdges();

private:
    std::vector<Point*> points, isolinePoints;
    std::vector<Triangle*> triangles;
    std::vector<Edge*> isolineEdges;
    TriangleAction *triangleAction;
    IsolineAction *isolineAction;
};

inline std::vector<Triangle*> MainAction::getTriangles() {
    return triangles;
}

inline std::vector<Point*> MainAction::getPoints() {
    return points;
}

inline std::vector<Edge*> MainAction::getIsolineEdges() {
    return isolineEdges;
}

#endif // MAINACTION_H
