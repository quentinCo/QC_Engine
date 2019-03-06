#include <qc_graphic\render\programs\Shader.hpp>

#include <qc_graphic\Useful.hpp>

#if _DEBUG
    #include <iostream>
    #include <fstream>  
#endif

using namespace qc::render::program;

/*-------------------- SHADER CONSTRUCTORS ---------------------------------*/
Shader::Shader(const ShaderType& type)
    : id(0), type(type)
{
    GLenum glType   = GetGLShaderType(type);
    id              = glCreateShader(glType);
    
    assert(id != 0);
}

Shader::Shader(Shader&& r)
{
    this->~Shader();

    this->id    = r.id;
    this->code  = std::move(r.code);
    this->type = r.type;

    r.id = 0;
}

Shader::~Shader()
{
    /*
        Remember:
            As write in the khronos document, glDeleteShader don't delete but flag it as "to destoy".
            So if it's attached to a program, the shader is not deleted until the program exist.
            https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glAttachShader.xhtml

            The following code is possible and legit:
                {
                    Shader s = Shader(ShaderType::FRAGMENT_SHADER);
                    shader.appendCode();
                    shader.compile();
                    prog.attach(s);
                } // Shader object is destroy but not gl shader 

     */

    glDeleteShader(this->id);

    this->code.clear();
}


/*-------------------- SHADER OPERATORS ---------------------------------*/
Shader& Shader::operator= (Shader&& r)
{
    this->~Shader();

    this->id = r.id;
    this->code = std::move(r.code);
    this->type = r.type;

    r.id = 0;

    return *this;
}

/*-------------------- BUFFEROBJECT FUNCTIONS ---------------------------------*/
bool Shader::compile()
{
    int length = this->shaderLength();

    assert(length > 0 && "ASSERT: Try to compile an empty shader");
    
    std::string codeStr = this->code.str();
    const char* codeC   = codeStr.c_str();
    glShaderSource(this->id, 1, &codeC, 0);

    glCompileShader(this->id);

    bool success = this->checkCompilationError();

    #if _DEBUG
        this->printShaderInFile(!success);
    #endif

    if (!success)
        throw std::runtime_error("Shader Compilation Error");

    return success;
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
int Shader::shaderLength()
{
    this->code.seekp(0, std::ios_base::end);
    return static_cast<int>(this->code.tellp());
}


bool Shader::checkCompilationError()
{
    GLint isSucceed = 0;
    glGetShaderiv(this->id, GL_COMPILE_STATUS, &isSucceed);

    if (isSucceed == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &maxLength);

        std::unique_ptr<char[]> errorLog(new char[maxLength]);
        glGetShaderInfoLog(this->id, maxLength, &maxLength, errorLog.get());

        qc::Useful::PrintError("SHADER COMPILATION", errorLog.get());

        this->~Shader();

        return false;
    }

    return true;
}


#if _DEBUG
    void Shader::printShaderInFile(bool hasError)
    {

        // TODO Adapte for linux
        #if _WIN32
            if (CreateDirectory(".\\output_shader", NULL) || ERROR_ALREADY_EXISTS == GetLastError())
            {
                std::string shaderType;
                GetShaderTypeName(this->type, shaderType);

                std::string fileName = ".\\output_shader\\";
                if (hasError)
                    fileName += "ERROR_";
                fileName += shaderType;
                fileName += "_";
                fileName += std::to_string(this->debugId++);
                fileName += ".glsl";
        
                std::ofstream outfile(fileName);

                outfile << this->code.str() << std::endl;
            
                outfile.close();
            
                std::cout << "--> SHADER writed in file: " << fileName << std::endl;
            }
        #elif __linux__

        #endif
    }
#endif