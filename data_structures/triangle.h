#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cg3/geometry/2d/point2d.h>

class Triangle
{
public:
    //create new triangle from three points
    Triangle(const unsigned int& v1, const unsigned int& v2, const unsigned int& v3);

    //getters and setters
    unsigned int getV1() const;
    void setV1(const unsigned int& value);

    unsigned int getV2() const;
    void setV2(const unsigned int& value);

    unsigned int getV3() const;
    void setV3(const unsigned int& value);
private:
    //topmost point
    unsigned int v1;
    //leftmost point
    unsigned int v2;
    //rightmost point
    unsigned int v3;
};

#endif // TRIANGLE_H
