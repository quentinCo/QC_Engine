#pragma once

#include<glm\vec4.hpp>
#include<glm\vec2.hpp>

namespace qc
{

namespace render
{

struct Vertex
{
    // Constructors
    Vertex() : position(0), normal(0), uv(0) {}

    Vertex(const glm::vec4& position, const glm::vec4& normal, const glm::vec2& uv = glm::vec2(0))
        : position(position), normal(normal), uv(uv)
    {}

    ~Vertex() = default;

    glm::vec4 position;
    glm::vec4 normal;
    glm::vec2 uv;
};

};  //! namespace render

};  //! namespace qc
