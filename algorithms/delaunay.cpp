#include "delaunay.h"

#include <utils/delaunay_checker.h>
#include <cg3lib/cg3/core/cg3/geometry/2d/utils2d.h>

namespace DelaunayTriangulation {

namespace Checker {

void fillDataStructures(const Triangulation& triangulation, const DAG& dag, std::vector<cg3::Point2Dd>& points, cg3::Array2D<unsigned int>& triangles)
{
    const std::vector<Triangle>& triangleVector = triangulation.getTriangles();
    const std::vector<Node>& nodeVector = dag.getNodeList();

    const unsigned int& vectorSize = triangleVector.size();

    if(vectorSize > 1)
    {
        triangles.resize(vectorSize, dimension);

        unsigned int j = 0;

        //ignore bounding triangle
        for(unsigned int i = 1; i < vectorSize; i++)
        {
            //ignore deleted triangles
            if(nodeVector[i].getData() != deleted)
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
    const std::vector<Triangle>& triangles = triangulation.getTriangles();
    const std::vector<Node>& nodes = dag.getNodeList();

    const unsigned int totalTrianglesNumber = triangles.size();
    const unsigned int totalNodes = nodes.size();

    //find the triangle that contains this point using the DAG
    const unsigned int parentNodeIndex = dag.searchInNodes(0, totalNodes, point, triangles);
    const unsigned int triangleIndex = nodes[parentNodeIndex].getData();
    //these numbers must be not equal to -1

    //get vertices of containing triangle
    const cg3::Point2Dd& v1 = triangles[triangleIndex].getV1();
    const cg3::Point2Dd& v2 = triangles[triangleIndex].getV2();
    const cg3::Point2Dd& v3 = triangles[triangleIndex].getV3();

    unsigned int currentTriangle = totalTrianglesNumber;
    //TODO: replace magic numbers
    int triangle1 = -1;
    int triangle2 = -1;
    int triangle3 = -1;

    //create 3 new triangles and 3 new nodes

    if(!cg3::areCollinear(v1, v2, point))
    {
        //add triangle to the triangulation
        triangulation.addTriangle(Triangle(v1, v2, point));
        //add node to the dag
        dag.addNode(Node(currentTriangle), parentNodeIndex);

        triangle1 = currentTriangle;
        //increment index
        currentTriangle++;
    }

    if(!cg3::areCollinear(v2, v3, point))
    {
        //add triangle to the triangulation
        triangulation.addTriangle(Triangle(point, v2, v3)); //totalTrianglesNumber + 1
        //add node to the dag
        dag.addNode(Node(currentTriangle), parentNodeIndex);

        triangle2 = currentTriangle;
        //increment index
        currentTriangle++;
    }

    if(!cg3::areCollinear(v3, v1, point))
    {
        //add triangle to the triangulation
        triangulation.addTriangle(Triangle(v1, point, v3)); //totalTrianglesNumber + 2
        //add node to the dag
        dag.addNode(Node(currentTriangle), parentNodeIndex);

        triangle3 = currentTriangle;
        //increment index
        currentTriangle++;
    }

    const std::array<int, maxAdjacentTriangles>& oldTriangleAdjacencies = triangulation.getAdjacenciesFromTriangle(triangleIndex);

    //update adjacencies
    if(triangle1 != -1) //TODO: replace magic numbers
    {
        triangulation.addAdjacenciesForNewTriangle(triangle1,
                                               oldTriangleAdjacencies[0], triangle2, triangle3, triangleIndex);
    }
    if(triangle2 != - 1)
    {
        triangulation.addAdjacenciesForNewTriangle(triangle2,
                                               triangle1, oldTriangleAdjacencies[1], triangle3, triangleIndex);
    }
    if(triangle3 != -1)
    {
        triangulation.addAdjacenciesForNewTriangle(triangle3,
                                               triangle1, triangle2, oldTriangleAdjacencies[2], triangleIndex);
    }
}

}
