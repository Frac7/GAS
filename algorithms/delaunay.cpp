#include "delaunay.h"
#include "utils/delaunay_checker.h"

namespace DelaunayTriangulation {

namespace Checker {

void fillDataStructures(const Triangulation& triangulation, cg3::Array2D<unsigned int>& triangles)
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

}

}

/* LegalizeEdge(p r ,p i p j ,T )
    1. (* The point being inserted is p r , and p i p j is the edge of T that
        may need to be flipped *)
    2. if p i p j is illegal
    3. then let p i p j p k be the triangle adjacent to p r p i p j
        along p i p j
    4. (* flip p i p j *) replace p i p j with p r p k
    5. LegalizeEdge(p r ,p i p k ,T )
    6. LegalizeEdge(p r ,p k p j ,T )
*/
void legalizeEdge(const cg3::Point2Dd& point, const int& pi, const int& pj, Triangle& triangle)
{
    //TODO: reuse o implement...?
    if(!DelaunayTriangulation::Checker::
            isPointLyingInCircle(triangle.getV1(), triangle.getV2(), triangle.getV3(), point, true))
    {
        /*
         * if the edge is illegal, find the adjacent triangle to T having points pi and pj
         * to find this triangle, check for neighbors (it can be 1 or they can be 2) in the DAG
         * the third can be found using the reference to the parent and finding its leaf neighbor
         *
         * during the search, it can be necessary to check if a point is inside a triangle
         * use the method isPointLyingInTriangle in utils2D
         *
         * using this approach, triangle.isInside(point) must be implemented
         * and it is necessary to have a method to find the adjacent triangle
         *
         * Triangulation data structure must store adjacency
         *
         * for replacing the edge, the triangle and the adjacent along this edge
         * must be modified in the triangulation
         * and two new triangles must be added to the DAG as children of the initial two
         */
    }
}

}
