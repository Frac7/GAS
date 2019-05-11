#include "triangulation.h"

void Triangulation::addPoint(const cg3::Point2Dd& point)
{
    points.push_back(point);
}

void Triangulation::addTriangle(const Triangle& triangle)
{
    triangles.push_back(triangle);
}

void Triangulation::removePoint(const cg3::Point2Dd& point, int position)
{
    //points.erase(point);
}

void Triangulation::removeTriangle(const Triangle& triangle, int position)
{
    //triangles.erase(triangle);
}

std::vector<cg3::Point2Dd> Triangulation::getPoints() const
{
    return points;
}

std::vector<Triangle> Triangulation::getTriangles() const
{
    return triangles;
}
