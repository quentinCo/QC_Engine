
#include <qc_graphic/Render/Shapes/Shape.hpp>

using namespace qc_graphic;
using namespace qc_graphic::render;
using namespace qc_graphic::render::shape;

Shape::Shape(const std::vector<geometry::Vertex>& vertices, const std::vector<int>& indices)
    : vertices(vertices), indices(indices)
{
    render::GLBuffer<geometry::Vertex> vbo  = render::GLBuffer<geometry::Vertex>(vertices);
    render::GLBuffer<int> ibo               = render::GLBuffer<int>(indices);

    this->vao = render::VertexArray(vbo, ibo);
}

