#include <glm/vec4.hpp>

namespace qc_graphic
{
namespace geometry
{

struct Vertex
{
    // Methodes

    Vertex()
        : position(0), normal(0)
    {}

    Vertex(const glm::vec4& pos, const glm::vec4& norm)
        : position(pos), normal(norm)
    {}

    // Attributes

    glm::vec4 position;
    glm::vec4 normal;
};

}   // geometry

}   // qc_graphic

