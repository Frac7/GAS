#include "triangle.h"

/**
 * @brief Creates a node from 3 vertices
 * @param[in] v1: rightmost vertex
 * @param[in] v2: topmost vertex
 * @param[in] v3: leftmost vertex
*/
Triangle::Triangle(const cg3::Point2Dd& v1, const cg3::Point2Dd& v2, const cg3::Point2Dd& v3)
    : v1(v1), v2(v2), v3(v3) {}

/**
 * @brief Returns the first vertex
 * @return v1: vertex 1
*/
cg3::Point2Dd Triangle::getV1() const
{
    return v1;
}

/**
 * @brief Returns the second vertex
 * @return v2: vertex 2
*/
cg3::Point2Dd Triangle::getV2() const
{
    return v2;
}

/**
 * @brief Returns the third vertex
 * @return v3: vertex 3
*/
cg3::Point2Dd Triangle::getV3() const
{
    return v3;
}

/**
 * @brief Returns the center of the triangle
 * @return center: the barycenter of the triangle
*/
cg3::Point2Dd Triangle::getCenter() const
{
    return (v1 + v2 + v3) / 3;
}

/**
 * @brief Returns the center of the triangle
 * @return center: the circumcenter of the triangle
*/
cg3::Point2Dd Triangle::getCircumcenter() const
{
    double aX2 = v1.x() * v1.x();
    double bX2 = v2.x() * v2.x();
    double cX2 = v3.x() * v3.x();

    double aY2 = v1.y() * v1.y();
    double bY2 = v2.y() * v2.y();
    double cY2 = v3.y() * v3.y();

    double aSum = aX2 + aY2;
    double bSum = bX2 + bY2;
    double cSum = cX2 + cY2;

    double xNum = aSum * (v2.y() - v3.y()) + bSum * (v3.y() - v1.y()) + cSum * (v1.y() - v2.y());
    double yNum = aSum * (v3.x() - v2.x()) + bSum * (v1.x() - v3.x()) + cSum * (v2.x() - v1.x());

    double d = 2 * (v1.x() * (v2.y() - v3.y()) + v2.x() * (v3.y() - v1.y()) + v3.x() * (v1.y() - v2.y()));

    return cg3::Point2Dd(xNum / d, yNum / d);
}
