#include "Application.hpp"

#include <iostream>

#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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
	m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "qc_engine", nullptr, nullptr);

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
	Cube shape = makeCube();
//    Square shape = makeSquare();
    /*
	std::cout << "point" << std::endl;
	for (auto& it : cube.points)
		std::cout << "[" << it.x << ", " << it.y << ", " << it.z << "]" << std::endl;
	std::cout << "indices" << std::endl;
	for (auto& it : cube.indices)
		std::cout << it << std::endl;
	*/
	// Buffers
        // Vbo
	GLuint vbo;
	glCreateBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferStorage(GL_ARRAY_BUFFER, shape.points.size() * sizeof(glm::vec3), shape.points.data(), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    assert(glGetError() == GL_NO_ERROR);

	    // Ibo
	GLuint ibo;
	glCreateBuffers(1, &ibo);
    glBindBuffer(GL_ARRAY_BUFFER, ibo);
	glBufferStorage(GL_ARRAY_BUFFER, shape.indices.size() * sizeof(int), shape.indices.data(), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    assert(glGetError() == GL_NO_ERROR);

        // Vao
	GLuint vao;
	glCreateVertexArrays(1, &vao);
	
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
    int positionAttrLocation = 0;
    glEnableVertexAttribArray(positionAttrLocation); // Active attribute
	glVertexAttribPointer(positionAttrLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    assert(glGetError() == GL_NO_ERROR);

    // Program
    GLuint vs = makeVertexShader();
    assert(glGetError() == GL_NO_ERROR);

    GLuint fs = makeFragmentShader();
    assert(glGetError() == GL_NO_ERROR);

    GLuint prog = makeProgram({ vs,fs });
    assert(glGetError() == GL_NO_ERROR);


    // Uniforms
    GLuint uMVPMatrix = glGetUniformLocation(prog, "uMVPMatrix");

    assert(glGetError() == GL_NO_ERROR);

    // Matrices
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -10));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(100));

	while (!glfwWindowShouldClose(m_window))
	{
        glViewport(0, 0, m_windowWidth, m_windowHeight);

		// Render here 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1, 0, 0, 1);

		glEnable(GL_DEPTH_TEST);
        
        glUseProgram(prog);
        assert(glGetError() == GL_NO_ERROR);


        glm::mat4 projMatrix = glm::perspective(70.f, float(m_windowWidth) / m_windowHeight, 0.01f, 100.f);
        glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
        glm::mat4 mvpMatrix = projMatrix * viewMatrix * modelMatrix;
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

        assert(glGetError() == GL_NO_ERROR);

		glBindVertexArray(vao);

        assert(glGetError() == GL_NO_ERROR);

		glDrawElements(GL_TRIANGLES, shape.indices.size(), GL_UNSIGNED_INT, nullptr);

        //modelMatrix = glm::rotate(modelMatrix, 0.01f, glm::vec3(0, 1, 0));

        assert(glGetError() == GL_NO_ERROR);

		glBindVertexArray(0);

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

	layout(location = 0) in vec3 aPosition;

	uniform mat4 uMVPMatrix;

	void main()
	{
        gl_Position = uMVPMatrix * vec4(aPosition,1);
	}
)";

    return makeShader(GL_VERTEX_SHADER, vertexShaderSrc.c_str());
}

GLuint Application::makeFragmentShader()
{
    std::string fragmentShaderSrc =
R"(
    #version 430

    out vec4 fColor;

    void main()
    {
        fColor = vec4(0,0,1,1);
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
	for (int i = 0; i < 8; ++i)
	{
		if (i % 4 == 0) x = -x;
		y = -y;
		if (i % 2 == 0) z = -z;

		cube.points.push_back(glm::vec3(x, y, z));
	}

	for (int i = 0; i <= 8; ++i)
	{
		cube.indices.push_back(i % 8);
		cube.indices.push_back((i + 1) % 8);
		cube.indices.push_back((i + 2) % 8);
	}

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j <= 4; j += 4)
		{
			cube.indices.push_back((j + i) % 8);
			cube.indices.push_back((j + i + 2) % 8);
			cube.indices.push_back((j + i + 4) % 8);
		}
	}

	return cube;
}


Application::Square Application::makeSquare()
{
    Square square;
    square.points = {
        glm::vec3(1,1,0),
        glm::vec3(-1,1,0),
        glm::vec3(-1,-1,0),
        glm::vec3(1,-1,0)
    };

    square.indices = {
        0, 1, 2,
        0, 2, 3
    };
    
    return square;
}