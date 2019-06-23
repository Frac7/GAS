#include "drawablevoronoi.h"

DrawableVoronoi::DrawableVoronoi(Triangulation& triangulation, DAG& dag, const cg3::Pointd& center, const double& radius) :
    center(center), radius(radius), triangulation(triangulation), dag(dag) {}
//parameters of the bounding triangle are passed because the triangulation is inside this polygon

void DrawableVoronoi::draw() const
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
            cg3::Point2Dd circumCenter = triangles[i].getCircumcenter();
            cg3::viewer::drawPoint2D(circumCenter, Qt::yellow, 5);

            const std::array<int, maxAdjacentTriangles>& adjacencies = triangulation.getAdjacenciesFromTriangle(i);

            if(adjacencies[0] != noAdjacentTriangle)
            {
                cg3::viewer::drawLine2D(circumCenter, triangles[adjacencies[0]].getCircumcenter(), Qt::blue, 1);
            }
            if(adjacencies[1] != noAdjacentTriangle)
            {
                cg3::viewer::drawLine2D(circumCenter, triangles[adjacencies[1]].getCircumcenter(), Qt::blue, 1);
            }
            if(adjacencies[2] != noAdjacentTriangle)
            {
                cg3::viewer::drawLine2D(circumCenter, triangles[adjacencies[2]].getCircumcenter(), Qt::blue, 1);
            }
        }
    }
}

cg3::Pointd DrawableVoronoi::sceneCenter() const
{
    return center;
}

double DrawableVoronoi::sceneRadius() const
{
    return radius;
}
