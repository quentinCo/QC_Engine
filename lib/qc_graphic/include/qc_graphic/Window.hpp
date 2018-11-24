#pragma once

#include <GLFW/glfw3.h>

#include <string>
#include <chrono>

namespace qc
{

class Window
{
public:
    // Constructors
    Window();
    Window(int width, int height, const std::string & name);

    ~Window();

    // Getters / Setters
    bool    getShouldClose();
    float   getFrameDuration();
    float   getFps();

    // Functions
    bool init();
    void update();

private:
    // Properties
    GLFWwindow*     glwfWindow;
    int             width;
    int             height;
    std::string     name;
    
    // Events
    bool            shouldClose;

    // Duration datas
    std::chrono::system_clock::time_point   initClock;
    std::chrono::system_clock::time_point   prevFrameClock;
    float                                   frameDuration;  // In ms
    float                                   fps;

};

};  //! namespace qc

