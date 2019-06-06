#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <cg3/geometry/2d/point2d.h>
#include "triangle.h"

const unsigned int maxAdjacentTriangles = 3;

class Triangulation
{
public:

    //add a triangle to the triangulation
    void addTriangle(const Triangle& triangle);
    //remove a triangle from the triangulation
    void removeTriangle(const std::vector<Triangle>::iterator position);

    //get triangles
    std::vector<Triangle> getTriangles() const;

    /* I think that setters are not needed since this is a randomized incremental approach... */

    //after and edge is flipped, the 2 triangles sharing that edge change adjacency
    void modifyAdjacency(const unsigned int& triangle,
                         const unsigned int& neighbor,
                         const unsigned int& triangleAdjNeighbor,
                         const unsigned int& neighborAdjTriangle);
    //triangleAdjNeighbor indicates index from 0 to 2 to find the adjacet neighbor
    //the same is for neighbor

    std::array<int, maxAdjacentTriangles> getAdjacencies(const unsigned int &triangle);

    void clearDataStructure();

private:
    //triangles of the triangulation
    std::vector<Triangle> triangles;

    //adjacency of triangles
    std::vector<std::array<int, maxAdjacentTriangles> > adjacency;
};

#endif // TRIANGULATION_H
