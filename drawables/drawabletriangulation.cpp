#include "drawabletriangulation.h"

/**
 * @brief Initializes the drawable object
 * @param[in] triangulation: array of triangles and adjacencies
 * @param[in] dag: the search data structure
 * @param[in] center: the center of the triangulation - center of the bounding triangle
 * @param[in] radius: the radius of the triangulation - radius of the bounding triangle
*/
DrawableTriangulation::DrawableTriangulation(Triangulation& triangulation, DAG& dag, const cg3::Pointd& center, const double radius) :
    center(center), radius(radius), triangulation(triangulation), dag(dag) {}
//parameters of the bounding triangle are passed because the triangulation is inside this polygon

/**
 * @brief Draws the triangulation
 *
 * This method draws only triangles contained in leaves, it draws green lines for the edges and red points for the vertices.
*/
void DrawableTriangulation::draw() const
{
    const std::vector<Triangle>& triangles = triangulation.getTriangles();
    const std::vector<Node>& nodes = dag.getNodeList();

    unsigned int length = unsigned(triangles.size());

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

/**
 * @brief Returns the triangulation center
 * @return center: the bounding triangle barycenter
*/
cg3::Pointd DrawableTriangulation::sceneCenter() const
{
    return center;
}

/**
 * @brief Return the radius of the triangulation
 * @return radius: the bounding triangle radius
*/
double DrawableTriangulation::sceneRadius() const
{
    return radius;
}
