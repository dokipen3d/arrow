#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <string>
#include <vector>
#include "PlugTypes.h"
#include "sparestack.hpp"
#include <chrono>

#define GLFW_INCLUDE_GLU

class Node;
class UIWindow;


class GLGui {



private:

    UIWindow *eventWindow;
    UIWindow *userPointerWindow;
    static GLFWwindow* currentEventWindow;

    sparestack<GLFWwindow*> glfwWindows;
    static std::chrono::steady_clock::time_point last;



public:

    GLGui();
    virtual ~GLGui();

    static keyStoreStruct keyStore;
    static UIRect windowRect;
    static bool ready;

    // a flag to make sure we only resize one call at a time.
    static bool windowResized;
    static bool firstWindowCreated;
    void DrawGui();
    void CloseGUI(int return_code);
    void processEvents();//called in main loop and checkes key presses to maniuplate drawing of ui and perform functions. IE. interface!
    void addWindow(UIWindow *window);
    
    //GUI view stuff........

    // create a window and set it's user pointer to the int value given. doesn't need to get value back as it is up to application/UIWindow to destroy the windows with the ids
    void createWindow(std::size_t id, int width, int height, std::string title);
    void destroyWindow(std::size_t id);

    void makeWindowContextCurrent(std::size_t id);
    void swapBuffers(std::size_t id);
    void waitEvents();
    void waitEventsTimeout(double timeout);




    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void MousePosCallback(GLFWwindow* window, double xpos, double ypos);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void window_refresh_callback(GLFWwindow* window);

    void refresh();

    void resizeWindow(int width, int height);

    void setCallBacks(GLFWwindow *window);//safe to set them as window has been opened.

    void enableMousePosCallback(GLFWwindow *window);
    void disableMousePosCallback(GLFWwindow *window);

    void enableMouseButtonCallback(GLFWwindow *window);
    void disableMouseButtonCallback(GLFWwindow *window);

};