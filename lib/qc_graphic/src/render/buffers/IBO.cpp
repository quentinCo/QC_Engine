#include<qc_graphic\render\buffers\IBO.hpp>

using namespace qc::render::buffer;

/*-------------------- IBO CONSTRUCTORS ---------------------------------*/
IBO::IBO(const unsigned int* data, unsigned int size)
{
    this->allocate();
    this->populate(data, size);
}

IBO::IBO(const std::vector<unsigned int>& data)
{
    this->allocate();
    this->populate(data);
}

// Methods
bool IBO::populate(const unsigned int* data, unsigned int size)
{
    return BufferObject<unsigned int>::populate(data, size, 0);
}

bool IBO::populate(const std::vector<unsigned int>& data)
{
    return BufferObject<unsigned int>::populate(data, 0);
}
