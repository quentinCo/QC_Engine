#include "Application.hpp"

#include <iostream>

#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <qc_graphic/Geometry/Shapes/Shape.hpp>
#include <qc_graphic/Geometry/Transformation.hpp>


/*-------------------- APPLICATION  CONSTRUCTOR ----------------------------------*/

Application::Application()
{

	//
	// GLFW
	// -------------------------------------------------------------------------
	
	// Init glfw library
	if (!glfwInit())
	{
		std::string errorMess = "Unable to init GLFW";
		std::cerr << errorMess.c_str() << std::endl;
		throw std::runtime_error(errorMess.c_str());
	}

	// Create window
	m_window = glfwCreateWindow(static_cast<int>(m_windowWidth), static_cast<int>(m_windowHeight), "qc_engine", nullptr, nullptr);

	if (!m_window)
	{
		std::string errorMess = "Unable to create window";
		std::cerr << errorMess.c_str() << std::endl;
		throw std::runtime_error(errorMess.c_str());
	}

	// Make window's context
	glfwMakeContextCurrent(m_window);

	// -------------------------------------------------------------------------


	//
	// GLAD
	// -------------------------------------------------------------------------

	// Init OpenGL
	if (!gladLoadGL())
	{
		std::string errorMess = "Unable to init OpenGL";
		std::cerr << errorMess.c_str() << std::endl;
		throw std::runtime_error(errorMess.c_str());
	}

	// -------------------------------------------------------------------------

}

Application::~Application()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool Application::waitKey()
{
	std::cin.clear();
	std::cout << "Please press any key to continue" << std::endl;
	std::cout << "..." << std::endl;
	std::cin.get();
	return true;
}


/*-------------------------------  RUN  ------------------------------------------*/

int Application::run()
{
	Cube cube = makeCube();
	
	std::cout << "point" << std::endl;
    for (auto& it : cube.points)
    {
		std::cout << "[" << it.position.r << ", " << it.position.g << ", " << it.position.b << "]";
        std::cout << "[" << it.normal.r << ", " << it.normal.g << ", " << it.normal.b << "]" << std::endl;
    }
	std::cout << "indices" << std::endl;
    std::cout << "[" << std::endl;
    for (size_t i = 0; i < cube.indices.size(); i += 3)
    {
        std::cout << cube.indices[i] << ", " << cube.indices[i + 1] << ", " << cube.indices[i + 2] << std::endl;
    }
    std::cout << "]" << std::endl;
	
	// Buffers
    qc_graphic::geometry::shape::Shape shape = qc_graphic::geometry::shape::Shape(cube.points, cube.indices);

    // Program
    GLuint vs   = makeVertexShader();
    GLuint fs   = makeFragmentShader();
    GLuint prog = makeProgram({ vs,fs });

    // Uniforms
    GLuint uMVPMatrix       = glGetUniformLocation(prog, "uMVPMatrix");
    GLuint uColor           = glGetUniformLocation(prog, "uColor");
    GLuint uNormalMatrix    = glGetUniformLocation(prog, "uNormalMatrix");

    // Matrices
    qc_graphic::geometry::Transformation transform = qc_graphic::geometry::Transformation();
    transform.setPosition(0, 0, -4);

	while (!glfwWindowShouldClose(m_window))
	{
        glViewport(0, 0, static_cast<GLsizei>(m_windowWidth), static_cast<GLsizei>(m_windowHeight));

		// Render here 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.25, 0, 0, 1);

		glEnable(GL_DEPTH_TEST);
        
        glUseProgram(prog);

        glm::mat4 modelMatrix   = transform.getTransformMatrix();
        glm::mat4 projMatrix    = glm::perspective(70.f, float(m_windowWidth) / m_windowHeight, 0.01f, 100.f);
        glm::mat4 viewMatrix    = glm::lookAt(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
        glm::mat4 mvpMatrix     = projMatrix * viewMatrix * modelMatrix;

        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

        glUniform4fv(uColor, 1, glm::value_ptr(glm::vec4(1, 1, 0, 1)));

        glm::mat4 normalMatrix = glm::transpose(glm::inverse(viewMatrix * modelMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		
        const auto& vao = shape.getVao();
        vao.bindVertexArray();
		
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(cube.indices.size()), GL_UNSIGNED_INT, nullptr);

        const float angleX = 1 / 100.f;
        const float angleY = 1 / 200.f;
        const float angleZ = 1 / 400.f;
        transform.rotate(angleX, angleY, angleZ);

        vao.unbindVertexArray();

		glDisable(GL_DEPTH_TEST);

		// Swap front and back buffers
		glfwSwapBuffers(m_window);

		// Poll for and process events
		glfwPollEvents();
	}
	return 0;
}

GLuint Application::makeProgram(const std::vector<GLuint>& shaders)
{
    if (!shaders.size())
        return 0;

    GLuint prog = glCreateProgram();
    for(auto& it : shaders)
        glAttachShader(prog, it);

    glLinkProgram(prog);

    GLint linkStatus;
    glGetProgramiv(prog, GL_LINK_STATUS, &linkStatus);

    if (linkStatus != GL_TRUE) {
        GLint logLength;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);

        std::unique_ptr<char[]> buffer(new char[logLength]);
        glGetProgramInfoLog(prog, logLength, 0, buffer.get());

        std::cerr << "Program link error:" << buffer.get() << std::endl;
        throw std::runtime_error("Program link error:" + std::string(buffer.get()));
    }

    return prog;
}

GLuint Application::makeVertexShader()
{
	std::string vertexShaderSrc =
R"(
	#version 430

	layout(location = 0) in vec4 aPosition;
    layout(location = 1) in vec4 aNormal; 

	uniform mat4 uMVPMatrix;
    uniform mat4 uNormalMatrix;

    out vec4 vNormal;

	void main()
	{
        vNormal = uNormalMatrix * aNormal;
        gl_Position = uMVPMatrix * aPosition;
	}
)";

    return makeShader(GL_VERTEX_SHADER, vertexShaderSrc.c_str());
}

GLuint Application::makeFragmentShader()
{
    std::string fragmentShaderSrc =
R"(
    #version 430
    
    in vec4 vNormal;

    uniform vec4 uColor;

    out vec4 fColor;

    void main()
    {
        fColor = dot(vNormal, vec4(0,1,0,1)) * uColor;
    }
)";

    return makeShader(GL_FRAGMENT_SHADER, fragmentShaderSrc.c_str());
}

GLuint Application::makeShader(GLuint type, const char* src)
{
	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, 0);
	glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    assert(status == GL_TRUE);

	return shader;
}

Application::Cube Application::makeCube()
{
    Cube cube;
    int x = -1;
    int y = -1;
    int z = -1;
    std::vector<glm::vec4> vertex;
    for (int i = 0; i < 8; ++i)
    {
        if (i % 4 == 0) x = -x;
        y = -y;
        if (i < 2 || i >= 6)
            z = 1;
        else
            z = -1;

        vertex.push_back(glm::vec4(x, y, z, 1));
    }


    cube.points = {
        qc_graphic::geometry::Vertex(vertex[0], glm::vec4(1,0,0,0)),
        qc_graphic::geometry::Vertex(vertex[1], glm::vec4(1,0,0,0)),
        qc_graphic::geometry::Vertex(vertex[2], glm::vec4(1,0,0,0)),
        qc_graphic::geometry::Vertex(vertex[3], glm::vec4(1,0,0,0)),
        qc_graphic::geometry::Vertex(vertex[4], glm::vec4(-1,0,0,0)),
        qc_graphic::geometry::Vertex(vertex[5], glm::vec4(-1,0,0,0)),
        qc_graphic::geometry::Vertex(vertex[6], glm::vec4(-1,0,0,0)),
        qc_graphic::geometry::Vertex(vertex[7], glm::vec4(-1,0,0,0)),

        qc_graphic::geometry::Vertex(vertex[0], glm::vec4(0,0,1,0)),
        qc_graphic::geometry::Vertex(vertex[1], glm::vec4(0,0,1,0)),
        qc_graphic::geometry::Vertex(vertex[7], glm::vec4(0,0,1,0)),
        qc_graphic::geometry::Vertex(vertex[6], glm::vec4(0,0,1,0)),
        qc_graphic::geometry::Vertex(vertex[2], glm::vec4(0,0,-1,0)),
        qc_graphic::geometry::Vertex(vertex[3], glm::vec4(0,0,-1,0)),
        qc_graphic::geometry::Vertex(vertex[5], glm::vec4(0,0,-1,0)),
        qc_graphic::geometry::Vertex(vertex[4], glm::vec4(0,0,-1,0)),

        qc_graphic::geometry::Vertex(vertex[0], glm::vec4(0,1,0,0)),
        qc_graphic::geometry::Vertex(vertex[2], glm::vec4(0,1,0,0)),
        qc_graphic::geometry::Vertex(vertex[4], glm::vec4(0,1,0,0)),
        qc_graphic::geometry::Vertex(vertex[6], glm::vec4(0,1,0,0)),
        qc_graphic::geometry::Vertex(vertex[1], glm::vec4(0,-1,0,0)),
        qc_graphic::geometry::Vertex(vertex[3], glm::vec4(0,-1,0,0)),
        qc_graphic::geometry::Vertex(vertex[5], glm::vec4(0,-1,0,0)),
        qc_graphic::geometry::Vertex(vertex[7], glm::vec4(0,-1,0,0))
    };

    cube.indices = {
        0, 1, 2,
        3, 2, 1,
        4, 5, 6,
        7, 6, 5,

        8, 11, 10,
        8, 10, 9,
        12, 13, 15,
        13, 14, 15,

        16, 17, 18,
        16, 18, 19,
        20, 21, 22,
        20, 22, 23
    };

    return cube;
}