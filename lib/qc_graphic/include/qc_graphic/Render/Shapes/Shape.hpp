#pragma once

#include <glm/vec3.hpp>

#include <qc_graphic/Render/VertexArray.hpp>

#include <qc_graphic/Geometry/Vertex.hpp>

namespace qc_graphic
{

namespace render
{

namespace shape
{

class Shape
{
public:
    Shape()
    {};

    Shape(const std::vector<geometry::Vertex>& vertices, const std::vector<int>& indices);
    
    ~Shape()
    {};

    Shape(const Shape& o)               = delete;
    Shape& operator= (const Shape& o)   = delete;

    Shape(Shape&& o)                    = default;
    Shape& operator= (Shape&& o)        = default;


    const std::vector<geometry::Vertex>& getVertices() const
    {
        return vertices;
    }

    const std::vector<int>& getIndices() const
    {
        return indices;
    }

    size_t getNbVertices() const
    {
        return vertices.size();
    }

    size_t getNbIndices() const
    {
        return indices.size();
    }

    const qc_graphic::render::VertexArray& getVao() const
    {
        return vao;
    }

    // TODO 
    /*
        If set functions are required, think to create shape copy before
        if there is more than one instance which point on it.
    */


protected:
    std::vector<geometry::Vertex>   vertices;
    std::vector<int>                indices;

    qc_graphic::render::VertexArray vao;
};


}// render

}// geometry

}// qc_graphic