#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Temp
#include <glm/matrix.hpp>
#include <vector>

#include <qc_graphic\Geometry\Vertex.hpp>

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
	struct Cube
	{
		Cube()
		{}

		std::vector<qc_graphic::geometry::Vertex> points;
		std::vector<int> indices;
	};
    

	//-- Function
    GLuint makeProgram(const std::vector<GLuint>& shader);
	GLuint makeVertexShader();
	GLuint makeFragmentShader();
	GLuint makeShader(GLuint type, const char* src);
	Cube makeCube();

};