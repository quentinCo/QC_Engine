#include <qc_graphic\render\VAO.hpp>

using namespace qc::render;
using namespace qc::render::buffer;

/*-------------------- VAO CONSTRUCTORS ---------------------------------*/
VAO::VAO(const VBO& vbo, const IBO& ibo)
    : VAO()
{
    this->allocate();
    this->link(vbo, ibo);
}

VAO::VAO(VAO&& r)
{
    if (this->isAllocated())
        this->release();

    this->pointer   = r.pointer;
    this->vbo       = r.vbo;
    this->ibo       = r.ibo;

    r = VAO();
}

VAO::~VAO()
{
    if (this->isAllocated())
        this->release();
}

/*-------------------- VAO OPERATORS ---------------------------------*/
VAO& VAO::operator= (VAO&& r)
{
    if (this->isAllocated())
        this->release();

    this->pointer   = r.pointer;
    this->vbo       = r.vbo;
    this->ibo       = r.ibo;

    r.pointer   = 0;
    r.vbo       = nullptr;
    r.ibo       = nullptr;

    return *this;
}

/*-------------------- VAO FUNCTIONS ---------------------------------*/
void VAO::allocate()
{
    glCreateVertexArrays(1, &this->pointer);
}

void VAO::link(const buffer::VBO& vbo, const buffer::IBO& ibo)
{
    assert(this->isAllocated() && "ASSERT: you try to link an un-allocated VAO");
    
    // vbo
    int bindingIdx = 0;
    glVertexArrayVertexBuffer(this->pointer, bindingIdx, vbo.getPointer(), 0, sizeof(Vertex)); // Bind vbo in vao at binding index 0, starting offset 0, offset sizeof(Vertex)

    // -- attributes
    glVertexArrayAttribBinding(this->pointer, AttributeLocation::POSITION, bindingIdx);
    glEnableVertexArrayAttrib(this->pointer, AttributeLocation::POSITION);
    glVertexArrayAttribFormat(this->pointer, AttributeLocation::POSITION, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));

    glVertexArrayAttribBinding(this->pointer, AttributeLocation::NORMAL, bindingIdx);
    glEnableVertexArrayAttrib(this->pointer, AttributeLocation::NORMAL);
    glVertexArrayAttribFormat(this->pointer, AttributeLocation::NORMAL, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));

    glVertexArrayAttribBinding(this->pointer, AttributeLocation::UV, bindingIdx);
    glEnableVertexArrayAttrib(this->pointer, AttributeLocation::UV);
    glVertexArrayAttribFormat(this->pointer, AttributeLocation::UV, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));

    // ibo
    glVertexArrayElementBuffer(this->pointer, ibo.getPointer());

    // save pointers
    this->vbo = &vbo;
    this->ibo = &ibo;
}

bool VAO::bind()
{
    if (this->isUsebable() == false)
        return false;

    glBindVertexArray(this->pointer);
    return true;
}

bool VAO::isAllocated()
{
    return this->pointer != 0;
}

bool VAO::isUsebable()
{
    return (this->pointer != 0 && this->vbo != nullptr && this->ibo != nullptr);
}





/*
 *
 *
 *
 *
 *
 */


 // PRIVATE ----------------------------------------------------------------------------------------------------------------------------
 // ------------------------------------------------------------------------------------------------------------------------------------
void VAO::release()
{
    glDeleteVertexArrays(1, &this->pointer);
}
