#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Temp
#include <glm/matrix.hpp>
#include <vector>

class Application
{
public:
	
    Application();
	~Application();

	//-- RUN -------------------------------
	/*
		Run the application and rendering loop
	*/
    int run();

	static bool waitKey();

private:
	//-- Window
    const size_t m_windowWidth = 1280;
    const size_t m_windowHeight = 720;
	GLFWwindow* m_window;
	
	//-- Struct

    struct Vertex
    {
        Vertex()
            : position(0), normal(0)
        {}

        Vertex(const glm::vec4& pos, const glm::vec4& norm)
            : position(pos), normal(norm)
        {}

        glm::vec4 position;
        glm::vec4 normal;
    };

	struct Cube
	{
		Cube()
		{}

		std::vector<Vertex> points;
		std::vector<int> indices;
	};
    

	//-- Function
    GLuint makeProgram(const std::vector<GLuint>& shader);
	GLuint makeVertexShader();
	GLuint makeFragmentShader();
	GLuint makeShader(GLuint type, const char* src);
	Cube makeCube();

};