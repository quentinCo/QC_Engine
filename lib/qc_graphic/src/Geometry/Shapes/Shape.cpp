
#include <qc_graphic/Geometry/Shapes/Shape.hpp>

using namespace qc_graphic;
using namespace qc_graphic::geometry;
using namespace qc_graphic::geometry::shape;

Shape::Shape(const std::vector<Vertex>& vertices, const std::vector<int>& indices)
    : vertices(vertices), indices(indices)
{
    render::GLBuffer<Vertex> vbo    = render::GLBuffer<Vertex>(vertices);
    render::GLBuffer<int> ibo       = render::GLBuffer<int>(indices);

    this->vao = render::VertexArray(vbo, ibo);
}

