#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cg3/geometry/2d/point2d.h>

class Triangle
{
public:
    //create new triangle from three points
    Triangle(const cg3::Point2Dd& v1, const cg3::Point2Dd& v2, const cg3::Point2Dd& v3);

    //getters
    cg3::Point2Dd getV1() const;
    cg3::Point2Dd getV2() const;
    cg3::Point2Dd getV3() const;

    cg3::Point2Dd getCenter() const;

    cg3::Point2Dd getCircumcenter() const;

protected:

    cg3::Point2Dd v1;
    cg3::Point2Dd v2;
    cg3::Point2Dd v3;
};

#endif // TRIANGLE_H
