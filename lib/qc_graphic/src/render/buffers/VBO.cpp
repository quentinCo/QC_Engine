#include<qc_graphic\render\buffers\VBO.hpp>

using namespace qc::render::buffer;
using namespace qc::render;

/*-------------------- VBO CONSTRUCTORS ---------------------------------*/
VBO::VBO(const Vertex* data, unsigned int size)
{
    this->allocate();
    this->populate(data, size);
}

VBO::VBO(const std::vector<Vertex>& data)
{
    this->allocate();
    this->populate(data);
}

// Methods
bool VBO::populate(const Vertex* data, unsigned int size)
{
    return BufferObject<Vertex>::populate(data, size, 0);
}

bool VBO::populate(const std::vector<Vertex>& data)
{
    return BufferObject<Vertex>::populate(data, 0);
}
