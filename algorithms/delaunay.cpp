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
                  const cg3::Point2Dd& pi, const cg3::Point2Dd& pj, const cg3::Point2Dd& pr, const cg3::Point2Dd& pk,
                  const unsigned int& edge, std::array<int, dimension>& triangleAdj, std::array<int, dimension>& adjTriangleAdj)
{
    if(DelaunayTriangulation::Checker::
            isPointLyingInCircle(pi, pj, pr, pk, false))
    {
        //TODO: verify adjacencies

        const unsigned int totalTrianglesNumber = triangulation.getTriangles().size();

        int adjacentTriangle1_0 = adjTriangleAdj[0] == triangleIndex? adjTriangleAdj[1] : adjTriangleAdj[0];
        int adjacentTriangle1_2 = edge == 0? triangleAdj[1] : triangleAdj[0];

        int adjacentTriangle2_0 = adjTriangleAdj[2] == triangleIndex? adjTriangleAdj[1] : adjTriangleAdj[2];
        int adjacentTriangle2_1 = edge == 2? triangleAdj[1] : triangleAdj[2];

        //TODO: refactoring
        /*int adj11, adj12, adj21, adj22;
        if(edge == 0)
        {
            adj11 = triangleAdj[1];
            adj12 = triangleAdj[2];
        } else if(edge == 1)
        {
            adj11 = triangleAdj[0];
            adj12 = triangleAdj[2];
        } else if(edge == 2)
        {
            adj11 = triangleAdj[0];
            adj12 = triangleAdj[1];
        }
        if(adjTriangleAdj[0] == triangleIndex)
        {
            adj21 = adjTriangleAdj[1];
            adj22 = adjTriangleAdj[2];
        } else if(adjTriangleAdj[1] == triangleIndex)
        {
            adj21 = adjTriangleAdj[0];
            adj22 = adjTriangleAdj[2];
        } else if(adjTriangleAdj[2] == triangleIndex)
        {
            adj21 = adjTriangleAdj[0];
            adj22 = adjTriangleAdj[1];
        }*/

        triangulation.addTriangle(Triangle(pi, pk, pr));
        dag.addNode(Node(totalTrianglesNumber), triangleIndex, adjacentIndex);
        triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber, adjacentTriangle1_0, totalTrianglesNumber + 1, adjacentTriangle1_2, adjacentIndex);

        triangulation.addTriangle(Triangle(pr, pk, pj));
        dag.addNode(Node(totalTrianglesNumber + 1), triangleIndex, adjacentIndex);
        triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 1, totalTrianglesNumber, adjacentTriangle2_0, adjacentTriangle2_1, triangleIndex);

        //TODO: legalize

        //legalizeEdge(triangulation, dag, totalTrianglesNumber, 0, pi, pk, pr, pj, 0);
        //legalizeEdge(triangulation, dag, totalTrianglesNumber, 0, pr, pk, pj, pi, 0);
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

        //these values are cached because during the computation in this function some adjacencies are added
        //the address of this array changes (it belongs to a vector)
        const int adjacency0 = oldTriangleAdjacencies[0];
        const int adjacency1 = oldTriangleAdjacencies[1];
        const int adjacency2 = oldTriangleAdjacencies[2];

        //TODO: avoid code repetition

        //create 3 new triangles and 3 new nodes

        //add triangle to the triangulation
        triangulation.addTriangle(Triangle(v1, v2, point));
        //add node to the dag
        dag.addNode(Node(totalTrianglesNumber), parentNodeIndex);
        //update adjacencies
        triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber,
            adjacency0, totalTrianglesNumber + 1, totalTrianglesNumber + 2, triangleIndex);

        //add triangle to the triangulation
        triangulation.addTriangle(Triangle(point, v2, v3)); //totalTrianglesNumber + 1
        //add node to the dag
        dag.addNode(Node(totalTrianglesNumber + 1), parentNodeIndex);
        //update adjacencies
        triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 1,
            totalTrianglesNumber, adjacency1, totalTrianglesNumber + 2, triangleIndex);

        //add triangle to the triangulation
        triangulation.addTriangle(Triangle(v1, point, v3)); //totalTrianglesNumber + 2
        //add node to the dag
        dag.addNode(Node(totalTrianglesNumber + 2), parentNodeIndex);
        //update adjacencies
        triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 2,
            totalTrianglesNumber, totalTrianglesNumber + 1, adjacency2, triangleIndex);

        //adjacent triangles are known (their index)
        //for legalizing and edge I need v1, v2, v3 (one of these is the point to be inserted) and pk
        //pk is the vertex of the adjacent triangle (the unique vertex that is not in the edge to be tested)

        //in legalize edge I need also the triangulation to call the function recursively and I need to insert triangles
        //I need the dag to insert new nodes

        //oppositepk is the edge of the neighbour where I can find the adjacency with the triangle
        //using this edge I can find pk that is the opposite point of this edge in the adjacent triangle

        if(adjacency0 != -1)
        {
            const int oppositePk = triangulation.findAdjacency(adjacency0, totalTrianglesNumber);
            const int pkIndex = (oppositePk + 2) % 3; //TODO: replace magic numbers

            const cg3::Point2Dd& pk = pkIndex == 0? triangles[adjacency0].getV1() : (pkIndex == 1? triangles[adjacency0].getV2() : triangles[adjacency0].getV3());
            legalizeEdge(triangulation, dag, totalTrianglesNumber, adjacency0, v1, v2, point, pk, 0, triangulation.getAdjacenciesFromTriangle(totalTrianglesNumber), triangulation.getAdjacenciesFromTriangle(adjacency0));
        }

        if(adjacency1 != -1)
        {
            const int oppositePk = triangulation.findAdjacency(adjacency1, totalTrianglesNumber + 1);
            const int pkIndex = (oppositePk + 2) % 3; //TODO: replace magic numbers

            const cg3::Point2Dd& pk = pkIndex == 0? triangles[adjacency1].getV1() : (pkIndex == 1? triangles[adjacency1].getV2() : triangles[adjacency1].getV3());
            legalizeEdge(triangulation, dag, totalTrianglesNumber + 1, adjacency1, v2, v3, point, pk, 1, triangulation.getAdjacenciesFromTriangle(totalTrianglesNumber + 1), triangulation.getAdjacenciesFromTriangle(adjacency1));
        }

        if(adjacency2 != -1)
        {
            const int oppositePk = triangulation.findAdjacency(adjacency2, totalTrianglesNumber + 2);
            const int pkIndex = (oppositePk + 2) % 3; //TODO: replace magic numbers

            const cg3::Point2Dd& pk = pkIndex == 0? triangles[adjacency2].getV1() : (pkIndex == 1? triangles[adjacency2].getV2() : triangles[adjacency2].getV3());
            legalizeEdge(triangulation, dag, totalTrianglesNumber + 2, adjacency2, v3, v1, point, pk, 2, triangulation.getAdjacenciesFromTriangle(totalTrianglesNumber + 2), triangulation.getAdjacenciesFromTriangle(adjacency2));
        }

    }


}

}
