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
    GLint location = glGetUniformLocation(this->id, name);

    if (location == -1)
    {
        std::string err = "Uniform named: ";
        err += name;
        //err += " seem to doesn't exist. (\"";
        //err += name;
        //err += "\" can exist, but discard at compilation because you don't use it)";
        err += " seem to doesn't exist. (It can exist, but discard at compilation because you don't use it)";
        qc::Useful::PrintError("UNIFORM LOCATION", err);
    }

    return location;
}

GLint Program::getRessourcesIndex(GLenum glInterface, const char* name) const
{
    GLint index = glGetProgramResourceIndex(this->id, glInterface, name);
    if (index == -1)
    {
        std::string err = "Ressource named: ";
        err += name;
        err += " seem to doesn't exist";
        qc::Useful::PrintError("RESSOURCES INDEX", err);
    }

    return index;
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




/*
 *
 *
 *
 *
 *
 */


 // GLOBAL USEFUL METHODS --------------------------------------------------------------------------------------------------------------
 // ------------------------------------------------------------------------------------------------------------------------------------

namespace qc
{

namespace render
{

namespace program
{



    static bool generateDefaultVertexShader(Shader& vs);
    static bool generateDefaultFragmentShader(Shader& fs);
    static bool makeDefaultProgram(Program& program);

    extern Program GetDefaultProgram()
    {
        Program defaultProgram;
        makeDefaultProgram(defaultProgram);
        return defaultProgram;
    }

    static bool generateDefaultVertexShader(Shader& vs)
    {
        std::string vertexShaderSrc =
            R"(
#version 430

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec4 aNormal; 
layout(location = 2) in vec2 aTexCoord; 

uniform mat4 uMVPMatrix;
uniform mat4 uNormalMatrix;

out vec4 vNormal;
out vec2 vTexCoord;

void main()
{
vNormal = uNormalMatrix * aNormal;
vTexCoord = aTexCoord;
gl_Position = uMVPMatrix * aPosition;
}
)";

        vs.appendCode(vertexShaderSrc);
        return vs.compile();
    }

    static bool generateDefaultFragmentShader(Shader& fs)
    {
        std::string fragmentShaderSrc =
            R"(
#version 430

layout(origin_upper_left) in vec4 gl_FragCoord;    

in vec4 vNormal;
in vec2 vTexCoord;

out vec4 fColor;

void checkerboard(in vec2 texCoord, inout vec4 color)
{
float scale = 3.0;
float sum = floor(texCoord.x * scale) + floor(texCoord.y * scale);
float modulo = mod(sum, 2); 
color =  vec4(color.xyz * modulo, 1);
} 

void main()
{
vec4 color = vec4(1, 0.5, 0, 1);
vec4 dir = normalize(vec4(0.25,0.25,0.75,0));

checkerboard(vTexCoord, color);
fColor = dot(vNormal, dir) * color;
}
)";

        fs.appendCode(fragmentShaderSrc);
        return fs.compile();
    }

    static bool makeDefaultProgram(Program& program)
    {
        Shader vs = Shader(ShaderType::VERTEX_SHADER);
        Shader fs = Shader(ShaderType::FRAGMENT_SHADER);

        generateDefaultVertexShader(vs);
        generateDefaultFragmentShader(fs);
        program.attach({ vs, fs });

        return program.link();
    }

}   //! program

}   //! render

}   //! qc