#pragma once

#include <qc_graphic\render\buffers\BufferObject.hpp>

namespace qc
{

namespace render
{

namespace buffer
{

class IBO : public BufferObject<unsigned int>
{
public :
    // Constructor - destructor
    IBO() {}                                                            // init pointeur to 0

    IBO(const unsigned int* data, unsigned int size);                   // allocate and populate buffer
    IBO(const std::vector<unsigned int>& data);                         // --

    IBO(const IBO& o) = delete;                                         // NO COPY

    IBO(IBO&& r) = default;

    ~IBO() {}                                                           // free the pointer

    // Operators
    IBO& operator= (const IBO& o) = delete;                             // NO COPY
    IBO& operator= (IBO&& r) = default;

    // Methods
    bool populate(const unsigned int* data, unsigned int size) override;       // Call the protected populate function with the specific flag
    bool populate(const std::vector<unsigned int>& data) override;             // --
};

}   //! namespace buffer

}   //! namespace render

}   //! namespace qc