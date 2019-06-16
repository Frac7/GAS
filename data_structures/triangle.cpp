#include "triangle.h"

Triangle::Triangle(const cg3::Point2Dd& v1, const cg3::Point2Dd& v2, const cg3::Point2Dd& v3)
    : v1(v1), v2(v2), v3(v3) {}

cg3::Point2Dd Triangle::getV1() const
{
    return v1;
}

cg3::Point2Dd Triangle::getV2() const
{
    return v2;
}

cg3::Point2Dd Triangle::getV3() const
{
    return v3;
}

cg3::Point2Dd Triangle::getCenter() const
{
    return (v1 + v2 + v3) / 3;
}
