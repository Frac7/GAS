#include "drawabletriangle.h"

/**
 * @brief Initializes the drawable object by creating a triangle
 * @param[in] v1: the first vertex coordinate
 * @param[in] v2: the second vertex coordinate
 * @param[in] v3: the third vertex coordinate
*/
DrawableTriangle::DrawableTriangle(const cg3::Point2Dd &v1, const cg3::Point2Dd &v2, const cg3::Point2Dd &v3) :
    Triangle(v1, v2, v3) {}

/**
 * @brief Draws the triangle
*/
void DrawableTriangle::draw() const
{
    cg3::viewer::drawTriangle2D(v1, v2, v3, Qt::red, 1);
}

/**
 * @brief Calculates the triangle center
 * @return center: the triangle barycenter
*/
cg3::Pointd DrawableTriangle::sceneCenter() const
{
    //triangle barycenter
    cg3::Point2Dd center = getCenter();
    return cg3::Pointd(center.x(), center.y(), 0);
}

/**
 * @brief Calculates the distance between the farthest vertex and the barycenter
 * @return radius: the triangle radius
*/
double DrawableTriangle::sceneRadius() const
{
    cg3::Point2Dd center = getCenter();
    //the maximum distance from vertex to center is the scene radius
    return std::max(std::max(v1.dist(center), v2.dist(center)), v3.dist(center));
}
