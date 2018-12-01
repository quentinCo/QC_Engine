#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <string>
#include <chrono>

namespace qc
{

class Window
{
public:
    // Constructors
    Window();
    Window(int width, int height, const std::string& name);

    ~Window();

    // Getters / Setters
    bool    getShouldClose() const;
    float   getFrameDuration() const;
    float   getFps() const;

    // Functions
    bool init();
#if 0
    bool init(const Window& mother);  // mother : primary window, the init window will be linked with the mother
#endif
    void update();

    // Statics
    static bool MakeCurrent(Window* window);
    static Window* GetCurrent();

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

    // Statics
    static Window*  currentWindow;


    // Functions
    bool initGlfwWindow();
    bool initOpenGL();
    void initClocks();


};

};  //! namespace qc

