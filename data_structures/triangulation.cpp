#include "triangulation.h"

Triangulation::Triangulation() {}

Triangulation::Triangulation(const std::vector<Triangle> &triangles,
                             const std::vector<std::array<int, maxAdjacentTriangles> > &adjacencies)
    : triangles(triangles), adjacencies(adjacencies) {}

void Triangulation::addTriangle(const Triangle& triangle)
{
    triangles.push_back(triangle);
}

std::vector<Triangle> Triangulation::getTriangles() const
{
    return triangles;
}

// I assume that here the edge in common for triangle and neighbor is already flipped
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
    std::swap(adjacencies[triangle][adjacencyToChangeTriangle], adjacencies[triangle][triangleAdjNeighbor]);

    unsigned int unchangedAdjacencyNeighbor;
    unsigned int adjacencyToChangeNeighbor;

    //the edge that doesn't change has Vx as endpoint, so the adjacency doesn't change
    unchangedAdjacencyNeighbor = (neighborAdjTriangle + 1) % maxAdjacentTriangles;

    //3 - (0 + 1) = 3 - (0 + 2) = 3 - (1 + 2) the adjacency remaining
    adjacencyToChangeNeighbor = maxAdjacentTriangles - (neighborAdjTriangle + unchangedAdjacencyNeighbor);

    //swap due to edge flip
    std::swap(adjacencies[neighbor][adjacencyToChangeNeighbor], adjacencies[neighbor][neighborAdjTriangle]);

    //swap adjacencies belonging to each other
    std::swap(adjacencies[triangle][adjacencyToChangeTriangle], adjacencies[neighbor][adjacencyToChangeNeighbor]);

    //TODO: recursively modify adjacency for adjTriangleNeighbor and adjNeighborTriangle
    //TODO: find a condition to stop -exploit reusability
}


std::array<int, maxAdjacentTriangles> Triangulation::getAdjacenciesFromTriangle(const unsigned int& triangle)
{
    return adjacencies[triangle];
}

void Triangulation::clearDataStructure()
{
    //erase triangle from position 1 to position n - 1
    std::vector<Triangle>::iterator triangleIterator = triangles.begin() + 1;
    triangles.erase(triangleIterator, triangles.end());

    //erase adjacencies from position 1 to position n - 1
    std::vector<std::array<int, maxAdjacentTriangles>>::iterator adjacenciesIterator = adjacencies.begin() + 1;
    adjacencies.erase(adjacenciesIterator, adjacencies.end());

    //in this way we keep always the bounding triangle as first element
}

void Triangulation::addAdjacenciesForNewTriangle(const int &v1v2, const int &v2v3, const int &v3v1)
{
    std::array<int, maxAdjacentTriangles> newAdjacency = {v1v2, v2v3, v3v1};
    adjacencies.push_back(newAdjacency);
}

void Triangulation::addAdjacenciesForNewTriangle(const unsigned int &triangle, const int &v1v2, const int &v2v3, const int &v3v1, const unsigned int& old)
{
    addAdjacenciesForNewTriangle(v1v2, v2v3, v3v1);

    const unsigned int& length = adjacencies.size();

    //update old adjacencies
    if(v1v2 < length)
    {
        if(findOldAdjacency(v1v2, old) != -1)
        {
            adjacencies[v1v2][findOldAdjacency(v1v2, old)] = triangle;
        }
    }
    if(v2v3 < length)
    {
        if(findOldAdjacency(v2v3, old) != -1)
        {
            adjacencies[v2v3][findOldAdjacency(v2v3, old)] = triangle;
        }
    }
    if(v3v1 < length)
    {
        if(findOldAdjacency(v3v1, old) != -1)
        {
            adjacencies[v3v1][findOldAdjacency(v3v1, old)] = triangle;
        }
    }
}

unsigned int Triangulation::findOldAdjacency(const unsigned int &edge, const unsigned int& old)
{
    return adjacencies[edge][0] == old? 0 : (adjacencies[edge][1] == old? 1 : (adjacencies[edge][2] == old? 2 : -1));
}
