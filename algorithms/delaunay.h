#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <iostream>

#include <cg3/data_structures/arrays/array2d.h>
#include <cg3/geometry/2d/point2d.h>
#include <data_structures/triangulation.h>

/* PERSONAL: is the use of these namespaces correct? DelaunayTriangluation::Checker*/

namespace DelaunayTriangulation {

const unsigned int dimension = 3;

namespace Checker {

void fillDataStructures(const Triangulation& triangulation, std::vector<cg3::Point2Dd>& points, cg3::Array2D<unsigned int>& triangles);
}

}

#endif // DELAUNAY_H
