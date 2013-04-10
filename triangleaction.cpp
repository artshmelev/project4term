#include <algorithm>

#include "triangleaction.h"
#include "structures.h"

void TriangleAction::run() {
    sort(points.begin(), points.end(), classCmp());
    Triangle *current = new Triangle(points[0], points[1], points[2]);
    triangles.push_back(current);
    for (int i = 3; i < points.size(); ++i) {
        triangles.push_back(new Triangle(points[0], points[1], points[i]));
        current = triangles.back();
    }
}

std::vector<Triangle*> TriangleAction::getTriangles() {
    return triangles;
}
