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
            if(nodeVector[i].isLeaf())
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

void legalizeEdge(Triangulation& triangulation, DAG& dag, const cg3::Point2Dd& point, const unsigned int& pipj, const unsigned int& triangle)
{
    const std::vector<Triangle>& triangles = triangulation.getTriangles();
    Triangle triangleObject = triangles[triangle];

    if(!DelaunayTriangulation::Checker::
            isPointLyingInCircle(triangleObject.getV1(), triangleObject.getV2(), triangleObject.getV3(), point, true))
    {
        const unsigned int& trianglesNumber = triangles.size();

        const unsigned int& adjacentTriangle = triangulation.getAdjacenciesFromTriangle(triangle)[pipj];
        const unsigned int& adjacentEdgeAdjacentTriangle = triangulation.findOldAdjacency(adjacentTriangle, triangle);

        //TODO: replace magic numbers
        const unsigned int& pk = (adjacentEdgeAdjacentTriangle + 2) % 3;
        const unsigned int& pr = (adjacentTriangle + 2) % 3;

        //pick the adjacent triangle
        Triangle adjacent = triangles[adjacentTriangle];

        const cg3::Point2Dd& oldTA = triangleObject.getV1();
        const cg3::Point2Dd& oldTB = triangleObject.getV2();
        const cg3::Point2Dd& oldTC = triangleObject.getV3();

        const cg3::Point2Dd& oldAA = adjacent.getV1();
        const cg3::Point2Dd& oldAB = adjacent.getV2();
        const cg3::Point2Dd& oldAC = adjacent.getV3();

        //TODO: replace edge

        dag.addNode(Node(trianglesNumber), triangle, adjacentTriangle);
        dag.addNode(Node(trianglesNumber + 1), triangle, adjacentTriangle);

        triangulation.addTriangle(triangleObject);
        triangulation.addTriangle(adjacent);

        //TODO: change these numbers
        legalizeEdge(triangulation, dag, point, 0, trianglesNumber);
        legalizeEdge(triangulation, dag, point, 1, trianglesNumber + 1);

        //TODO: modify adjacencies

    }
}

void incrementalTriangulation(Triangulation& triangulation, DAG& dag, const cg3::Point2Dd& point)
{
    const std::vector<Triangle>& triangles = triangulation.getTriangles();

    const std::vector<Node>& nodes = dag.getNodeList();
    const int totalNodes = nodes.size();

    //find the triangle that contains this point using the DAG
    const unsigned int parentNodeIndex = dag.searchInNodes(totalNodes, point, triangles);
    const unsigned int triangleIndex = nodes[parentNodeIndex].getData();
    //these numbers must be not equal to -1

    //get vertices of containing triangle
    const cg3::Point2Dd& v1 = triangles[triangleIndex].getV1();
    const cg3::Point2Dd& v2 = triangles[triangleIndex].getV2();
    const cg3::Point2Dd& v3 = triangles[triangleIndex].getV3();

    if(point != v1 && point != v2 && point != v3)
    {
        const unsigned int totalTrianglesNumber = triangles.size();

        const std::array<int, maxAdjacentTriangles>& oldTriangleAdjacencies = triangulation.getAdjacenciesFromTriangle(triangleIndex);

        //TODO: avoid code repetition

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

        legalizeEdge(triangulation, dag, point, 0, totalTrianglesNumber);
        legalizeEdge(triangulation, dag, point, 1, totalTrianglesNumber + 1);
        legalizeEdge(triangulation, dag, point, 2, totalTrianglesNumber + 2);

    }


}

}
