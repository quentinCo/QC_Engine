
#include<qc_graphic/Render/VertexArray.hpp>

using namespace qc_graphic;
using namespace qc_graphic::render;

VertexArray::VertexArray(GLBuffer<geometry::Vertex>& vbo, GLBuffer<int>& ibo)
    : pointer(0), vbo(std::move(vbo)), ibo(std::move(ibo))
{
    assert((this->vbo.getPointer() != 0 || this->ibo.getPointer() != 0) && "You are trying to create VerteArray with uninitialised GLBuffer");

    glCreateVertexArrays(1, &pointer);
    assert(pointer != 0);

    int bindingVboIndex = 0;
    glVertexArrayVertexBuffer(pointer, bindingVboIndex, this->vbo.getPointer(), 0, sizeof(geometry::Vertex)); // Link vao and vbo

    glVertexArrayAttribBinding(pointer, AttributeLocation::POSITION, bindingVboIndex);  // Link vbo position attribute and vao
    glEnableVertexArrayAttrib(pointer,  AttributeLocation::POSITION);                   // Active attribute
    glVertexArrayAttribFormat(pointer,  AttributeLocation::POSITION, 4, GL_FLOAT, GL_FALSE, offsetof(geometry::Vertex, position));

    glVertexArrayAttribBinding(pointer, AttributeLocation::NORMAL, bindingVboIndex);
    glEnableVertexArrayAttrib(pointer,  AttributeLocation::NORMAL);
    glVertexArrayAttribFormat(pointer,  AttributeLocation::NORMAL, 4, GL_FLOAT, GL_FALSE, offsetof(geometry::Vertex, normal));

    glVertexArrayElementBuffer(pointer, this->ibo.getPointer()); // Link vao and ibo;
}

VertexArray::VertexArray(VertexArray&& o)
{
    if (pointer != 0)
        glDeleteVertexArrays(1, &pointer);

    pointer = o.pointer;
    vbo = std::move(o.vbo);
    ibo = std::move(o.ibo);

    o.pointer = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& o)
{
    if (pointer != 0)
        glDeleteVertexArrays(1, &pointer);

    pointer = o.pointer;
    vbo = std::move(o.vbo);
    ibo = std::move(o.ibo);

    o.pointer = 0;
    return *this;
}

