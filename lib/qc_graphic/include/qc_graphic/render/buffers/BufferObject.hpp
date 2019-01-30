#pragma once

#include <glad/glad.h>
#include <vector>

#include <assert.h>

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
    BufferObject();                                                     // init pointeur to 0
    BufferObject(const T* data, unsigned int size, GLbitfield  flags);  // allocate and populate buffer
    BufferObject(const std::vector<T>& data, GLbitfield  flags);        // --

    BufferObject(const BufferObject<T>& o) = delete;                    // NO COPY

    BufferObject(BufferObject<T>&& r);

    virtual ~BufferObject();                                            // free the pointer

    // Operators
    BufferObject<T>& operator= (const BufferObject<T>& o) = delete;     // NO COPY
    BufferObject<T>& operator= (BufferObject<T>&& r);


    // Getters / Setters
    GLuint getPointer() const;

    // Methods
    void allocate();                                                    // Create the buffer and save pointer value in pointer attribute

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





/*
 *
 *
 *
 *
 *
 */


 // PUBLIC -----------------------------------------------------------------------------------------------------------------------------
 // ------------------------------------------------------------------------------------------------------------------------------------

/*-------------------- BUFFEROBJECT CONSTRUCTORS ---------------------------------*/
template<typename T>
BufferObject<T>::BufferObject()
    : pointer(0), flags(0) 
{}

template<typename T>
BufferObject<T>::BufferObject(const T* data, unsigned int size, GLbitfield  flags)
    : BufferObject()
{
    this->allocate();
    this->populate(data, size, flags);
}

template<typename T>
BufferObject<T>::BufferObject(const std::vector<T>& data, GLbitfield  flags)
    : BufferObject()
{
    this->allocate();
    this->populate(data, flags);
}


template<typename T>
BufferObject<T>::BufferObject(BufferObject<T>&& r)
{
    if (this->isAllocated())
        this->release();

    this->pointer = r.pointer;
    this->flags = r.flags;

    r.pointer = 0;
}


template<typename T>
BufferObject<T>::~BufferObject()
{
    if (this->isAllocated())
        this->release();
}

/*-------------------- BUFFEROBJECT OPERATORS ---------------------------------*/
template<typename T>
BufferObject<T>& BufferObject<T>::operator= (BufferObject<T>&& r)
{
    if (this->isAllocated())
        this->release();

    this->pointer = r.pointer;
    this->flags = r.flags;

    r.pointer = 0;

    return *this;
}

/*-------------------- BUFFEROBJECT GETTERS ----------------------------------*/
template<typename T>
GLuint BufferObject<T>::getPointer() const
{
    return this->pointer;
}

/*-------------------- BUFFEROBJECT FUNCTIONS ---------------------------------*/
template<typename T>
void BufferObject<T>::allocate()
{
    glCreateBuffers(1, &this->pointer);
}

//template<typename T>
//bool BufferObject<T>::bind(){}

//template<typename T>
//bool BufferObject<T>::unbind(){}


template<typename T>
bool BufferObject<T>::isAllocated()
{
    return this->pointer != 0;
}




/*
 *
 *
 *
 *
 *
 */


 // PROTECTED --------------------------------------------------------------------------------------------------------------------------
 // ------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
bool BufferObject<T>::populate(const T* data, unsigned int size, GLbitfield  flags)
{
    assert(this->isAllocated() && "ASSERT: You try to populate an un-allocated buffer.");
    glNamedBufferStorage(this->pointer, size * sizeof(T), data, flags);
    return true;
}

template<typename T>
bool BufferObject<T>::populate(const std::vector<T>& data, GLbitfield  flags)
{
    return this->populate(data.data(), static_cast<unsigned int>(data.size()), flags);
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

template<typename T>
void BufferObject<T>::release()
{
    glDeleteBuffers(1, &this->pointer);
}














} // ! namespace buffer

}; // ! namespace render

}; // ! namespace qc
