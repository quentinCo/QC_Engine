#pragma once

#include <glad/glad.h>

#include <vector>

namespace qc_graphic
{

namespace render
{

template <typename T>

class GLBuffer
{

public:

    // Constructors
    GLBuffer()
        : pointer(0)
    {}

    GLBuffer(const std::vector<T>& data)
        : pointer(0)
    {
        glCreateBuffers(1, &pointer);
        assert(pointer != 0);

        glNamedBufferStorage(pointer, data.size() * sizeof(T), data.data(), 0);
    }

    // Destructor
    ~GLBuffer()
    {
        if (pointer != 0) glDeleteBuffers(1, &pointer);
    }

    // Copy operators
    GLBuffer(const GLBuffer<T>& o)                  = delete;
    GLBuffer<T>& operator=(const GLBuffer<T>& o)    = delete;

    // Move operators
    GLBuffer(GLBuffer<T>&& o)
    {
        if(pointer != 0)
            glDeleteBuffers(1, &pointer);

        pointer = o.pointer;
        o.pointer = 0;
    }

    GLBuffer<T>& operator=(GLBuffer<T>&& o)
    {
        if (pointer != 0)
            glDeleteBuffers(1, &pointer);

        pointer = o.pointer;
        o.pointer = 0;
        return *this;
    }

    // Getters
    GLuint getPointer()
    {
        return pointer;
    }


private:

    GLuint pointer = 0;

};

}// render

}// qc_graphic
