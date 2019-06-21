#include "drawabletriangulation.h"

DrawableTriangulation::DrawableTriangulation(Triangulation& triangulation, DAG& dag, const cg3::Pointd& center, const double& radius) :
    center(center), radius(radius), triangulation(triangulation), dag(dag) {}
//parameters of the bounding triangle are passed because the triangulation is inside this polygon

void DrawableTriangulation::draw() const
{
    const std::vector<Triangle>& triangles = triangulation.getTriangles();
    const std::vector<Node>& nodes = dag.getNodeList();

    const unsigned int length = triangles.size();

    //draw each triangle of triangulation
    for(unsigned int i = 1; i < length; i++)
    {
        //ignore bounding triangle
        if(nodes[i].isLeaf())
        {
            cg3::viewer::drawPoint2D(triangles[i].getV1(), Qt::red, 5);
            cg3::viewer::drawPoint2D(triangles[i].getV2(), Qt::red, 5);
            cg3::viewer::drawPoint2D(triangles[i].getV3(), Qt::red, 5);

            cg3::viewer::drawLine2D(triangles[i].getV1(), triangles[i].getV2(), Qt::green, 1);
            cg3::viewer::drawLine2D(triangles[i].getV2(), triangles[i].getV3(), Qt::green, 1);
            cg3::viewer::drawLine2D(triangles[i].getV3(), triangles[i].getV1(), Qt::green, 1);
        }
    }
}

cg3::Pointd DrawableTriangulation::sceneCenter() const
{
    return center;
}

double DrawableTriangulation::sceneRadius() const
{
    return radius;
}
