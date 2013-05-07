#include "structures.h"

#include <algorithm>

std::vector<Point*> Triangle::getPoints() {
    std::vector<Point*> pnts;
    pnts.push_back(point1);
    pnts.push_back(point2);
    pnts.push_back(point3);
    return pnts;
}

std::vector<Edge> Triangle::getEdges() {
    std::vector<Edge> edges;
    edges.push_back(Edge(point1, point2));
    edges.push_back(Edge(point2, point3));
    edges.push_back(Edge(point3, point1));
    return edges;
}
