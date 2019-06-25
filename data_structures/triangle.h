#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cg3/geometry/2d/point2d.h>

/**
 * @brief Triangle
 *
 * The triangle data structure is implemented by storing inserted points for the algorithm: it contains the 3 vertices in counter-clockwise order.
 * This class implements getters and setters and two methods, one for the barycenter (used for the drawable) and one for the circumcenter (used for Voronoi).
 */
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

    //setters
    void setV1(const cg3::Point2Dd &value);
    void setV2(const cg3::Point2Dd &value);
    void setV3(const cg3::Point2Dd &value);

protected:

    cg3::Point2Dd v1;
    cg3::Point2Dd v2;
    cg3::Point2Dd v3;
};

#endif // TRIANGLE_H
