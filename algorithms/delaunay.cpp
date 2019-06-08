#include "delaunay.h"
#include "utils/delaunay_checker.h"

namespace DelaunayTriangulation {

namespace Checker {

void fillDataStructures(const Triangulation& triangulation, std::vector<cg3::Point2Dd>& points, cg3::Array2D<unsigned int>& triangles)
{
    std::vector<Triangle> triangleVector = triangulation.getTriangles();
    unsigned int vectorSize = triangleVector.size();

    triangles.resize(vectorSize, dimension);

    unsigned int j = 0;

    //ignore bounding triangle
    for(unsigned int i = 1; i < vectorSize; i++)
    {
        points.push_back(triangleVector[i].getV1());
        points.push_back(triangleVector[i].getV2());
        points.push_back(triangleVector[i].getV3());

        triangles(i, 0) = j;
        triangles(i, 1) = j + 1;
        triangles(i, 2) = j + 2;

        j += 3;
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
        Triangle adjacent = triangulation.getTriangles()[triangulation.getAdjacenciesFromTriangle(triangle)[pipj]];

    }
}

void incrementalTriangulation(Triangulation& triangulation, DAG& dag, const cg3::Point2Dd& point)
{
    //TODO: manage the case where the point lies on the edge

    const std::vector<Triangle>& triangles = triangulation.getTriangles();
    const std::vector<Node>& nodes = dag.getNodeList();

    const unsigned int totalTrianglesNumber = triangles.size();

    //find the triangle that contains this point using the DAG
    unsigned int parentNodeIndex = dag.findNodeContainingPoint(point, triangles);
    int triangleIndex = nodes[parentNodeIndex].getData();

    //get vertices of triangle
    const cg3::Point2Dd& v1 = triangles[triangleIndex].getV1();
    const cg3::Point2Dd& v2 = triangles[triangleIndex].getV2();
    const cg3::Point2Dd& v3 = triangles[triangleIndex].getV3();

    //create 3 new triangles and 3 new nodes

    const unsigned int& indexOfTriangle1 = totalTrianglesNumber;
    //add triangle to the triangulation
    const Triangle triangle1(v1, v2, point); //totalTrianglesNumber
    triangulation.addTriangle(triangle1);
    //add node to the dag
    const Node node1(indexOfTriangle1);
    dag.addNode(node1, parentNodeIndex);

    const unsigned int& indexOfTriangle2 = totalTrianglesNumber + 1;
    //add triangle to the triangulation
    const Triangle triangle2(point, v2, v3); //totalTrianglesNumber + 1
    triangulation.addTriangle(triangle2);
    //add node to the dag
    const Node node2(indexOfTriangle2);
    dag.addNode(node2, parentNodeIndex);

    const unsigned int& indexOfTriangle3 = totalTrianglesNumber + 2;
    //add triangle to the triangulation
    const Triangle triangle3(v1, point, v3); //totalTrianglesNumber + 2
    triangulation.addTriangle(triangle3);
    //add node to the dag
    const Node node3(indexOfTriangle3);
    dag.addNode(node3, parentNodeIndex);

    const std::array<int, maxAdjacentTriangles>& oldTriangleAdjacencies = triangulation.getAdjacenciesFromTriangle(triangleIndex);

    //update adjacencies
    triangulation.addAdjacenciesForNewTriangle(indexOfTriangle1, oldTriangleAdjacencies[0], totalTrianglesNumber + 1, totalTrianglesNumber + 2, triangleIndex);
    triangulation.addAdjacenciesForNewTriangle(indexOfTriangle2, totalTrianglesNumber, oldTriangleAdjacencies[1], totalTrianglesNumber + 2, triangleIndex);
    triangulation.addAdjacenciesForNewTriangle(indexOfTriangle3, totalTrianglesNumber, totalTrianglesNumber + 1, oldTriangleAdjacencies[2], triangleIndex);
}

}
