#include "delaunay.h"

namespace DelaunayTriangulation {

namespace Checker {

void fillDataStructures(const Triangulation& triangulation, std::vector<cg3::Point2Dd>& points, cg3::Array2D<unsigned int>& triangles)
{
    std::vector<Triangle> triangleVector = triangulation.getTriangles();
    unsigned int vectorSize = triangleVector.size();

    triangles.resize(vectorSize, dimension);

    for(unsigned int i = 0; i < vectorSize; i++)
    {
        unsigned int j = 0;

        triangles(i, j) = triangleVector[i].getV1();
        triangles(i, j + 1) = triangleVector[i].getV2();
        triangles(i, j + 2) = triangleVector[i].getV3();
    }

    points = triangulation.getPoints();

}

}

}
