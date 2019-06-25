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
                  const unsigned int triangleIndex, const unsigned int adjacentIndex,
                  const cg3::Point2Dd& p1, const cg3::Point2Dd& p2, const cg3::Point2Dd& p3, const cg3::Point2Dd& pk,
                  const unsigned int edge, const unsigned int adjEdge,
                  std::array<int, dimension>& triangleAdj, std::array<int, dimension>& adjTriangleAdj);

void incrementalTriangulation(Triangulation& triangulation, DAG &dag, const cg3::Point2Dd& point);

void addElementToTriangulation(Triangulation& triangulation, DAG& dag,
                unsigned int index, unsigned int parentIndex,
                const cg3::Point2Dd& v1, const cg3::Point2Dd& v2, const cg3::Point2Dd& v3,
                int adjacency0, int adjacency1, int adjacency2);

void testEdge(Triangulation& triangulation, DAG& dag,
              unsigned int triangle, unsigned int adjacent, unsigned int edge,
              const cg3::Point2Dd& v1, const cg3::Point2Dd& v2, const cg3::Point2Dd& v3);

void addElementAfterFlip(Triangulation& triangulation, DAG& dag,
                         unsigned int index, unsigned int firstParentIndex, unsigned int secondParentIndex,
                         const cg3::Point2Dd& v1, const cg3::Point2Dd& v2, const cg3::Point2Dd& v3,
                         int adjacency0, int adjacency1, int adjacency2);

}

#endif // DELAUNAY_H
