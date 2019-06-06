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

/* I assume that here the edge in common for triangle and neighbor is already flipped */
void Triangulation::modifyAdjacency(const unsigned int& triangle,
                     const unsigned int& neighbor,
                     const unsigned int& triangleAdjNeighbor,
                     const unsigned int& neighborAdjTriangle)
{
    unsigned int unchangedAdjacencyTriangle;
    unsigned int adjacencyToChangeTriangle;

    //the edge that doesn't change has as endpoint
    //Vx | x doesn't belong to triangleAdjNeighbor, triangleAdjNeighbor + 1 % maxAdjacentTriangles
    unchangedAdjacencyTriangle = (triangleAdjNeighbor + 1) % maxAdjacentTriangles;

    //3 - (0 + 1), 3 - (0 + 2), 3 - (1 + 2) the remaining adjacency
    adjacencyToChangeTriangle = maxAdjacentTriangles - (triangleAdjNeighbor + unchangedAdjacencyTriangle);
    //due to the flip, this edge picks the adjacency of triangleAdjNeighbor -neighbor triangle changed due to flip

    //swap due to edge flip
    std::swap(adjacency[triangle][adjacencyToChangeTriangle], adjacency[triangle][triangleAdjNeighbor]);

    unsigned int unchangedAdjacencyNeighbor;
    unsigned int adjacencyToChangeNeighbor;

    //the edge that doesn't change has Vx as endpoint, so the adjacency doesn't change
    unchangedAdjacencyNeighbor = (neighborAdjTriangle + 1) % maxAdjacentTriangles;

    //3 - (0 + 1) = 3 - (0 + 2) = 3 - (1 + 2) the adjacency remaining
    adjacencyToChangeNeighbor = maxAdjacentTriangles - (neighborAdjTriangle + unchangedAdjacencyNeighbor);

    //swap due to edge flip
    std::swap(adjacency[neighbor][adjacencyToChangeNeighbor], adjacency[neighbor][neighborAdjTriangle]);

    //swap adjacencies belonging to each other
    std::swap(adjacency[triangle][adjacencyToChangeTriangle], adjacency[neighbor][adjacencyToChangeNeighbor]);

    //TODO: recursively modify adjacency for adjTriangleNeighbor and adjNeighborTriangle
    //TODO: find a condition to stop -exploit reusability
}


std::array<int, maxAdjacentTriangles> Triangulation::getAdjacencies(const unsigned int& triangle)
{
    return adjacency[triangle];
}

void Triangulation::clearDataStructure()
{
    triangles.clear();
    adjacency.clear();
}
