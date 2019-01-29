#pragma once

#include <glad/glad.h>
#include <vector>

namespace qc
{

namespace render
{

namespace buffer
{

template <typename T>
class BufferObject
{

public:
    // Constructor - destructor
    BufferObject() : pointer(0), flags(0) {}                            // init pointeur to 0
    
    BufferObject(const T* data, unsigned int size, GLbitfield  flags);  // allocate and populate buffer
    BufferObject(const std::vector<T>& data, GLbitfield  flags);        // --

    BufferObject(const BufferObject<T>& o) = delete;                    // NO COPY

    BufferObject(BufferObject<T>&& r);

    virtual ~BufferObject();                                            // free the pointer

    // Operators
    BufferObject<T>& operator= (const BufferObject<T>& o) = delete;     // NO COPY
    BufferObject<T>& operator= (BufferObject<T>&& r);


    // Getters / Setters
    GLuint getPointer() const
    {
        return this->pointer;
    }

    // Methods
    bool allocate();                                                    // Create the buffer and save pointer value in pointer attribute

    virtual bool populate(const T* data, unsigned int size) = 0;        // Call the protected populate function with the specific flag
    virtual bool populate(const std::vector<T>& data) = 0;              // --

    //bool bind();
    
    //bool unbind();

    bool isAllocated();

protected:
    bool populate(const T* data, unsigned int size, GLbitfield  flags); // allocate the buffer size and populate the storage
    bool populate(const std::vector<T>& data, GLbitfield  flags);       // --

private:
    // Properties
    GLuint      pointer;
    GLbitfield  flags;

    // Methods
    void release();
};

} // ! namespace buffer

}; // ! namespace render

}; // ! namespace qc
