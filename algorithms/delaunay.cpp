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

    //if one of these flag is true, we have to create 4 new triangles instead of 3
    const bool colinearV1V2 = cg3::areCollinear(v1, v2, point);
    const bool colinearV2V3 = cg3::areCollinear(v2, v3, point);
    const bool colinearV3V1 = cg3::areCollinear(v3, v1, point);

    //point is inside a triangle, so we have to create 3 new triangles
    if(!colinearV1V2 && !colinearV2V3 && !colinearV3V1)
    {
        pointInside(v1, v2, v3, point, totalTrianglesNumber, parentNodeIndex, triangleIndex, triangulation, dag);
    }
    else
    {
        /*unsigned int edge = -1; //TODO: replace magic numbers
        if(colinearV1V2)
        {
            edge = 0;
        }
        else if(colinearV2V3)
        {
            edge = 1;
        }
        else
        {
            edge = 2;
        }*/

        const unsigned int edge = colinearV1V2? 0 : (colinearV2V3? 1 : 2);

        pointInSegment(v1, v2, v3, point, totalTrianglesNumber, parentNodeIndex, triangleIndex, edge, triangulation, dag, triangles);
    }


}

//TODO: avoid code repetition in the next 2 functions

void pointInside(const cg3::Point2Dd& v1, const cg3::Point2Dd& v2, const cg3::Point2Dd& v3, const cg3::Point2Dd& point,
                 const unsigned int& totalTrianglesNumber, const unsigned int& parentNodeIndex, const unsigned int& triangleIndex,
                 Triangulation& triangulation, DAG& dag)
{

    const std::array<int, maxAdjacentTriangles>& oldTriangleAdjacencies = triangulation.getAdjacenciesFromTriangle(triangleIndex);

    //create 3 new triangles and 3 new nodes

    //add triangle to the triangulation
    triangulation.addTriangle(Triangle(v1, v2, point));
    //add node to the dag
    dag.addNode(Node(totalTrianglesNumber), parentNodeIndex);
    //update adjacencies
    triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber,
        oldTriangleAdjacencies[0], totalTrianglesNumber + 1, totalTrianglesNumber + 2, triangleIndex);

    //add triangle to the triangulation
    triangulation.addTriangle(Triangle(point, v2, v3)); //totalTrianglesNumber + 1
    //add node to the dag
    dag.addNode(Node(totalTrianglesNumber + 1), parentNodeIndex);
    //update adjacencies
    triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 1,
        totalTrianglesNumber, oldTriangleAdjacencies[1], totalTrianglesNumber + 2, totalTrianglesNumber);

    //add triangle to the triangulation
    triangulation.addTriangle(Triangle(v1, point, v3)); //totalTrianglesNumber + 2
    //add node to the dag
    dag.addNode(Node(totalTrianglesNumber + 2), parentNodeIndex);
    //update adjacencies
    triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 2,
        totalTrianglesNumber, totalTrianglesNumber + 1, oldTriangleAdjacencies[2], triangleIndex);

    //TODO: legalize edge
}

void pointInSegment(const cg3::Point2Dd& v1, const cg3::Point2Dd& v2, const cg3::Point2Dd& v3, const cg3::Point2Dd& point,
                 const unsigned int& totalTrianglesNumber, const unsigned int& parentNodeIndex, const unsigned int& triangleIndex, const unsigned int& adjacency,
                 Triangulation& triangulation, DAG& dag, const std::vector<Triangle>& triangles)
{

    const std::array<int, maxAdjacentTriangles>& oldTriangleAdjacencies = triangulation.getAdjacenciesFromTriangle(triangleIndex);
    //get the adjacent triangle
    const unsigned int adjacentTriangleIndex = oldTriangleAdjacencies[adjacency];
    Triangle adjacent = triangles[adjacentTriangleIndex];

    //get old adjacencies for neighbor triangle
    const std::array<int, maxAdjacentTriangles>& oldNeighborAdjacencies = triangulation.getAdjacenciesFromTriangle(adjacentTriangleIndex);

    const cg3::Point2Dd& adjacentV1 = adjacent.getV1();
    const cg3::Point2Dd& adjacentV2 = adjacent.getV2();
    const cg3::Point2Dd& adjacentV3 = adjacent.getV3();

    //based on the adjacency where the point lies (segment), create two new triangles
    switch(adjacency)
    {
        case 0: //v3 -> adjacency in v1v2

            //first triangle

            //add triangle to the triangulation
            triangulation.addTriangle(Triangle(v1, point, v3));
            //add node to the dag
            dag.addNode(Node(totalTrianglesNumber), parentNodeIndex);
            //update adjacencies
            triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber,
                totalTrianglesNumber + 2, totalTrianglesNumber + 1, oldTriangleAdjacencies[2], triangleIndex);

            //add triangle to the triangulation
            triangulation.addTriangle(Triangle(point, v2, v3));
            //add node to the dag
            dag.addNode(Node(totalTrianglesNumber + 1), parentNodeIndex);
            //update adjacencies
            triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 1,
                totalTrianglesNumber + 3, oldTriangleAdjacencies[1], totalTrianglesNumber, triangleIndex);

            //second triangle

            //add triangle to the triangulation
            triangulation.addTriangle(Triangle(adjacentV1, adjacentV2, point));
            //add node to the dag
            dag.addNode(Node(totalTrianglesNumber + 2), parentNodeIndex);
            //update adjacencies
            triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 2,
                oldNeighborAdjacencies[0], totalTrianglesNumber + 3, totalTrianglesNumber, adjacentTriangleIndex);

            //add triangle to the triangulation
            triangulation.addTriangle(Triangle(point, adjacentV2, adjacentV3));
            //add node to the dag
            dag.addNode(Node(totalTrianglesNumber + 3), parentNodeIndex);
            //update adjacencies
            triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 3,
                totalTrianglesNumber + 2, oldNeighborAdjacencies[1], totalTrianglesNumber + 1, adjacentTriangleIndex);

            break;
        case 1: //v1 -> adjacency in v2v3

            //first triangle

            //add triangle to the triangulation
            triangulation.addTriangle(Triangle(v1, v2, point));
            //add node to the dag
            dag.addNode(Node(totalTrianglesNumber), parentNodeIndex);
            //update adjacencies
            triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber,
                oldTriangleAdjacencies[0], totalTrianglesNumber + 3, totalTrianglesNumber + 1, triangleIndex);

            //add triangle to the triangulation
            triangulation.addTriangle(Triangle(v1, point, v3));
            //add node to the dag
            dag.addNode(Node(totalTrianglesNumber + 1), parentNodeIndex);
            //update adjacencies
            triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 1,
                totalTrianglesNumber, totalTrianglesNumber + 3, oldTriangleAdjacencies[2], triangleIndex);

            //second triangle

            //add triangle to the triangulation
            triangulation.addTriangle(Triangle(adjacentV1, adjacentV2, point));
            //add node to the dag
            dag.addNode(Node(totalTrianglesNumber + 2), parentNodeIndex);
            //update adjacencies
            triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 2,
                totalTrianglesNumber + 3, oldNeighborAdjacencies[1], totalTrianglesNumber + 1, adjacentTriangleIndex);

            //add triangle to the triangulation
            triangulation.addTriangle(Triangle(point, adjacentV2, adjacentV3));
            //add node to the dag
            dag.addNode(Node(totalTrianglesNumber + 3), parentNodeIndex);
            //update adjacencies
            triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 3,
                oldNeighborAdjacencies[0], totalTrianglesNumber + 2, totalTrianglesNumber, adjacentTriangleIndex);

            break;
        case 2: //v2 -> adjacency in v3v1

            //first triangle

            //add triangle to the triangulation
            triangulation.addTriangle(Triangle(point, v2, v3));
            //add node to the dag
            dag.addNode(Node(totalTrianglesNumber), parentNodeIndex);
            //update adjacencies
            triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber,
                oldTriangleAdjacencies[0], totalTrianglesNumber + 1, totalTrianglesNumber + 2, triangleIndex);

            //add triangle to the triangulation
            triangulation.addTriangle(Triangle(v1, v2, point));
            //add node to the dag
            dag.addNode(Node(totalTrianglesNumber + 1), parentNodeIndex);
            //update adjacencies
            triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 1,
                totalTrianglesNumber, oldNeighborAdjacencies[1], totalTrianglesNumber + 3, triangleIndex);

            //second triangle

            //add triangle to the triangulation
            triangulation.addTriangle(Triangle(adjacentV1, point, adjacentV3));
            //add node to the dag
            dag.addNode(Node(totalTrianglesNumber + 2), parentNodeIndex);
            //update adjacencies
            triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 2,
                totalTrianglesNumber, totalTrianglesNumber + 3, oldNeighborAdjacencies[2], adjacentTriangleIndex);

            //add triangle to the triangulation
            triangulation.addTriangle(Triangle(point, adjacentV2, adjacentV3));
            //add node to the dag
            dag.addNode(Node(totalTrianglesNumber + 3), parentNodeIndex);
            //update adjacencies
            triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 3,
                totalTrianglesNumber + 1, oldNeighborAdjacencies[1], totalTrianglesNumber + 2, adjacentTriangleIndex);

            break;
    }

    //TODO: legalize edge
}

}
