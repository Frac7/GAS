#ifndef DRAWABLEVORONOI_H
#define DRAWABLEVORONOI_H

#include <data_structures/dag.h>
#include <data_structures/triangulation.h>

#include <cg3/viewer/interfaces/drawable_object.h>
#include <cg3/viewer/renderable_objects/2d/renderable_objects2d.h>

class DrawableVoronoi : public cg3::DrawableObject
{
public:
    DrawableVoronoi(Triangulation& triangulation, DAG& dag, const cg3::Pointd& center, const double radius);

    void draw() const;
    cg3::Pointd sceneCenter() const;
    double sceneRadius() const;

private:
    const cg3::Pointd center;
    const double radius;

    Triangulation& triangulation;
    DAG& dag;
};

#endif // DRAWABLEVORONOI_H
