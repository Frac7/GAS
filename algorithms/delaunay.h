#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <cg3/data_structures/arrays/array2d.h>
#include <cg3/geometry/2d/point2d.h>

#include "data_structures/dag.h"
#include "data_structures/triangulation.h"

//is the use of these namespaces correct? DelaunayTriangluation::Checker

namespace DelaunayTriangulation {

const unsigned int dimension = 3;

namespace Checker {

void fillDataStructures(const Triangulation& triangulation, const DAG& dag, std::vector<cg3::Point2Dd>& points, cg3::Array2D<unsigned int>& triangles);

}

void legalizeEdge(const cg3::Point2Dd& point, const int& pi, const int& pj, Triangle& triangle);

void incrementalTriangulation(Triangulation& triangulation, DAG &dag, const cg3::Point2Dd& point);

void pointInside(const cg3::Point2Dd& v1, const cg3::Point2Dd& v2, const cg3::Point2Dd& v3, const cg3::Point2Dd& point,
                 const unsigned int totalTrianglesNumber, const unsigned int& parentNodeIndex, const unsigned int& triangleIndex,
                 Triangulation& triangulation, DAG& dag);

void pointInSegment(const cg3::Point2Dd& v1, const cg3::Point2Dd& v2, const cg3::Point2Dd& v3, const cg3::Point2Dd& point,
                 const unsigned int totalTrianglesNumber, const unsigned int& parentNodeIndex, const unsigned int& triangleIndex, const unsigned int& adjacency,
                 Triangulation& triangulation, DAG& dag, const std::vector<Triangle>& triangles);

}

#endif // DELAUNAY_H
