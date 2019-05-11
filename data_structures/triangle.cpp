#include "triangle.h"

Triangle::Triangle(const unsigned int& v1, const unsigned int& v2, const unsigned int& v3)
    : v1(v1), v2(v2), v3(v3) {}
unsigned int Triangle::getV1() const
{
    return v1;
}

void Triangle::setV1(const unsigned int& value)
{
    v1 = value;
}

unsigned int Triangle::getV2() const
{
    return v2;
}

void Triangle::setV2(const unsigned int& value)
{
    v2 = value;
}

unsigned int Triangle::getV3() const
{
    return v3;
}

void Triangle::setV3(const unsigned int& value)
{
    v3 = value;
}
