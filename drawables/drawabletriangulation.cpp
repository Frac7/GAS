#include "drawabletriangulation.h"

DrawableTriangulation::DrawableTriangulation(const cg3::Pointd& center, const double& radius) :
    Triangulation (), center(center), radius(radius) {}

//parameters of the bounding triangle are passed because the triangulation is inside this polygon

DrawableTriangulation::DrawableTriangulation(const std::vector<Triangle>& triangles,
                                             const std::vector<std::array<int, maxAdjacentTriangles>>& adjacencies,
                                             const cg3::Pointd &center,
                                             const double& radius) :
    Triangulation (triangles, adjacencies), center(center), radius(radius) {}

//TODO: implement method for updating the triangulation
void DrawableTriangulation::draw() const
{
    unsigned int length = triangles.size();
    //draw each triangle of triangulation
    for(unsigned int i = 0; i < length; i++)
    {
        cg3::viewer::drawTriangle2D(triangles[i].getV1(), triangles[i].getV2(), triangles[i].getV3(), Qt::black);
    }
}

cg3::Pointd DrawableTriangulation::sceneCenter() const
{
    return center;
}

double DrawableTriangulation::sceneRadius() const
{
    return radius;
}
