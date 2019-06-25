#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <cg3/geometry/2d/point2d.h>
#include "triangle.h"

const unsigned int maxAdjacentTriangles = 3;

const int noAdjacentTriangle = -1;

const unsigned int v1v2Edge = 0;
const unsigned int v2v3Edge = 1;
const unsigned int v3v1Edge = 2;

/**
 * @brief Triangulation: triangles and adjacencies
 *
 * The triangulation data structure is implemented using a vector of triangles and a vector for triangle adjacencies:
 * each position of the latter corresponds to the position of the represented triangle in triangle vector and each element is an array of 3 positions,
 * each of them storing the adjacency as integer index of corresponding triangle.
 * The first location is the edge V1V2, the second is the edge V2V3 and the third is the edge V3V1.
 *
 * This class implements getters and methos for adding triangles and adjacencies;
 * in particular, as far as adjacencies are concerned, there are 3 overload of this method:
 * there is one method that picks only the 3 adjacencies for the considered triangle and stores them;
 * there is a method that picks the 3 adjacencies and the triangle that generates the new 3 triangles;
 * the third overload picks two old parent triangle and it is used when the edge legalization produces two new triangles.
 * There is also another method that returns the edge where two triangles taken in input are adjacent
 * and a method for clearing the data structure without leaving the bounding triangle.
 */
class Triangulation
{
public:
    Triangulation();
    Triangulation(const std::vector<Triangle>& triangles,
                  const std::vector<std::array<int, maxAdjacentTriangles>>& adjacencies);

    //add a triangle to the triangulation
    void addTriangle(const Triangle& triangle);

    //get triangles
    std::vector<Triangle>& getTriangles();

    std::array<int, maxAdjacentTriangles>& getAdjacenciesFromTriangle(const unsigned int triangle);

    void clearDataStructure();

    void addAdjacenciesForNewTriangle(const int v1v2, const int v2v3, const int v3v1);
    void addAdjacenciesForNewTriangle(const unsigned int triangle, const int v1v2, const int v2v3, const int v3v1,
                                      const unsigned int old);
    void addAdjacenciesForNewTriangle(const unsigned int triangle, const int v1v2, const int v2v3, const int v3v1,
                                      const unsigned int old, const unsigned int oldAdj);

    int findAdjacency(const unsigned int triangle, const unsigned int adjacent);

protected:
    //triangles of the triangulation
    std::vector<Triangle> triangles;

    //adjacency of triangles
    std::vector<std::array<int, maxAdjacentTriangles> > adjacencies;
};

#endif // TRIANGULATION_H
