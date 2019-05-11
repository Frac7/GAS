#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <cg3/geometry/2d/point2d.h>
#include "triangle.h"

class Triangulation
{
public:

    //add a point to the triangulation
    void addPoint(const cg3::Point2Dd& point);
    //remove a point from the triangulation
    void removePoint(const cg3::Point2Dd& point, int position);

    //add a triangle to the triangulation
    void addTriangle(const Triangle& triangle);
    //remove a triangle from the triangulation
    void removeTriangle(const Triangle& triangle, int position);

    //get points
    std::vector<cg3::Point2Dd> getPoints() const;
    //get triangles
    std::vector<Triangle> getTriangles() const;

    /*PERSONAL: I think that setters are not needed since this is a randomized incremental approach...*/

private:
    //all the input points for the triangulation
    std::vector<cg3::Point2Dd> points;
    //triangles of the triangulation
    std::vector<Triangle> triangles;
};

#endif // TRIANGULATION_H
