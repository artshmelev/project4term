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

private:
    std::vector<Point*> points;
    //std::vector<Edge*> edges;
    std::vector<Triangle*> triangles;
    TriangleAction *triangleAction;
    IsolineAction *isolineAction;
};

inline std::vector<Triangle*> MainAction::getTriangles() {
    return triangles;
}

inline std::vector<Point*> MainAction::getPoints() {
    return points;
}

#endif // MAINACTION_H
