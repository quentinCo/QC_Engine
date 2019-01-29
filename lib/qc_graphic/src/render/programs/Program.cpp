#include <qc_graphic\render\programs\Program.hpp>

#include <qc_graphic\Useful.hpp>

using namespace qc::render::program;

/*-------------------- PROGRAM CONSTRUCTORS ---------------------------------*/
Program::Program()
    : id(0)
{
    this->id = glCreateProgram();
}

Program::Program(Program&& r)
{
    this->~Program();

    this->id            = r.id;
    this->attachedList  = std::move(r.attachedList);

    r.id = 0;
}

Program::~Program()
{
    glDeleteProgram(this->id);
}


/*-------------------- PROGRAM OPERATORS ---------------------------------*/
Program& Program::operator= (Program&& r)
{
    this->~Program();

    this->id            = r.id;
    this->attachedList  = std::move(r.attachedList);

    r.id = 0;

    return *this;
}

/*-------------------- PROGRAM GETTERS ---------------------------------*/

GLint Program::getUniformLocation(const char* name) const
{
    return glGetUniformLocation(this->id, name);
}

GLint Program::getRessourcesIndex(GLenum glInterface, const char* name) const
{
    return glGetProgramResourceIndex(this->id, glInterface, name);
}

/*-------------------- PROGRAM FUNCTIONS ---------------------------------*/
bool Program::attach(std::initializer_list<std::reference_wrapper<const Shader>> shaders)
{
    bool res = true;
    for (const auto& it : shaders)
        res |= this->attach(it);

    return res;
}

bool Program::attach(const std::vector<const Shader*>& shaders)
{
    bool res = true;
    for (const auto& it : shaders)
        res |= this->attach(*it);

    return res;
}

bool Program::attach(const Shader& shader)
{
    GLuint shaderId = shader.getId();

    glAttachShader(this->id, shaderId);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        return false;

    this->attachedList.push_back(shaderId);
    return true;
}

bool Program::link()
{
    glLinkProgram(this->id);

    bool success = this->checkLinkError();

    /*
        As advised in khronos wiki, we detach the linked shader.
        https://www.khronos.org/opengl/wiki/Shader_Compilation#Cleanup
     */

    // TODO: see if it's judicious to do it if link fail.

    for (auto& it : this->attachedList)
        glDetachShader(this->id, it);

    return success;
}

void Program::use() const
{
    glUseProgram(this->id);
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
bool Program::checkLinkError()
{
    GLint isSucceed = 0;
    glGetProgramiv(this->id, GL_LINK_STATUS, &isSucceed);
    if (isSucceed == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &maxLength);

        std::unique_ptr<char> errorLog(new char[maxLength]);
        glGetProgramInfoLog(this->id, maxLength, &maxLength, errorLog.get());

        qc::Useful::PrintError("PROGRAM LINKING", errorLog.get());

        this->~Program();

        return false;
    }

    return true;
}