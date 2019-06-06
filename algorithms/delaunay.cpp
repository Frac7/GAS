#include "delaunay.h"
#include "utils/delaunay_checker.h"

namespace DelaunayTriangulation {

namespace Checker {

void fillDataStructures(const Triangulation& triangulation, std::vector<cg3::Point2Dd>& points, cg3::Array2D<unsigned int>& triangles)
{
    std::vector<Triangle> triangleVector = triangulation.getTriangles();
    unsigned int vectorSize = triangleVector.size();

    triangles.resize(vectorSize, dimension);

    for(unsigned int i = 0; i < vectorSize; i+=3)
    {
        points[i] = triangleVector[i].getV1();
        points[i + 1] = triangleVector[i].getV2();
        points[i + 2] = triangleVector[i].getV3();

        triangles(i, 0) = i;
        triangles(i, 1) = i + 1;
        triangles(i, 2) = i + 2;
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
void legalizeEdge(Triangulation& triangulation, const cg3::Point2Dd& point, const unsigned int& pipj, const unsigned int& triangle)
{
    Triangle t = triangulation.getTriangles()[triangle];
    //TODO: reuse o implement...?
    if(!DelaunayTriangulation::Checker::
            isPointLyingInCircle(t.getV1(), t.getV2(), t.getV3(), point, true))
    {
        /*
         * if the edge is illegal, find the adjacent triangle to T adjacent in pipj
         *
         * for replacing the edge, the triangle and the adjacent along this edge
         * must be modified in the triangulation
         * and two new triangles must be added to the DAG as children of the initial two
         */

        //pick the adjacent triangle
        Triangle adjacent = triangulation.getTriangles()[triangulation.getAdjacencies(triangle)[pipj]];

    }
}

}
