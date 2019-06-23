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
cg3::Point2Dd& Triangle::getV1()
{
    return v1;
}

/**
 * @brief Returns the second vertex
 * @return v2: vertex 2
*/
cg3::Point2Dd& Triangle::getV2()
{
    return v2;
}

/**
 * @brief Returns the third vertex
 * @return v3: vertex 3
*/
cg3::Point2Dd& Triangle::getV3()
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
    double x, y;

    cg3::Point2Dd v1v2EdgeMiddle((v1.x() + v2.x()) / 2, (v1.y() + v2.y()) / 2);
    double v1v2EdgeSlope = ((v2.y() - v1.y()) / (v2.x() - v1.x()));

    double xMinusY = (v1v2EdgeMiddle.y() + (-v1v2EdgeSlope * -v1v2EdgeMiddle.x())) / v1v2EdgeSlope;

    cg3::Point2Dd v2v3EdgeMiddle((v2.x() + v3.x()) / 2, (v2.y() + v3.y()) / 2);
    double v2v3EdgeSlope = ((v3.y() - v2.y()) / (v3.x() - v2.x()));

    double xPlusY = (v2v3EdgeMiddle.y() + (-v2v3EdgeSlope * -v2v3EdgeMiddle.x())) / v2v3EdgeSlope;

    x = xMinusY + xPlusY;
    y = v1v2EdgeMiddle.y() - (v1v2EdgeSlope * (x - v1v2EdgeMiddle.x()));

    return cg3::Point2Dd(x, y);
}
