#include "triangulation.h"

Triangulation::Triangulation() {}

Triangulation::Triangulation(const std::vector<Triangle> &triangles,
                             const std::vector<std::array<int, maxAdjacentTriangles> > &adjacencies)
    : triangles(triangles), adjacencies(adjacencies) {}

void Triangulation::addTriangle(const Triangle& triangle)
{
    triangles.push_back(triangle);
}

std::vector<Triangle>& Triangulation::getTriangles()
{
    return triangles;
}

std::array<int, maxAdjacentTriangles>& Triangulation::getAdjacenciesFromTriangle(const unsigned int& triangle)
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
    adjacencies.push_back({v1v2, v2v3, v3v1});
}

void Triangulation::addAdjacenciesForNewTriangle(const unsigned int &triangle, const int &v1v2, const int &v2v3, const int &v3v1, const unsigned int& old)
{
    addAdjacenciesForNewTriangle(v1v2, v2v3, v3v1);

    const unsigned int& length = adjacencies.size();

    //update old adjacencies
    if(v1v2 < length)
    {
        const int& adjacency = findAdjacency(v1v2, old);
        if(adjacency != -1)
        {
            adjacencies[v1v2][adjacency] = triangle;
        }
    }
    if(v2v3 < length)
    {
        const int& adjacency = findAdjacency(v2v3, old);
        if(adjacency)
        {
            adjacencies[v2v3][adjacency] = triangle;
        }
    }
    if(v3v1 < length)
    {
        const int& adjacency = findAdjacency(v3v1, old);
        if(adjacency != -1)
        {
            adjacencies[v3v1][adjacency] = triangle;
        }
    }
}

unsigned int Triangulation::findAdjacency(const unsigned int &triangle, const unsigned int& adjacent)
{
    return adjacencies[triangle][0] == adjacent? 0 : (adjacencies[triangle][1] == adjacent? 1 : (adjacencies[triangle][2] == adjacent? 2 : -1));
}
