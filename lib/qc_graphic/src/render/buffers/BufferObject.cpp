#include <qc_graphic\render\buffers\BufferObject.hpp>

using namespace qc::render::buffer;


/*-------------------- BUFFEROBJECT CONSTRUCTORS ---------------------------------*/
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

    this->pointer   = r.pointer;
    this->flags     = r.flags;
    
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

    this->pointer   = r.pointer;
    this->flags     = r.flags;
    
    r.pointer = 0;

    return *this;
}

/*-------------------- BUFFEROBJECT FUNCTIONS ---------------------------------*/
template<typename T>
bool BufferObject<T>::allocate()
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
    return this->populate(data.data(), data.size(), flags);
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
