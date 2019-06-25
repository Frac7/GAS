#include "delaunay.h"

#include <utils/delaunay_checker.h>
#include <cg3lib/cg3/core/cg3/geometry/2d/utils2d.h>

namespace DelaunayTriangulation {

namespace Checker {

/**
 * @brief Fills points and triangle to check the computed triangulation
 * @param[in] triangulation: the triangulation data structure
 * @param[in] dag: the search data structure
 * @param[in] points: data structure to fill with points
 * @param[in] triangles: data strucutre to fill with triangles
*/
void fillDataStructures(Triangulation& triangulation, DAG& dag, std::vector<cg3::Point2Dd>& points, cg3::Array2D<unsigned int>& triangles)
{
    const std::vector<Triangle>& triangleVector = triangulation.getTriangles();
    const std::vector<Node>& nodeVector = dag.getNodeList();

    unsigned int vectorSize = unsigned(triangleVector.size());

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

/**
 * @brief Edge flip
 * @param[in] triangulation: triangulation data structure
 * @param[in] dag: search data structure
 * @param[in] triangle index: index of new triangle to legalize
 * @param[in] adjacent index: index of adjacent triangle in the edge to legalize
 * @param[in] p1: triangle vertex 1
 * @param[in] p2: triangle vertex 2
 * @param[in] p3: triangle vertex 3
 * @param[in] pk: triangle opposite vertex
 * @param[in] triangle adjacencies: adjacencies for the new triangle
 * @param[in] adjacent triangle adjacencies: adjacencies for the adjacent triangle
*/
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

        unsigned int totalTrianglesNumber = unsigned(triangles.size());

        //replace pi pj with pr pk

        switch (edge)
        {
            case v1v2Edge: //p1 p2 is pi pj, p3 is pr
            switch (adjEdge)
            {
                case v1v2Edge: //pk was v3 in the adjacent triangle

                    addElementAfterFlip(triangulation, dag,
                                        totalTrianglesNumber, triangleIndex, adjacentIndex,
                                        p2, p3, pk,
                                        triangleAdj[v2v3Edge], int(totalTrianglesNumber + 1), adjTriangleAdj[v3v1Edge]);

                    addElementAfterFlip(triangulation, dag,
                                        totalTrianglesNumber + 1, triangleIndex, adjacentIndex,
                                        p3, p1, pk,
                                        triangleAdj[v3v1Edge], adjTriangleAdj[v2v3Edge], int(totalTrianglesNumber));

                    //first triangle, pk pj is 2
                    if(adjTriangleAdj[v3v1Edge] != noAdjacentTriangle)
                    {
                        testEdge(triangulation, dag, totalTrianglesNumber, unsigned(adjTriangleAdj[v3v1Edge]), v3v1Edge, p2, p3, pk);
                    }

                    //second triangle, pi pk is 1
                    if(adjTriangleAdj[v2v3Edge] != noAdjacentTriangle)
                    {
                        testEdge(triangulation, dag, totalTrianglesNumber + 1, unsigned(adjTriangleAdj[v2v3Edge]), v2v3Edge, p3, p1, pk);
                    }
                    break;

                case v2v3Edge: //pk was v1 in the adjacent triangle

                    addElementAfterFlip(triangulation, dag,
                                        totalTrianglesNumber, triangleIndex, adjacentIndex,
                                        pk, p2, p3,
                                        adjTriangleAdj[v1v2Edge], triangleAdj[v2v3Edge], int(totalTrianglesNumber + 1));

                    addElementAfterFlip(triangulation, dag,
                                        totalTrianglesNumber + 1, triangleIndex, adjacentIndex,
                                        pk, p3, p1,
                                        int(totalTrianglesNumber), triangleAdj[v3v1Edge], adjTriangleAdj[v3v1Edge]);

                    //first triangle, pk pj is 0
                    if(adjTriangleAdj[v1v2Edge] != noAdjacentTriangle)
                    {
                        testEdge(triangulation, dag, totalTrianglesNumber, unsigned(adjTriangleAdj[v1v2Edge]), v1v2Edge, pk, p2, p3);
                    }

                    //second triangle, pi pk is 2
                    if(adjTriangleAdj[v3v1Edge] != noAdjacentTriangle)
                    {
                        testEdge(triangulation, dag, totalTrianglesNumber + 1, unsigned(adjTriangleAdj[v3v1Edge]), v3v1Edge, pk, p3, p1);
                    }

                    break;
                case v3v1Edge: //pk was v2 in the adjacent triangle

                    addElementAfterFlip(triangulation, dag,
                                        totalTrianglesNumber, triangleIndex, adjacentIndex,
                                        p1, pk, p3,
                                        adjTriangleAdj[v1v2Edge], int(totalTrianglesNumber + 1), triangleAdj[v3v1Edge]);

                    addElementAfterFlip(triangulation, dag,
                                        totalTrianglesNumber + 1, triangleIndex, adjacentIndex,
                                        p3, pk, p2,
                                        int(totalTrianglesNumber), adjTriangleAdj[v2v3Edge], triangleAdj[v2v3Edge]);

                    //first triangle, pi pk is 0
                    if(adjTriangleAdj[v1v2Edge] != noAdjacentTriangle)
                    {
                        testEdge(triangulation, dag, totalTrianglesNumber, unsigned(adjTriangleAdj[v1v2Edge]), v1v2Edge, p1, pk, p3);
                    }

                    //second triangle, pk pj is 1
                    if(adjTriangleAdj[v2v3Edge] != noAdjacentTriangle)
                    {
                        testEdge(triangulation, dag, totalTrianglesNumber + 1, unsigned(adjTriangleAdj[v2v3Edge]), v2v3Edge, p3, pk, p2);
                    }
                    break;
            }
            break;
            case v2v3Edge: //p2 p3 is pi pj, p1 is pr
                switch (adjEdge)
                {
                    case v1v2Edge: // pk was v3 in the adjacent triangle

                        addElementAfterFlip(triangulation, dag,
                                            totalTrianglesNumber, triangleIndex, adjacentIndex,
                                            p1, p2, pk,
                                            triangleAdj[v1v2Edge], adjTriangleAdj[v2v3Edge], int(totalTrianglesNumber + 1));

                        addElementAfterFlip(triangulation, dag,
                                            totalTrianglesNumber + 1, triangleIndex, adjacentIndex,
                                            p3, p1, pk,
                                            triangleAdj[v3v1Edge], int(totalTrianglesNumber), adjTriangleAdj[v3v1Edge]);

                        //first triangle, pi pk is 1
                        if(adjTriangleAdj[v2v3Edge] != noAdjacentTriangle)
                        {
                            testEdge(triangulation, dag, totalTrianglesNumber, unsigned(adjTriangleAdj[v2v3Edge]), v2v3Edge, p1, p2, pk);
                        }

                        //second triangle, pk pj is 2
                        if(adjTriangleAdj[v3v1Edge] != noAdjacentTriangle)
                        {
                            testEdge(triangulation, dag, totalTrianglesNumber + 1, unsigned(adjTriangleAdj[v3v1Edge]), v3v1Edge, p3, p1, pk);
                        }

                        break;

                case v2v3Edge: // pk was v1 in the adjacent triangle

                    addElementAfterFlip(triangulation, dag,
                                        totalTrianglesNumber, triangleIndex, adjacentIndex,
                                        pk, p1, p2,
                                        int(totalTrianglesNumber + 1), triangleAdj[v1v2Edge], adjTriangleAdj[v3v1Edge]);

                    addElementAfterFlip(triangulation, dag,
                                        totalTrianglesNumber + 1, triangleIndex, adjacentIndex,
                                        pk, p3, p1,
                                        adjTriangleAdj[v1v2Edge], triangleAdj[v3v1Edge], int(totalTrianglesNumber));

                    //first triangle, pi pk is 2
                    if(adjTriangleAdj[v3v1Edge] != noAdjacentTriangle)
                    {
                        testEdge(triangulation, dag, totalTrianglesNumber, unsigned(adjTriangleAdj[v3v1Edge]), v3v1Edge, pk, p1, p2);
                    }

                    //second triangle, pk pj is 0
                    if(adjTriangleAdj[v1v2Edge] != noAdjacentTriangle)
                    {
                        testEdge(triangulation, dag, totalTrianglesNumber + 1, unsigned(adjTriangleAdj[v1v2Edge]), v1v2Edge, pk, p3, p1);
                    }

                    break;

                    case v3v1Edge: //pk was v2 in the adjacent triangle

                        addElementAfterFlip(triangulation, dag,
                                            totalTrianglesNumber, triangleIndex, adjacentIndex,
                                            p2, pk, p1,
                                            adjTriangleAdj[v1v2Edge], int(totalTrianglesNumber + 1), triangleAdj[v1v2Edge]);

                        addElementAfterFlip(triangulation, dag,
                                            totalTrianglesNumber + 1, triangleIndex, adjacentIndex,
                                            p1, pk, p3,
                                            int(totalTrianglesNumber), adjTriangleAdj[v2v3Edge], triangleAdj[v3v1Edge]);

                        //first triangle, pi pk is 0
                        if(adjTriangleAdj[v1v2Edge] != noAdjacentTriangle)
                        {
                            testEdge(triangulation, dag, totalTrianglesNumber, unsigned(adjTriangleAdj[v1v2Edge]), v1v2Edge, p2, pk, p1);
                        }

                        //second triangle, pk pj is 1
                        if(adjTriangleAdj[v2v3Edge] != noAdjacentTriangle)
                        {
                            testEdge(triangulation, dag, totalTrianglesNumber + 1, unsigned(adjTriangleAdj[v2v3Edge]), v2v3Edge, p1, pk, p3);
                        }

                        break;
                }
                break;
            case v3v1Edge: //p3 p1 is pi pj, p2 is pr
                switch (adjEdge)
                {
                    case v1v2Edge: //pk was v3 in the adjacent triangle

                        addElementAfterFlip(triangulation, dag,
                                            totalTrianglesNumber, triangleIndex, adjacentIndex,
                                            p2, p3, pk,
                                            triangleAdj[v2v3Edge], adjTriangleAdj[v2v3Edge], int(totalTrianglesNumber + 1));

                        addElementAfterFlip(triangulation, dag,
                                            totalTrianglesNumber + 1, triangleIndex, adjacentIndex,
                                            p1, p2, pk,
                                            triangleAdj[v1v2Edge], int(totalTrianglesNumber), adjTriangleAdj[v3v1Edge]);

                        //first triangle, pi pk is 1
                        if(adjTriangleAdj[v2v3Edge] != noAdjacentTriangle)
                        {
                            testEdge(triangulation, dag, totalTrianglesNumber, unsigned(adjTriangleAdj[v2v3Edge]), v2v3Edge, p2, p3, pk);
                        }

                        //second triangle, pk pj is 2
                        if(adjTriangleAdj[v3v1Edge] != noAdjacentTriangle)
                        {
                            testEdge(triangulation, dag, totalTrianglesNumber + 1, unsigned(adjTriangleAdj[v3v1Edge]), v3v1Edge, p1, p2, pk);
                        }

                        break;
                    case v2v3Edge: //pk was v1 in the adjacent triangle

                        addElementAfterFlip(triangulation, dag,
                                            totalTrianglesNumber, triangleIndex, adjacentIndex,
                                            pk, p2, p3,
                                            int(totalTrianglesNumber + 1), triangleAdj[v2v3Edge], adjTriangleAdj[v3v1Edge]);

                        addElementAfterFlip(triangulation, dag,
                                            totalTrianglesNumber + 1, triangleIndex, adjacentIndex,
                                            pk, p1, p2,
                                            adjTriangleAdj[v1v2Edge], triangleAdj[v1v2Edge], int(totalTrianglesNumber));

                        //first triangle, pi pk is 2
                        if(adjTriangleAdj[v3v1Edge] != noAdjacentTriangle)
                        {
                            testEdge(triangulation, dag, totalTrianglesNumber, unsigned(adjTriangleAdj[v3v1Edge]), v3v1Edge, pk, p2, p3);
                        }

                        //second triangle, pk pj is 0
                        if(adjTriangleAdj[v1v2Edge] != noAdjacentTriangle)
                        {
                            testEdge(triangulation, dag, totalTrianglesNumber + 1, unsigned(adjTriangleAdj[v1v2Edge]), v1v2Edge, pk, p1, p2);
                        }

                        break;

                case v3v1Edge: //pk was v2 in the adjacent triangle

                    addElementAfterFlip(triangulation, dag,
                                        totalTrianglesNumber, triangleIndex, adjacentIndex,
                                        p3, pk, p2,
                                        adjTriangleAdj[v1v2Edge], int(totalTrianglesNumber + 1), triangleAdj[v2v3Edge]);

                    addElementAfterFlip(triangulation, dag,
                                        totalTrianglesNumber + 1, triangleIndex, adjacentIndex,
                                        p2, pk, p1,
                                        int(totalTrianglesNumber), adjTriangleAdj[v2v3Edge], triangleAdj[v1v2Edge]);

                    //first triangle, pi pk is 0
                    if(adjTriangleAdj[v1v2Edge] != noAdjacentTriangle)
                    {
                        testEdge(triangulation, dag, totalTrianglesNumber, unsigned(adjTriangleAdj[v1v2Edge]), v1v2Edge, p3, pk, p2);
                    }

                    //second triangle, pk pj is 1
                    if(adjTriangleAdj[v2v3Edge] != noAdjacentTriangle)
                    {
                        testEdge(triangulation, dag, totalTrianglesNumber + 1, unsigned(adjTriangleAdj[v2v3Edge]), v2v3Edge, p2, pk, p1);
                    }

                    break;

                }

                break;
        }
    }
}

/**
 * @brief Incremental step for computing the triangulation
 * @param[in] triangulation: triangulation data structure
 * @param[in] dag: search data structure
 * @param[in] point: the point to be added to the triangulation
*/
void incrementalTriangulation(Triangulation& triangulation, DAG& dag, const cg3::Point2Dd& point)
{
    const std::vector<Triangle>& triangles = triangulation.getTriangles();

    const std::vector<Node>& nodes = dag.getNodeList();

    //find the triangle that contains this point using the DAG
    unsigned int parentNodeIndex = unsigned(dag.searchInNodes(point, triangles));
    unsigned int triangleIndex = unsigned(nodes[parentNodeIndex].getData());
    //these numbers must be not equal to -1

    //get vertices of containing triangle
    const cg3::Point2Dd& v1 = triangles[triangleIndex].getV1();
    const cg3::Point2Dd& v2 = triangles[triangleIndex].getV2();
    const cg3::Point2Dd& v3 = triangles[triangleIndex].getV3();

    //cases where two points coincide are not managed
    if(point != v1 && point != v2 && point != v3)
    {
        unsigned int totalTrianglesNumber = unsigned(triangles.size());

        const std::array<int, maxAdjacentTriangles>& oldTriangleAdjacencies = triangulation.getAdjacenciesFromTriangle(triangleIndex);

        //these values are cached because during the computation in this function some adjacencies are added
        //the address of this array changes (it belongs to a vector)
        int adjacency0 = oldTriangleAdjacencies[v1v2Edge];
        int adjacency1 = oldTriangleAdjacencies[v2v3Edge];
        int adjacency2 = oldTriangleAdjacencies[v3v1Edge];

        //create 3 new triangles and 3 new nodes

        addElementToTriangulation(triangulation, dag,
                                  totalTrianglesNumber, triangleIndex,
                                  v1, v2, point,
                                  adjacency0, int(totalTrianglesNumber + 1), int(totalTrianglesNumber + 2));

        addElementToTriangulation(triangulation, dag,
                                  totalTrianglesNumber + 1, triangleIndex,
                                  point, v2, v3,
                                  int(totalTrianglesNumber), adjacency1, int(totalTrianglesNumber + 2));

        addElementToTriangulation(triangulation, dag,
                                  totalTrianglesNumber + 2, triangleIndex,
                                  v1, point, v3,
                                  int(totalTrianglesNumber), int(totalTrianglesNumber + 1), adjacency2);

        //adjacent triangles are known (their index)
        //for legalizing and edge I need v1, v2, v3 (one of these is the point to be inserted) and pk
        //pk is the vertex of the adjacent triangle (the unique vertex that is not in the edge to be tested)

        //in legalize edge I need also the triangulation to call the function recursively and I need to insert triangles
        //I need the dag to insert new nodes

        if(adjacency0 != noAdjacentTriangle)
        {
            testEdge(triangulation, dag, totalTrianglesNumber, unsigned(adjacency0), v1v2Edge, v1, v2, point);
        }

        if(adjacency1 != noAdjacentTriangle)
        {
            testEdge(triangulation, dag, totalTrianglesNumber + 1, unsigned(adjacency1), v2v3Edge, point, v2, v3);
        }

        if(adjacency2 != noAdjacentTriangle)
        {
            testEdge(triangulation, dag, totalTrianglesNumber + 2, unsigned(adjacency2), v3v1Edge, v1, point, v3);
        }

    }

}

/**
 * @brief Adds node, triangle and adjacencies to the triangulation after inserting a new point
 * @param[in] triangulation: triangulation data structure
 * @param[in] dag: search data structure
 * @param[in] triangle index: index of new triangle to create
 * @param[in] parent index: index of parent triangle where the point is inserted
 * @param[in] p1: new triangle vertex 1
 * @param[in] p2: new triangle vertex 2
 * @param[in] p3: new triangle vertex 3
 * @param[in] adjacency0: triangle adjacent in V1V2
 * @param[in] adjacency1: triangle adjacent in V2V3
 * @param[in] adjacency2: triangle adjacent in V3V1
*/
void addElementToTriangulation(Triangulation& triangulation, DAG& dag,
                unsigned int index, unsigned int parentIndex,
                const cg3::Point2Dd& v1, const cg3::Point2Dd& v2, const cg3::Point2Dd& v3,
                int adjacency0, int adjacency1, int adjacency2)
{
    //add triangle to the triangulation
    triangulation.addTriangle(Triangle(v1, v2, v3));
    //add node to the dag
    dag.addNode(Node(index), parentIndex);
    //update adjacencies
    triangulation.addAdjacenciesForNewTriangle(index,
        adjacency0, adjacency1, adjacency2, parentIndex);
}

/**
 * @brief Adds node, triangle and adjacencies to the triangulation after inserting a new point
 * @param[in] triangulation: triangulation data structure
 * @param[in] dag: search data structure
 * @param[in] triangle index: index of new triangle to legalize
 * @param[in] adjacent index: index of adjacent triangle where the edge is illegal
 * @param[in] edge: index of illegal edge
 * @param[in] p1: new triangle vertex 1
 * @param[in] p2: new triangle vertex 2
 * @param[in] p3: new triangle vertex 3
*/
void testEdge(Triangulation& triangulation, DAG& dag,
              unsigned int triangle, unsigned int adjacent, unsigned int edge,
              const cg3::Point2Dd& v1, const cg3::Point2Dd& v2, const cg3::Point2Dd& v3)
{
    std::vector<Triangle>& triangles = triangulation.getTriangles();

    //opposite pk is the edge where the adjacent is adjacent to the triangle
    //by adding two to this number I can find pk, for example: oppositePk is 0 (V1V2), pk is V3 so 0 + 2 % 3 = 2 that is the index of V3

    int oppositePk = triangulation.findAdjacency(adjacent, triangle);
    unsigned int pkIndex = (unsigned(oppositePk) + 2) % dimension;

    //oppositepk is the edge of the neighbour where I can find the adjacency with the triangle
    //using this edge I can find pk that is the opposite point of this edge in the adjacent triangle

    const cg3::Point2Dd& pk = pkIndex == 0? triangles[adjacent].getV1() :
            (pkIndex == 1? triangles[adjacent].getV2() : triangles[adjacent].getV3());

    legalizeEdge(triangulation, dag, triangle, adjacent, v1, v2, v3, pk, edge, unsigned(oppositePk),
                 triangulation.getAdjacenciesFromTriangle(triangle), triangulation.getAdjacenciesFromTriangle(adjacent));
}

/**
 * @brief Adds node, triangle and adjacencies to the triangulation due to flip edge
 * @param[in] triangulation: triangulation data structure
 * @param[in] dag: search data structure
 * @param[in] triangle index: index of new triangle to legalize
 * @param[in] first parent index: index of first parent triangle with illegal edge
 * @param[in] second parent index: index of second parent triangle with illegal edge
 * @param[in] p1: new triangle vertex 1
 * @param[in] p2: new triangle vertex 2
 * @param[in] p3: new triangle vertex 3
 * @param[in] adjacency0: triangle adjacent in V1V2
 * @param[in] adjacency1: triangle adjacent in V2V3
 * @param[in] adjacency2: triangle adjacent in V3V1
*/
void addElementAfterFlip(Triangulation& triangulation, DAG& dag,
                         unsigned int index, unsigned int firstParentIndex, unsigned int secondParentIndex,
                         const cg3::Point2Dd& v1, const cg3::Point2Dd& v2, const cg3::Point2Dd& v3,
                         int adjacency0, int adjacency1, int adjacency2)
{
    //add triangle to the triangulation
    triangulation.addTriangle(Triangle(v1, v2, v3));
    //add node to the dag
    dag.addNode(Node(index), firstParentIndex, secondParentIndex);
    //update adjacencies
    triangulation.addAdjacenciesForNewTriangle(index,
        adjacency0, adjacency1, adjacency2, firstParentIndex, secondParentIndex);
}

}
