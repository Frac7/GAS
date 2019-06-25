#include "triangulation.h"

/**
 * @brief Default constructor
*/
Triangulation::Triangulation() {}

/**
 * @brief Creates a triangulation from triangles and adjacencies
 * @param[in] triangles: array of triangles of the triangulation
 * @param[in] adjacencies: array of adjacencies for triangles
*/
Triangulation::Triangulation(const std::vector<Triangle> &triangles,
                             const std::vector<std::array<int, maxAdjacentTriangles> > &adjacencies)
    : triangles(triangles), adjacencies(adjacencies) {}

/**
 * @brief Add a triangle to the triangulation
 * @param[in] triangle: triangle to add
*/
void Triangulation::addTriangle(const Triangle& triangle)
{
    triangles.push_back(triangle);
}

/**
 * @brief Returns the triangles of the triangulation
 * @return triangles: the array of triangles
*/
std::vector<Triangle>& Triangulation::getTriangles()
{
    return triangles;
}

/**
 * @brief Returns the adjacencies of the triangle
 * @param[in] triangle: the index of the triangle
 * @return adjacencies: the array containing the adjacencies for the triangle
*/
std::array<int, maxAdjacentTriangles>& Triangulation::getAdjacenciesFromTriangle(unsigned int triangle)
{
    return adjacencies[triangle];
}

/**
 * @brief Clears triangles and adjacencies but not the first triangle - bounding triangle
*/
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

/**
 * @brief Adds adjacencies for a new triangle
 * @param[in] v1v2: the adjacent triangle index in edge v1v2
 * @param[in] v2v3: the adjacent triangle index in edge v2v3
 * @param[in] v3v1: the adjacent triangle index in edge v3v1
*/
void Triangulation::addAdjacenciesForNewTriangle(int v1v2, int v2v3, int v3v1)
{
    adjacencies.push_back({v1v2, v2v3, v3v1});
}

/**
 * @brief Adds adjacencies for a new triangle and replace old adjacencies when flipping edge
 * @param[in] v1v2: the adjacent triangle index in edge v1v2
 * @param[in] v2v3: the adjacent triangle index in edge v2v3
 * @param[in] v3v1: the adjacent triangle index in edge v3v1
 * @param[in] old: the old triangle index that must be replaced in the adjacencies
 * @param[in] oldAdj: the old adjacent triangle index that must be replaced in the adjacencies
*/
void Triangulation::addAdjacenciesForNewTriangle(unsigned int triangle, int v1v2, int v2v3, int v3v1,
                                                 unsigned int old, unsigned int oldAdj)
{
    addAdjacenciesForNewTriangle(v1v2, v2v3, v3v1);

    int length = int(adjacencies.size());

    int adjacency = noAdjacentTriangle;

    //update old adjacencies and create new ones only if the adjacent triangles exists
    //in some cases the adjacent triangle doesn't exist yet

    if(v1v2 != noAdjacentTriangle && v1v2 < length)
    {
        adjacency = findAdjacency(unsigned(v1v2), old);
        if(adjacency != noAdjacentTriangle)
        {
            adjacencies[unsigned(v1v2)][unsigned(adjacency)] = int(triangle);
        }

        adjacency = findAdjacency(unsigned(v1v2), oldAdj);
        if(adjacency != noAdjacentTriangle)
        {
            adjacencies[unsigned(v1v2)][unsigned(adjacency)] = int(triangle);
        }
    }
    if(v2v3 != noAdjacentTriangle && v2v3 < length)
    {
        adjacency = findAdjacency(unsigned(v2v3), old);
        if(adjacency != noAdjacentTriangle)
        {
            adjacencies[unsigned(v2v3)][unsigned(adjacency)] = int(triangle);
        }

        adjacency = findAdjacency(unsigned(v2v3), oldAdj);
        if(adjacency != noAdjacentTriangle)
        {
            adjacencies[unsigned(v2v3)][unsigned(adjacency)] = int(triangle);
        }
    }
    if(v3v1 != noAdjacentTriangle && v3v1 < length)
    {
        adjacency = findAdjacency(unsigned(v3v1), old);
        if(adjacency != noAdjacentTriangle)
        {
            adjacencies[unsigned(v3v1)][unsigned(adjacency)] = int(triangle);
        }

        adjacency = findAdjacency(unsigned(v3v1), oldAdj);
        if(adjacency != noAdjacentTriangle)
        {
            adjacencies[unsigned(v3v1)][unsigned(adjacency)] = int(triangle);
        }
    }
}

/**
 * @brief Adds adjacencies for a new triangle and replace old adjacencies when adding a triangle
 * @param[in] v1v2: the adjacent triangle index in edge v1v2
 * @param[in] v2v3: the adjacent triangle index in edge v2v3
 * @param[in] v3v1: the adjacent triangle index in edge v3v1
 * @param[in] old: the old triangle index that must be replaced in the adjacencies
*/
void Triangulation::addAdjacenciesForNewTriangle(unsigned int triangle, int v1v2, int v2v3, int v3v1, unsigned int old)
{
    addAdjacenciesForNewTriangle(v1v2, v2v3, v3v1);

    int length = int(adjacencies.size());

    int adjacency = noAdjacentTriangle;

    //update old adjacencies and create new ones only if the adjacent triangles exists
    //in some cases the adjacent triangle doesn't exist yet

    if(v1v2 != noAdjacentTriangle && v1v2 < length)
    {
        adjacency = findAdjacency(unsigned(v1v2), old);
        if(adjacency != noAdjacentTriangle)
        {
            adjacencies[unsigned(v1v2)][unsigned(adjacency)] = int(triangle);
        }
    }
    if(v2v3 != noAdjacentTriangle && v2v3 < length)
    {
        adjacency = findAdjacency(unsigned(v2v3), old);
        if(adjacency != noAdjacentTriangle)
        {
            adjacencies[unsigned(v2v3)][unsigned(adjacency)] = int(triangle);
        }
    }
    if(v3v1 != noAdjacentTriangle && v3v1 < length)
    {
        adjacency = findAdjacency(unsigned(v3v1), old);
        if(adjacency != noAdjacentTriangle)
        {
            adjacencies[unsigned(v3v1)][unsigned(adjacency)] = int(triangle);
        }
    }
}

/**
 * @brief Finds the edge where the triangle is adjacent to its adjacent triangle
 * @param[in] triangle: the index to be tested
 * @param[in] adjacent: the adjacent triangle where the adjacency must be checked
 * @return adjacency: the edge where the triangle is adjacent to the adjacent triangle
*/
int Triangulation::findAdjacency(unsigned int triangle, unsigned int adjacent)
{
    //check if the triangle is adjacent in that edge with the adjacent
    if(adjacencies[triangle][v1v2Edge] == int(adjacent))
    {
        return v1v2Edge;
    }

    if(adjacencies[triangle][v2v3Edge] == int(adjacent))
    {
        return v2v3Edge;
    }

    if(adjacencies[triangle][v3v1Edge] == int(adjacent))
    {
        return v3v1Edge;
    }

    //return -1 if the adjacency was not found
    return noAdjacentTriangle;
}
