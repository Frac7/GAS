#ifndef DRAWABLETRIANGLE_H
#define DRAWABLETRIANGLE_H

#include <data_structures/triangle.h>

#include <cg3/viewer/interfaces/drawable_object.h>
#include <cg3/viewer/renderable_objects/2d/renderable_objects2d.h>

/**
 * @brief DrawableTriangle
 *
 * This class inherits from Triangle and DrawableObject.
 * This class is used only for drawing the bounding triangle, it is initialized with the constant coordinates declared in the manager.
 */
class DrawableTriangle : public Triangle, public cg3::DrawableObject
{
public:
    DrawableTriangle(const cg3::Point2Dd& v1, const cg3::Point2Dd& v2, const cg3::Point2Dd& v3);

    void draw() const;
    cg3::Pointd sceneCenter() const;
    double sceneRadius() const;
};

#endif // DRAWABLETRIANGLE_H
