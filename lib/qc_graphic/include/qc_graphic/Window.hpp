/*!
    @date 26/02/19
 */

#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <glm\vec2.hpp>

#include <string>
#include <chrono>

namespace qc
{
    /*!
        Window class allow to create a glfw window with an OpenGL context.<br>
        Example:
        @code 
            int main()
            {
                qc::Window  window = qc::Window(1600, 900, "Example");

                bool isInit = this->window.init();
                if (isInit == false)
                    return 0;
            
                while (this->window.getShouldClose() == false)
                {
                    this->window.update();
                }
                return 0;
            }
        @endcode 
     */
class Window
{
public:
    // Constructors
    /*!
        Default constuctor, create a window with height and width to 500 pixel.<br>
        @warning The window is not usable like this, you need to call init().
        @see init()
     */
    Window();
    
    /*!
        Create a glfw window.<br>
        @warning The window is not usable like this, you need to call init().
        @param[in] width The pixel width
        @param[in] height The pixel height
        @param[in] name The window name
        @param[in] quitKey A GLFW_KEY enum value to specify key to quit close the window (escape by default)

        @see init()
     */
    Window(int width, int height, const std::string& name, int quitKey = GLFW_KEY_ESCAPE);

    //! Destroy the window
    ~Window();

    // Getters / Setters
    
    //! @return Pointer on the GLFWWindow object.
    GLFWwindow* getGLFWwindow() const;

    //! @return Pixel width size.
    int         getWidth() const;
    
    //! @return Pixel height size.
    int         getHeight() const;

    //! @return Widow size as int vector.
    glm::ivec2  getViewportSize() const;

    /*!
        @return true if the user request to close the window, by click on window cross or quitKey.
        @see Window()
     */
    bool        getShouldClose() const;

    //! @return Frame duration in ms.
    float       getFrameDuration() const;

    //! @return Frame duration in second.
    float       getFrameDurationSecond() const;

    //! @return The frame rate.
    float       getFps() const;

    // Functions
    /*!
        This function init everything is needed to have a fonctionnal window:
            - init glwf functions
            - glfw window
            - make the inited window as current
            - initialise opengl.
            - clocks system for fps counters

        The function initialise two callback system too, with specific output message:
            - callback for windows errors
            - callback for opengl errors (glDebugMessageCallback : OpenGL 4.3)

        @return true if no error occure during the window initialisation.
     */
    bool init();
#if 0
    bool init(const Window& mother);  // mother : primary window, the init window will be linked with the mother
#endif
    /*!
        The function:
            - swaps the display buffer
            - update the event poll
            - update clocks windows
     */
    void update();

    // Statics
    /*!
        Make a window as the current window.
        @param window The desired window to make as current
        @return true if no problems occure
     */
    static bool MakeCurrent(Window* window);

    /*!
        @return Pointer on the current window.
     */
    static Window* GetCurrent();

private:
    // Properties
    GLFWwindow*     glwfWindow;
    int             width;
    int             height;
    std::string     name;
    
    int             quitKey;

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

