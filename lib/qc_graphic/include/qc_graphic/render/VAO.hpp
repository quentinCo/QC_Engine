#pragma once

#include <qc_graphic\render\buffers\VBO.hpp>
#include <qc_graphic\render\buffers\IBO.hpp>

namespace qc
{

namespace render
{

enum AttributeLocation
{
    POSITION = 0,
    NORMAL,
    UV,
    NB_ATTRIBUTE_LOCATION
};

class VAO
{

public:
    // Constructor - destructor
    VAO() : pointer(0), vbo(nullptr), ibo(nullptr) {};                              // init pointers to default zero and nullptr

    VAO(const buffer::VBO& vbo, const buffer::IBO& ibo);                            // allocated vertex array and link the vbo and ibo

    VAO(const VAO& o) = delete;                                                     // NO COPY
    
    VAO(VAO&& r);

    ~VAO();

    // Operators
    VAO& operator= (const VAO& o) = delete;                                         // NO COPY

    VAO& operator= (VAO&& r);

    // Getters / Setters
    GLuint getPointer() const
    {
        return this->pointer;
    }

    const buffer::VBO* getVBO() const
    {
        return this->vbo;
    }

    const buffer::IBO* getIBO() const
    {
        return this->ibo;
    }

    // Methods
    void allocate();                                                                // create the buffer and save pointer value in pointer attribute 
    void link(const buffer::VBO& vbo, const buffer::IBO& ibo);                      // link VAO with VBO and IBO (save pointer to both object)

    bool isAllocated();
    bool isUsebable();                                                              // return true if VAO is allocated and linked

private:
    GLuint              pointer;
    const buffer::VBO*  vbo;
    const buffer::IBO*  ibo;


    // Methods
    void release();                                                                 // release the pointer

};

}   //! namespace render

}   //! namespace qc
