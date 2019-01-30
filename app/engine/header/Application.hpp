#pragma once

#include <qc_graphic\Window.hpp>

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

private:
	//-- Window
    qc::Window  window;

	//-- Struct

	//-- Function
};