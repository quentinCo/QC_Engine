#pragma once

#include <qc_graphic\render\buffers\BufferObject.hpp>
#include <qc_graphic\render\Vertex.hpp>

namespace qc
{

namespace render
{

namespace buffer
{

class VBO : public BufferObject<qc::render::Vertex>
{
public :
    // Constructor - destructor
    VBO() = default;

    VBO(const qc::render::Vertex* data, unsigned int size);                 // allocate and populate buffer
    VBO(const std::vector<qc::render::Vertex>& data);                       // --

    VBO(const VBO& o) = delete;                                             // NO COPY

    VBO(VBO&& r) = default;

    ~VBO() = default;

    // Operators
    VBO& operator= (const VBO& o) = delete;                                 // NO COPY
    VBO& operator= (VBO&& r) = default;
    
    // Methods
    bool populate(const qc::render::Vertex* data, unsigned int size) override;       // Call the protected populate function with the specific flag
    bool populate(const std::vector<qc::render::Vertex>& data) override;             // --
};

}   //! namespace buffer

}   //! namespace render

}   //! namespace qc