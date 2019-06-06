#include "drawabletriangle.h"

DrawableTriangle::DrawableTriangle(const cg3::Point2Dd &v1, const cg3::Point2Dd &v2, const cg3::Point2Dd &v3) :
    Triangle(v1, v2, v3) {}

void DrawableTriangle::draw() const
{
    //draw the triangle with points and lines
    drawPoints();
    drawLines();
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

void DrawableTriangle::drawPoints() const
{
    //draw 3 vertices
    cg3::viewer::drawPoint2D(v1, Qt::black);
    cg3::viewer::drawPoint2D(v2, Qt::black);
    cg3::viewer::drawPoint2D(v3, Qt::black);
}

void DrawableTriangle::drawLines() const
{
    //draw 3 lines
    cg3::viewer::drawLine2D(v1, v2, Qt::black);
    cg3::viewer::drawLine2D(v2, v3, Qt::black);
    cg3::viewer::drawLine2D(v3, v1, Qt::black);
}
