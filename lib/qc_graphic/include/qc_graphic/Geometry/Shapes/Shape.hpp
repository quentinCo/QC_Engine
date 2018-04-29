#pragma once

#include <glm/vec3.hpp>

#include <qc_graphic/Render/VertexArray.hpp>

#include <qc_graphic/Geometry/Vertex.hpp>

namespace qc_graphic
{

namespace geometry
{

namespace shape
{

class Shape
{
public:
    Shape()
    {};

    Shape(const std::vector<Vertex>& vertices, const std::vector<int>& indices);
    
    ~Shape()
    {};

    Shape(const Shape& o)               = delete;
    Shape& operator= (const Shape& o)   = delete;

    Shape(Shape&& o)                    = default;
    Shape& operator= (Shape&& o)        = default;


    const std::vector<Vertex>& getVertices()
    {
        return vertices;
    }

    const std::vector<int>& getIndices()
    {
        return indices;
    }

    const qc_graphic::render::VertexArray& getVao()
    {
        return vao;
    }

    // TODO 
    /*
        If set functions are required, think to create shape copy before
        if there is more than one instance which point on it.
    */


private:
    std::vector<Vertex>             vertices;
    std::vector<int>                indices;

    qc_graphic::render::VertexArray vao;
};


}// shape

}// geometry

}// qc_graphic