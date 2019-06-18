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

void fillDataStructures(Triangulation& triangulation, DAG& dag, std::vector<cg3::Point2Dd>& points, cg3::Array2D<unsigned int>& triangles);

}

void legalizeEdge(Triangulation& triangulation, DAG& dag,
                  const unsigned int& triangleIndex, const unsigned int& adjacentIndex,
                  const cg3::Point2Dd& pi, const cg3::Point2Dd& pj, const cg3::Point2Dd& pr, const cg3::Point2Dd& pk,
                  const unsigned int& edge, std::array<int, dimension>& triangleAdj, std::array<int, dimension>& adjTriangleAdj);

void incrementalTriangulation(Triangulation& triangulation, DAG &dag, const cg3::Point2Dd& point);

}

#endif // DELAUNAY_H
