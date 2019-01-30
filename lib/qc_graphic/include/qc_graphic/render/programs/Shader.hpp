#pragma once

#include <sstream>

#include <glad/glad.h>

#include <qc_graphic\render\programs\ShaderType.hpp>

namespace qc
{

namespace render
{

namespace program
{

class Shader
{

public:
    // Constructor - destructor
    Shader(const ShaderType& type);

    Shader(const Shader& o) = delete;

    Shader(Shader&& r);

    ~Shader();

    // Operators
    Shader& operator= (const Shader& o) = delete;

    Shader& operator= (Shader&& r);

    // Getters
    GLuint getId() const
    {
        return this->id;
    }

    const std::ostringstream& getCode() const
    {
        return this->code;
    }

    // Functions
    void appendCode(const std::string& code)
    {
        this->code << code;
    }

    bool compile();




private:

    GLuint              id;
    std::ostringstream  code;
    ShaderType          type;

    // Function
    int shaderLength();

    bool checkCompilationError();


    #if _DEBUG
        unsigned int debugId = 0;

        void printShaderInFile(bool hasError = false);
    #endif
};

}   //! namespace program

}   //! namespace render

}   //! namespace qc

