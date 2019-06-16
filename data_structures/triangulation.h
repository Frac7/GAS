#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <cg3/geometry/2d/point2d.h>
#include "triangle.h"

const unsigned int maxAdjacentTriangles = 3;

const int noAdjacentTriangle = -1;

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

    std::array<int, maxAdjacentTriangles>& getAdjacenciesFromTriangle(const unsigned int &triangle);

    void clearDataStructure();

    void addAdjacenciesForNewTriangle(const int& v1v2, const int& v2v3, const int& v3v1);
    void addAdjacenciesForNewTriangle(const unsigned int& triangle, const int& v1v2, const int& v2v3, const int& v3v1, const unsigned int& old);

    unsigned int findAdjacency(const unsigned int& triangle, const unsigned int& adjacent);

protected:
    //triangles of the triangulation
    std::vector<Triangle> triangles;

    //adjacency of triangles
    std::vector<std::array<int, maxAdjacentTriangles> > adjacencies;
};

#endif // TRIANGULATION_H
