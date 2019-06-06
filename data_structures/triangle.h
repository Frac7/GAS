#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cg3/geometry/2d/point2d.h>

class Triangle
{
public:
    Triangle();

    //create new triangle from three points
    Triangle(const cg3::Point2Dd& v1, const cg3::Point2Dd& v2, const cg3::Point2Dd& v3);

    //getters and setters
    cg3::Point2Dd getV1() const;
    void setV1(const cg3::Point2Dd& value);

    cg3::Point2Dd getV2() const;
    void setV2(const cg3::Point2Dd& value);

    cg3::Point2Dd getV3() const;
    void setV3(const cg3::Point2Dd& value);

    cg3::Point2Dd getCenter() const;

protected:
    //topmost point
    cg3::Point2Dd v1;
    //leftmost point
    cg3::Point2Dd v2;
    //rightmost point
    cg3::Point2Dd v3;
};

#endif // TRIANGLE_H
