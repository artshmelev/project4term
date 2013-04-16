#include <algorithm>
#include <QDebug>

#include "isolineaction.h"

void IsolineAction::run(std::vector<Point *> &points,
                        std::vector<Triangle *> &triangles) {
    float h = 15;
    for (int i = 0; i < triangles.size(); ++i) {
        std::vector<Point*> pnts = triangles[i]->getPoints();
        float t1 = pnts[0]->getT(),
              t2 = pnts[1]->getT(),
              t3 = pnts[2]->getT();
        if (std::min(t1, std::min(t2, t3)) < h &&
            h < std::max(t1, std::max(t2, t3)))
            qDebug() << t1 << " " << t2 << " " << t3;
    }
}
