#ifndef DRAWABLETRIANGULATION_H
#define DRAWABLETRIANGULATION_H

#include <data_structures/dag.h>
#include <data_structures/triangulation.h>

#include <cg3/viewer/interfaces/drawable_object.h>
#include <cg3/viewer/renderable_objects/2d/renderable_objects2d.h>

class DrawableTriangulation : public cg3::DrawableObject
{
public:
    DrawableTriangulation(Triangulation& triangulation, DAG& dag, const cg3::Pointd& center, const double& radius);

    void draw() const;
    cg3::Pointd sceneCenter() const;
    double sceneRadius() const;

private:
    const cg3::Pointd center;
    const double radius;

    Triangulation& triangulation;
    DAG& dag;
};

#endif // DRAWABLETRIANGULATION_H
