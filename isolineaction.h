#ifndef ISOLINEACTION_H
#define ISOLINEACTION_H

#include "structures.h"

class IsolineAction {
public:
    void run(std::vector<Point*> &points, std::vector<Triangle*> &triangles);

};

#endif // ISOLINEACTION_H
