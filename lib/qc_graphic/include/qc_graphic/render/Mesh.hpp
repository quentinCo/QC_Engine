#pragma once

#include <vector>

#include <qc_graphic\render\Vertex.hpp>
#include <qc_graphic\render\VAO.hpp>

namespace qc
{

namespace render
{

class Mesh
{
public:
    // Constructor - destructor
    Mesh() = default;

    Mesh(const Vertex* vertices, size_t nbVerticies, const unsigned int* indices, size_t nbIndices)
    {
        this->vertices = std::vector<Vertex>(vertices, vertices + nbVerticies);
        this->indices = std::vector<unsigned int>(indices, indices + nbIndices);

        this->vbo = buffer::VBO(this->vertices);
        this->ibo = buffer::IBO(this->indices);
        this->vao = VAO(this->vbo, this->ibo);
    }

    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
        : Mesh(vertices.data(), vertices.size(), indices.data(), indices.size())
    {}

    Mesh(const Mesh& o) = delete;
    
    Mesh(Mesh&& r) = default;

    ~Mesh() = default;

    // Operators
    Mesh& operator= (const Mesh& o) = delete;
    Mesh& operator= (Mesh&& r) = default;

    // Getters
    const VAO& getVAO() const
    {
        return this->vao;
    }

    GLuint getVAOPointer() const
    {
        return this->vao.getPointer();
    }

    GLsizei getNbIndices() const
    {
        return static_cast<GLsizei>(this->indices.size());
    }

private:
    std::vector<Vertex>         vertices;
    std::vector<unsigned int>   indices;

    buffer::VBO vbo;
    buffer::IBO ibo;
    VAO         vao;
};

}

}