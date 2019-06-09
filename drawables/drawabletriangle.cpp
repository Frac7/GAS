#include "drawabletriangle.h"

DrawableTriangle::DrawableTriangle(const cg3::Point2Dd &v1, const cg3::Point2Dd &v2, const cg3::Point2Dd &v3) :
    Triangle(v1, v2, v3) {}

void DrawableTriangle::draw() const
{
    //draw the triangle with points and lines
    cg3::viewer::drawTriangle2D(v1, v2, v3, Qt::red, 1);
}

cg3::Pointd DrawableTriangle::sceneCenter() const
{
    //triangle barycenter
    cg3::Point2Dd center = getCenter();
    return cg3::Pointd(center.x(), center.y(), 0);
}

double DrawableTriangle::sceneRadius() const
{
    cg3::Point2Dd center = getCenter();
    //the maximum distance from vertex to center is the scene radius
    return std::max(std::max(v1.dist(center), v2.dist(center)), v3.dist(center));
}
