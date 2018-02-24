
namespace qc_graphic
{

namespace render
{

template <typename T>

class GLBuffer
{

public:

    // Constructors
    GLBuffer(const std::vector<T>& data)
    {
        glCreateBuffers(1, &pointer);
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
    GLBuffer();

    GLuint pointer = 0;

};

}

}
