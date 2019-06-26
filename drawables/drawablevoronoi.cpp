#include "drawablevoronoi.h"

/**
 * @brief Initializes the drawable object
 * @param[in] triangulation: array of triangles and adjacencies
 * @param[in] dag: the search data structure
 * @param[in] center: the center of the triangulation - center of the bounding triangle
 * @param[in] radius: the radius of the triangulation - radius of the bounding triangle
*/
DrawableVoronoi::DrawableVoronoi(Triangulation& triangulation, DAG& dag, const cg3::Pointd& center, double radius) :
    center(center), radius(radius), triangulation(triangulation), dag(dag) {}
//parameters of the bounding triangle are passed because the triangulation is inside this polygon

/**
 * @brief Draws the Voronoi diagram
 *
 * This method draws only triangles contained in leaves, it draws blue lines for the edges and yellow points for the circumcenters.
*/
void DrawableVoronoi::draw() const
{
    std::vector<Triangle>& triangles = triangulation.getTriangles();
    std::vector<Node>& nodes = dag.getNodeList();

    unsigned int length = unsigned(triangles.size());

    //draw each triangle of triangulation
    for(unsigned int i = 1; i < length; i++)
    {
        //ignore bounding triangle
        if(nodes[i].isLeaf())
        {
            //draw circumcenter
            cg3::Point2Dd circumCenter = triangles[i].getCircumcenter();
            cg3::viewer::drawPoint2D(circumCenter, Qt::yellow, 5);

            //draw line from the circumcenter to each circumcenter of adjacent triangles
            const std::array<int, maxAdjacentTriangles>& adjacencies = triangulation.getAdjacenciesFromTriangle(i);

            if(adjacencies[v1v2Edge] != noAdjacentTriangle)
            {
                cg3::viewer::drawLine2D(circumCenter, triangles[unsigned(adjacencies[v1v2Edge])].getCircumcenter(), Qt::blue, 1);
            }
            if(adjacencies[v2v3Edge] != noAdjacentTriangle)
            {
                cg3::viewer::drawLine2D(circumCenter, triangles[unsigned(adjacencies[v2v3Edge])].getCircumcenter(), Qt::blue, 1);
            }
            if(adjacencies[v3v1Edge] != noAdjacentTriangle)
            {
                cg3::viewer::drawLine2D(circumCenter, triangles[unsigned(adjacencies[v3v1Edge])].getCircumcenter(), Qt::blue, 1);
            }
        }
    }
}

/**
 * @brief Returns the triangulation center
 * @return center: the bounding triangle barycenter
*/
cg3::Pointd DrawableVoronoi::sceneCenter() const
{
    return center;
}

/**
 * @brief Return the radius of the triangulation
 * @return radius: the bounding triangle radius
*/
double DrawableVoronoi::sceneRadius() const
{
    return radius;
}
