#pragma once

#include <initializer_list>
#include <vector>
#include <functional>

#include <glad/glad.h>

#include <qc_graphic\render\programs\Shader.hpp>

namespace qc
{

namespace render
{

namespace program
{

class Program
{

public:
    // Constructor - destructor
    Program();

    Program(const Program& o) = delete;

    Program(Program&& r);

    ~Program();

    // Operators
    Program& operator= (const Program& o) = delete;

    Program& operator= (Program&& r);

    // Getters
    GLuint getId()
    {
        return this->id;
    }

    // Functions
    bool attach(std::initializer_list<std::reference_wrapper<const Shader>> shaders);       // initializer_list can't contain ref, so we use reference_wrapper (see doc for both)

    //bool attach(const std::vector< std::reference_wrapper<const Shader>>& shaders);
    bool attach(const std::vector<const Shader*>& shaders);
    bool attach(const Shader& shader);

    bool link();

private:
    GLuint              id;
    std::vector<GLuint> attachedList; // TODO rework it, use for the detach

    bool checkLinkError();

};

}

}

}