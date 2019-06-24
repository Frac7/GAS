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
    double aX2 = std::pow(v1.x(), 2);
    double bX2 = std::pow(v2.x(), 2);
    double cX2 = std::pow(v3.x(), 2);

    double aY2 = std::pow(v1.y(), 2);
    double bY2 = std::pow(v2.y(), 2);
    double cY2 = std::pow(v3.y(), 2);

    double aSum = aX2 + aY2;
    double bSum = bX2 + bY2;
    double cSum = cX2 + cY2;

    double xNum = aSum * (v2.y() - v3.y()) + bSum * (v3.y() - v1.y()) + cSum * (v1.y() - v2.y());
    double yNum = aSum * (v3.x() - v2.x()) + bSum * (v1.x() - v3.x()) + cSum * (v2.x() - v1.x());

    double d = 2 * (v1.x() * (v2.y() - v3.y()) + v2.x() * (v3.y() - v1.y()) + v3.x() * (v1.y() - v2.y()));

    return cg3::Point2Dd(xNum / d, yNum / d);
}

/**
 * @brief Set the first vertex
 * @param[in] value: the vertex
 */
void Triangle::setV1(const cg3::Point2Dd &value)
{
    v1 = value;
}

/**
 * @brief Set the second vertex
 * @param[in] value: the vertex
 */
void Triangle::setV2(const cg3::Point2Dd &value)
{
    v2 = value;
}

/**
 * @brief Set the third vertex
 * @param[in] value: the vertex
 */
void Triangle::setV3(const cg3::Point2Dd &value)
{
    v3 = value;
}
