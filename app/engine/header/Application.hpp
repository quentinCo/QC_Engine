#pragma once

// Temp
#include <glm/matrix.hpp>
#include <vector>
#include <ctime>

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

	//-- Struct

	//-- Function
};