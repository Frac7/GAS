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
                  const unsigned int triangleIndex, const unsigned int adjacentIndex,
                  const cg3::Point2Dd& p1, const cg3::Point2Dd& p2, const cg3::Point2Dd& p3, const cg3::Point2Dd& pk,
                  const unsigned int edge, const unsigned int adjEdge,
                  std::array<int, dimension>& triangleAdj, std::array<int, dimension>& adjTriangleAdj)
{
    if(DelaunayTriangulation::Checker::
            isPointLyingInCircle(p1, p2, p3, pk, false))
    {
        const std::vector<Triangle>& triangles = triangulation.getTriangles();

        const unsigned int totalTrianglesNumber = triangles.size();

        //replace pi pj with pr pk

        switch (edge)
        {
            case v1v2Edge: //p1 p2 is pi pj, p3 is pr
            switch (adjEdge)
            {
                case v2v3Edge:
                    triangulation.addTriangle(Triangle(pk, p2, p3));
                    dag.addNode(Node(totalTrianglesNumber), triangleIndex, adjacentIndex);
                    triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber, adjTriangleAdj[0], triangleAdj[1], totalTrianglesNumber + 1, triangleIndex, adjacentIndex);

                    triangulation.addTriangle(Triangle(pk, p3, p1));
                    dag.addNode(Node(totalTrianglesNumber + 1), triangleIndex, adjacentIndex);
                    triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 1, totalTrianglesNumber, triangleAdj[2], adjTriangleAdj[2], triangleIndex, adjacentIndex);

                    //first triangle, pk pj is 0
                    if(adjTriangleAdj[0] != noAdjacentTriangle)
                    {
                        const int oppositePk = triangulation.findAdjacency(adjTriangleAdj[0], totalTrianglesNumber);
                        const int pkIndex = (oppositePk + 2) % 3; //TODO: replace magic numbers

                        const cg3::Point2Dd& newPk = pkIndex == 0? triangles[adjTriangleAdj[0]].getV1() : (pkIndex == 1? triangles[adjTriangleAdj[0]].getV2() : triangles[adjTriangleAdj[0]].getV3());
                        legalizeEdge(triangulation, dag, totalTrianglesNumber, adjTriangleAdj[0], pk, p2, p3, newPk, v1v2Edge, oppositePk,
                                triangulation.getAdjacenciesFromTriangle(totalTrianglesNumber), triangulation.getAdjacenciesFromTriangle(adjTriangleAdj[0]));
                    }

                    //second triangle, pi pk is 2
                    if(adjTriangleAdj[2] != noAdjacentTriangle)
                    {
                        const int oppositePk = triangulation.findAdjacency(adjTriangleAdj[2], totalTrianglesNumber + 1);
                        const int pkIndex = (oppositePk + 2) % 3; //TODO: replace magic numbers

                        const cg3::Point2Dd& newPk = pkIndex == 0? triangles[adjTriangleAdj[2]].getV1() : (pkIndex == 1? triangles[adjTriangleAdj[2]].getV2() : triangles[adjTriangleAdj[2]].getV3());
                        legalizeEdge(triangulation, dag, totalTrianglesNumber + 1, adjTriangleAdj[2], pk, p3, p1, newPk, v3v1Edge, oppositePk,
                                triangulation.getAdjacenciesFromTriangle(totalTrianglesNumber + 1), triangulation.getAdjacenciesFromTriangle(adjTriangleAdj[2]));
                    }

                    break;
                case v3v1Edge:
                    triangulation.addTriangle(Triangle(p1, pk, p3));
                    dag.addNode(Node(totalTrianglesNumber), triangleIndex, adjacentIndex);
                    triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber, adjTriangleAdj[0], totalTrianglesNumber + 1, triangleAdj[2], triangleIndex, adjacentIndex);

                    triangulation.addTriangle(Triangle(p3, pk, p2));
                    dag.addNode(Node(totalTrianglesNumber + 1), triangleIndex, adjacentIndex);
                    triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 1, totalTrianglesNumber, adjTriangleAdj[1], triangleAdj[1], triangleIndex, adjacentIndex);

                    //first triangle, pi pk is 0
                    if(adjTriangleAdj[0] != noAdjacentTriangle)
                    {
                        const int oppositePk = triangulation.findAdjacency(adjTriangleAdj[0], totalTrianglesNumber);
                        const int pkIndex = (oppositePk + 2) % 3; //TODO: replace magic numbers

                        const cg3::Point2Dd& newPk = pkIndex == 0? triangles[adjTriangleAdj[0]].getV1() : (pkIndex == 1? triangles[adjTriangleAdj[0]].getV2() : triangles[adjTriangleAdj[0]].getV3());
                        legalizeEdge(triangulation, dag, totalTrianglesNumber, adjTriangleAdj[0], p1, pk, p3, newPk, v1v2Edge, oppositePk,
                                triangulation.getAdjacenciesFromTriangle(totalTrianglesNumber), triangulation.getAdjacenciesFromTriangle(adjTriangleAdj[0]));
                    }

                    //second triangle, pk pj is 1
                    if(adjTriangleAdj[1] != noAdjacentTriangle)
                    {
                        const int oppositePk = triangulation.findAdjacency(adjTriangleAdj[1], totalTrianglesNumber + 1);
                        const int pkIndex = (oppositePk + 2) % 3; //TODO: replace magic numbers

                        const cg3::Point2Dd& newPk = pkIndex == 0? triangles[adjTriangleAdj[1]].getV1() : (pkIndex == 1? triangles[adjTriangleAdj[1]].getV2() : triangles[adjTriangleAdj[1]].getV3());
                        legalizeEdge(triangulation, dag, totalTrianglesNumber + 1, adjTriangleAdj[1], p3, pk, p2, newPk, v2v3Edge, oppositePk,
                                triangulation.getAdjacenciesFromTriangle(totalTrianglesNumber + 1), triangulation.getAdjacenciesFromTriangle(adjTriangleAdj[1]));
                    }
                    break;
            }
                break;
            case v2v3Edge: //p2 p3 is pi pj, p1 is pr
                switch (adjEdge)
                {
                    case v1v2Edge:
                        triangulation.addTriangle(Triangle(p1, p2, pk));
                        dag.addNode(Node(totalTrianglesNumber), triangleIndex, adjacentIndex);
                        triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber, triangleAdj[0], adjTriangleAdj[1], totalTrianglesNumber + 1, triangleIndex, adjacentIndex);

                        triangulation.addTriangle(Triangle(p1, pk, p3));
                        dag.addNode(Node(totalTrianglesNumber + 1), triangleIndex, adjacentIndex);
                        triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 1, totalTrianglesNumber, adjTriangleAdj[2], triangleAdj[2], triangleIndex, adjacentIndex);

                        //first triangle, pi pk is 1
                        if(adjTriangleAdj[1] != noAdjacentTriangle)
                        {
                            const int oppositePk = triangulation.findAdjacency(adjTriangleAdj[1], totalTrianglesNumber);
                            const int pkIndex = (oppositePk + 2) % 3; //TODO: replace magic numbers

                            const cg3::Point2Dd& newPk = pkIndex == 0? triangles[adjTriangleAdj[1]].getV1() : (pkIndex == 1? triangles[adjTriangleAdj[1]].getV2() : triangles[adjTriangleAdj[1]].getV3());
                            legalizeEdge(triangulation, dag, totalTrianglesNumber, adjTriangleAdj[1], p1, p2, pk, newPk, v2v3Edge, oppositePk,
                                    triangulation.getAdjacenciesFromTriangle(totalTrianglesNumber), triangulation.getAdjacenciesFromTriangle(adjTriangleAdj[1]));
                        }

                        //second triangle, pk pj is 1
                        if(adjTriangleAdj[2] != noAdjacentTriangle)
                        {
                            const int oppositePk = triangulation.findAdjacency(adjTriangleAdj[2], totalTrianglesNumber + 1);
                            const int pkIndex = (oppositePk + 2) % 3; //TODO: replace magic numbers

                            const cg3::Point2Dd& newPk = pkIndex == 0? triangles[adjTriangleAdj[2]].getV1() : (pkIndex == 1? triangles[adjTriangleAdj[2]].getV2() : triangles[adjTriangleAdj[2]].getV3());
                            legalizeEdge(triangulation, dag, totalTrianglesNumber + 1, adjTriangleAdj[2], p1, pk, p3, newPk, v2v3Edge, oppositePk,
                                    triangulation.getAdjacenciesFromTriangle(totalTrianglesNumber + 1), triangulation.getAdjacenciesFromTriangle(adjTriangleAdj[2]));
                        }

                        break;
                    case v3v1Edge:
                        triangulation.addTriangle(Triangle(p2, pk, p1));
                        dag.addNode(Node(totalTrianglesNumber), triangleIndex, adjacentIndex);
                        triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber, adjTriangleAdj[0], totalTrianglesNumber + 1, triangleAdj[0], triangleIndex, adjacentIndex);

                        triangulation.addTriangle(Triangle(p1, pk, p3));
                        dag.addNode(Node(totalTrianglesNumber + 1), triangleIndex, adjacentIndex);
                        triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 1, totalTrianglesNumber, adjTriangleAdj[1], triangleAdj[2], triangleIndex, adjacentIndex);

                        //first triangle, pi pk is 0
                        if(adjTriangleAdj[0] != noAdjacentTriangle)
                        {
                            const int oppositePk = triangulation.findAdjacency(adjTriangleAdj[0], totalTrianglesNumber);
                            const int pkIndex = (oppositePk + 2) % 3; //TODO: replace magic numbers

                            const cg3::Point2Dd& newPk = pkIndex == 0? triangles[adjTriangleAdj[0]].getV1() : (pkIndex == 1? triangles[adjTriangleAdj[0]].getV2() : triangles[adjTriangleAdj[0]].getV3());
                            legalizeEdge(triangulation, dag, totalTrianglesNumber, adjTriangleAdj[0], p2, pk, p1, newPk, v1v2Edge, oppositePk,
                                    triangulation.getAdjacenciesFromTriangle(totalTrianglesNumber), triangulation.getAdjacenciesFromTriangle(adjTriangleAdj[0]));
                        }

                        //second triangle, pk pj is 1
                        if(adjTriangleAdj[1] != noAdjacentTriangle)
                        {
                            const int oppositePk = triangulation.findAdjacency(adjTriangleAdj[1], totalTrianglesNumber + 1);
                            const int pkIndex = (oppositePk + 2) % 3; //TODO: replace magic numbers

                            const cg3::Point2Dd& newPk = pkIndex == 0? triangles[adjTriangleAdj[1]].getV1() : (pkIndex == 1? triangles[adjTriangleAdj[1]].getV2() : triangles[adjTriangleAdj[1]].getV3());
                            legalizeEdge(triangulation, dag, totalTrianglesNumber + 1, adjTriangleAdj[1], p1, pk, p3, newPk, v2v3Edge, oppositePk,
                                    triangulation.getAdjacenciesFromTriangle(totalTrianglesNumber + 1), triangulation.getAdjacenciesFromTriangle(adjTriangleAdj[1]));
                        }

                        break;
                }
                break;
            case v3v1Edge: //p3 p1 is pi pj, p2 is pr
                switch (adjEdge)
                {
                    case v1v2Edge:
                        triangulation.addTriangle(Triangle(p2, p3, pk));
                        dag.addNode(Node(totalTrianglesNumber), triangleIndex, adjacentIndex);
                        triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber, triangleAdj[1], adjTriangleAdj[1], totalTrianglesNumber + 1, triangleIndex, adjacentIndex);

                        triangulation.addTriangle(Triangle(p1, p2, pk));
                        dag.addNode(Node(totalTrianglesNumber + 1), triangleIndex, adjacentIndex);
                        triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 1, triangleAdj[0], totalTrianglesNumber, adjTriangleAdj[2], triangleIndex, adjacentIndex);

                        //first triangle, pi pk is 1
                        if(adjTriangleAdj[1] != noAdjacentTriangle)
                        {
                            const int oppositePk = triangulation.findAdjacency(adjTriangleAdj[1], totalTrianglesNumber);
                            const int pkIndex = (oppositePk + 2) % 3; //TODO: replace magic numbers

                            const cg3::Point2Dd& newPk = pkIndex == 0? triangles[adjTriangleAdj[1]].getV1() : (pkIndex == 1? triangles[adjTriangleAdj[1]].getV2() : triangles[adjTriangleAdj[1]].getV3());
                            legalizeEdge(triangulation, dag, totalTrianglesNumber, adjTriangleAdj[1], p2, p3, pk, newPk, v2v3Edge, oppositePk,
                                    triangulation.getAdjacenciesFromTriangle(totalTrianglesNumber), triangulation.getAdjacenciesFromTriangle(adjTriangleAdj[1]));
                        }

                        //second triangle, pk pj is 2
                        if(adjTriangleAdj[2] != noAdjacentTriangle)
                        {
                            const int oppositePk = triangulation.findAdjacency(adjTriangleAdj[2], totalTrianglesNumber + 1);
                            const int pkIndex = (oppositePk + 2) % 3; //TODO: replace magic numbers

                            const cg3::Point2Dd& newPk = pkIndex == 0? triangles[adjTriangleAdj[2]].getV1() : (pkIndex == 1? triangles[adjTriangleAdj[2]].getV2() : triangles[adjTriangleAdj[2]].getV3());
                            legalizeEdge(triangulation, dag, totalTrianglesNumber + 1, adjTriangleAdj[2], p1, p2, pk, newPk, v3v1Edge, oppositePk,
                                    triangulation.getAdjacenciesFromTriangle(totalTrianglesNumber + 1), triangulation.getAdjacenciesFromTriangle(adjTriangleAdj[2]));
                        }

                        break;
                    case v2v3Edge:
                        triangulation.addTriangle(Triangle(pk, p2, p3));
                        dag.addNode(Node(totalTrianglesNumber), triangleIndex, adjacentIndex);
                        triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber, totalTrianglesNumber + 1, triangleAdj[1], adjTriangleAdj[2], triangleIndex, adjacentIndex);

                        triangulation.addTriangle(Triangle(pk, p1, p2));
                        dag.addNode(Node(totalTrianglesNumber + 1), triangleIndex, adjacentIndex);
                        triangulation.addAdjacenciesForNewTriangle(totalTrianglesNumber + 1, adjTriangleAdj[0], triangleAdj[0], totalTrianglesNumber, triangleIndex, adjacentIndex);

                        //first triangle, pi pk is 2
                        if(adjTriangleAdj[2] != noAdjacentTriangle)
                        {
                            const int oppositePk = triangulation.findAdjacency(adjTriangleAdj[2], totalTrianglesNumber);
                            const int pkIndex = (oppositePk + 2) % 3; //TODO: replace magic numbers

                            const cg3::Point2Dd& newPk = pkIndex == 0? triangles[adjTriangleAdj[2]].getV1() : (pkIndex == 1? triangles[adjTriangleAdj[2]].getV2() : triangles[adjTriangleAdj[2]].getV3());
                            legalizeEdge(triangulation, dag, totalTrianglesNumber, adjTriangleAdj[2], pk, p2, p3, newPk, v3v1Edge, oppositePk,
                                    triangulation.getAdjacenciesFromTriangle(totalTrianglesNumber), triangulation.getAdjacenciesFromTriangle(adjTriangleAdj[2]));
                        }

                        //second triangle, pk pj is 0
                        if(adjTriangleAdj[0] != noAdjacentTriangle)
                        {
                            const int oppositePk = triangulation.findAdjacency(adjTriangleAdj[0], totalTrianglesNumber + 1);
                            const int pkIndex = (oppositePk + 2) % 3; //TODO: replace magic numbers

                            const cg3::Point2Dd& newPk = pkIndex == 0? triangles[adjTriangleAdj[0]].getV1() : (pkIndex == 1? triangles[adjTriangleAdj[0]].getV2() : triangles[adjTriangleAdj[0]].getV3());
                            legalizeEdge(triangulation, dag, totalTrianglesNumber + 1, adjTriangleAdj[0], pk, p1, p2, newPk, v1v2Edge, oppositePk,
                                    triangulation.getAdjacenciesFromTriangle(totalTrianglesNumber + 1), triangulation.getAdjacenciesFromTriangle(adjTriangleAdj[0]));
                        }

                        break;
                }

                break;
        }
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
        const int adjacency0 = oldTriangleAdjacencies[v1v2Edge];
        const int adjacency1 = oldTriangleAdjacencies[v2v3Edge];
        const int adjacency2 = oldTriangleAdjacencies[v3v1Edge];

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

        if(adjacency0 != noAdjacentTriangle)
        {
            const int oppositePk = triangulation.findAdjacency(adjacency0, totalTrianglesNumber);
            const int pkIndex = (oppositePk + 2) % 3; //TODO: replace magic numbers

            const cg3::Point2Dd& pk = pkIndex == 0? triangles[adjacency0].getV1() : (pkIndex == 1? triangles[adjacency0].getV2() : triangles[adjacency0].getV3());
            legalizeEdge(triangulation, dag, totalTrianglesNumber, adjacency0, v1, v2, point, pk, v1v2Edge, oppositePk, triangulation.getAdjacenciesFromTriangle(totalTrianglesNumber), triangulation.getAdjacenciesFromTriangle(adjacency0));
        }

        if(adjacency1 != noAdjacentTriangle)
        {
            const int oppositePk = triangulation.findAdjacency(adjacency1, totalTrianglesNumber + 1);
            const int pkIndex = (oppositePk + 2) % 3; //TODO: replace magic numbers

            const cg3::Point2Dd& pk = pkIndex == 0? triangles[adjacency1].getV1() : (pkIndex == 1? triangles[adjacency1].getV2() : triangles[adjacency1].getV3());
            legalizeEdge(triangulation, dag, totalTrianglesNumber + 1, adjacency1, point, v2, v3, pk, v2v3Edge, oppositePk, triangulation.getAdjacenciesFromTriangle(totalTrianglesNumber + 1), triangulation.getAdjacenciesFromTriangle(adjacency1));
        }

        if(adjacency2 != noAdjacentTriangle)
        {
            const int oppositePk = triangulation.findAdjacency(adjacency2, totalTrianglesNumber + 2);
            const int pkIndex = (oppositePk + 2) % 3; //TODO: replace magic numbers

            const cg3::Point2Dd& pk = pkIndex == 0? triangles[adjacency2].getV1() : (pkIndex == 1? triangles[adjacency2].getV2() : triangles[adjacency2].getV3());
            legalizeEdge(triangulation, dag, totalTrianglesNumber + 2, adjacency2, v1, point, v3, pk, v3v1Edge, oppositePk, triangulation.getAdjacenciesFromTriangle(totalTrianglesNumber + 2), triangulation.getAdjacenciesFromTriangle(adjacency2));
        }

    }


}

}
