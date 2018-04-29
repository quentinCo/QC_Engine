#pragma once

#include <glad/glad.h>

#include<qc_graphic/Render/GLBuffer.hpp>
#include<qc_graphic/Geometry/Vertex.hpp>

namespace qc_graphic
{

namespace render
{

class VertexArray
{

public:

    VertexArray()
        : pointer(0)
    {}

    VertexArray(GLBuffer<geometry::Vertex>& vbo, GLBuffer<int>& ibo);

    ~VertexArray()
    {
        if (pointer != 0) glDeleteVertexArrays(1, &pointer);
    }

    VertexArray(const VertexArray& o)               = delete;
    VertexArray& operator=(const VertexArray& o)    = delete;

    VertexArray(VertexArray&& o);
    VertexArray& operator=(VertexArray&& o);

    void bindVertexArray() const
    {
        assert(pointer != 0 && "You are trying to bind an uninitialised VertexArray");
        glBindVertexArray(pointer);
    }

    void unbindVertexArray() const
    {
        assert(pointer != 0 && "You are trying to unbind an uninitialised VertexArray");
        glBindVertexArray(0);
    }


private:

    GLuint pointer;

    GLBuffer<geometry::Vertex>  vbo;
    GLBuffer<int>               ibo;
};



enum AttributeLocation
{
    POSITION = 0,
    NORMAL,
};


}// render

}// qc_graphic