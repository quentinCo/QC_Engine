#pragma once

#include <string>
#include <assert.h>

#include <glad/glad.h>


namespace qc
{

namespace render
{

namespace program
{

enum class ShaderType
{
    COMPUTE_SHADER,
    VERTEX_SHADER,
    TESS_CONTROL_SHADER,
    TESS_EVALUATION_SHADER,
    GEOMETRY_SHADER,
    FRAGMENT_SHADER
};

inline GLenum GetGLShaderType(const ShaderType& type)
{
    switch(type)
    {
        case ShaderType::COMPUTE_SHADER:
            return GL_COMPUTE_SHADER;
            break;
        case ShaderType::VERTEX_SHADER:
            return GL_VERTEX_SHADER;
            break;
        case ShaderType::TESS_CONTROL_SHADER:
            return GL_TESS_CONTROL_SHADER;
            break;
        case ShaderType::TESS_EVALUATION_SHADER:
            return GL_TESS_EVALUATION_SHADER;
            break;
        case ShaderType::GEOMETRY_SHADER:
            return GL_GEOMETRY_SHADER;
            break;
        case ShaderType::FRAGMENT_SHADER:
            return GL_FRAGMENT_SHADER;
            break;
    }

    assert(false && "ASSERT: You use a ShaderType which is not define in convertion static methods GetGLShaderType.");
    return GL_INVALID_ENUM;
}

inline void GetShaderTypeName(const ShaderType& type, std::string& name)
{
    switch (type)
    {
        case ShaderType::COMPUTE_SHADER:
            name = "compute_shader";
            break;
        case ShaderType::VERTEX_SHADER:
            name = "vertex_shader";
            break;
        case ShaderType::TESS_CONTROL_SHADER:
            name = "tees_control_shader";
            break;
        case ShaderType::TESS_EVALUATION_SHADER:
            name = "tess_evaluation_shader";
            break;
        case ShaderType::GEOMETRY_SHADER:
            name = "geometry_shader";
            break;
        case ShaderType::FRAGMENT_SHADER:
            name = "fragment_shader";
            break;
        default:
            name = "UNKNOWN";
            break;
    }
}


}   //! namespace program

}   //! namespace render

}   //! namespace qc

