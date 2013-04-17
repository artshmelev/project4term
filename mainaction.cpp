#include "mainaction.h"
#include "structures.h"

MainAction::MainAction(std::vector<Point*> &pnts) {
    points = pnts;
    triangleAction = new TriangleAction();
    isolineAction = new IsolineAction();
}

MainAction::~MainAction() {
    delete triangleAction;
    delete isolineAction;

    for (int i = 0; i < points.size(); ++i)
        delete points[i];
    for (int i = 0; i < triangles.size(); ++i)
        delete triangles[i];
    for (int i = 0; i < isolineEdges.size(); ++i)
        delete isolineEdges[i];
    for (int i = 0; i < isolinePoints.size(); ++i)
        delete isolinePoints[i];
}

void MainAction::run() {
    triangleAction->run(points, triangles);
    isolineAction->run(isolinePoints, isolineEdges, triangles);
}
