#include "drawabletriangulation.h"

DrawableTriangulation::DrawableTriangulation(const Triangulation& triangulation, const cg3::Pointd& center, const double& radius) :
    center(center), radius(radius), triangulation(triangulation) {}
//parameters of the bounding triangle are passed because the triangulation is inside this polygon

void DrawableTriangulation::draw() const
{
    const std::vector<Triangle>& triangles = triangulation.getTriangles();
    const unsigned int length = triangles.size();

    //draw each triangle of triangulation
    for(unsigned int i = 1; i < length; i++)
    {
        //ignore bounding triangle
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
