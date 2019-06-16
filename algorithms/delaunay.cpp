#include "delaunay.h"

#include <utils/delaunay_checker.h>
#include <cg3lib/cg3/core/cg3/geometry/2d/utils2d.h>

namespace DelaunayTriangulation {

namespace Checker {

void fillDataStructures(Triangulation& triangulation, DAG& dag, std::vector<cg3::Point2Dd>& points, cg3::Array2D<unsigned int>& triangles)
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

void legalizeEdge(Triangulation& triangulation, DAG& dag,
                  const unsigned int& triangleIndex, const unsigned int& adjacentIndex,
                  const cg3::Point2Dd& v1, const cg3::Point2Dd& v2, const cg3::Point2Dd& v3, const cg3::Point2Dd& adjacentOpposite)
{
    if(!DelaunayTriangulation::Checker::
            isPointLyingInCircle(v1, v2, v3, adjacentOpposite, true))
    {

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

        //adjacent triangles are known (their index)
        //for legalizing and edge I need v1, v2, v3 (one of these is the point to be inserted) and pk
        //pk is the vertex of the adjacent triangle (the unique vertex that is not in the edge to be tested)

        //in legalize edge I need also the triangulation to call the function recursively and I need to insert triangles
        //I need the dag to insert new nodes

        //const unsigned int& adjacencyNeighbourToTriangle = triangulation.findAdjacency(oldTriangleAdjacencies[0], totalTrianglesNumber);
        //legalizeEdge(triangulation, dag, totalTrianglesNumber, oldTriangleAdjacencies[0], v1, v2, point, pk);
        //legalizeEdge(triangulation, dag, totalTrianglesNumber + 1, oldTriangleAdjacencies[1], point, v2, v3, pk);
        //legalizeEdge(triangulation, dag, totalTrianglesNumber + 2, oldTriangleAdjacencies[2], v1, point, v3, pk);

    }


}

}
