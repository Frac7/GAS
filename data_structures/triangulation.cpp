#include "triangulation.h"

void Triangulation::addTriangle(const Triangle& triangle)
{
    triangles.push_back(triangle);
}

void Triangulation::removeTriangle(const std::vector<Triangle>::iterator position)
{
    triangles.erase(position);
}

std::vector<Triangle> Triangulation::getTriangles() const
{
    return triangles;
}

void Triangulation::modifyAdjacency(unsigned int& triangle,
                     unsigned int& neighbor,
                     const unsigned int& triangleAdjNeighbor,
                     const unsigned int& neighborAdjTriangle)
{
    modifyTriangleAdjacency(triangle, neighbor, triangleAdjNeighbor);
    modifyTriangleAdjacency(neighbor, triangle, neighborAdjTriangle);
}

void Triangulation::modifyTriangleAdjacency(unsigned int& triangle,
                                            unsigned int& neighbor,
                                            const unsigned int& adjacencyIndex)
{
    int tmp = -1;

    //resulting edge from flipping
    unsigned int newStartingPoint = (adjacencyIndex + 1) % maxAdjacentTriangles;
    //preparing for swap
    tmp = adjacency[triangle][newStartingPoint];
    adjacency[triangle][newStartingPoint] = neighbor;

    //the edge that doesn't change has Vx as endpoint, so the adjacency doesn't change
    unsigned int unchangedAdjacency = (adjacencyIndex - 1) % maxAdjacentTriangles;

    //3 - (0 + 1) = 3 - (0 + 2) = 3 - (1 + 2) the adjacency remaining
    unsigned int adjacencyToChange = maxAdjacentTriangles - (newStartingPoint + unchangedAdjacency);
    //swap (here there was neighbor)
    adjacency[triangle][adjacencyToChange] = tmp;

    //recursively modify adjacency for neighbor triangle
    //modifyTriangleAdjacency(tmp, triangle, index from 0 to 2 for tmp->triangle adjacency);
    /*
     * TODO: perform some checks to identify adjacency index using common points
     */
}
