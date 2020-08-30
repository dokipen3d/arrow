#include "GLGui.h"
#include <string.h>

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "Application.h"
#include "Node.h"
#include "UIWindow.h"

//class AppCore;

keyStoreStruct GLGui::keyStore;
UIRect GLGui::windowRect;
int globalViewCount;
bool GLGui::windowResized;

GLGui::GLGui(){

    if (glfwInit() != GL_TRUE)  {
        CloseGUI(1);            }
    windowResized = false;

    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // glfwSwapInterval(1);
    // glewExperimental = GL_TRUE; // so that glew can getStringi and retrieve pointers
    // GLenum err = glewInit();
    // if (err != GLEW_OK)
    // {
    //     // Problem: glewInit failed, something is seriously wrong.
    //     cout << "glewInit failed, aborting." << endl;
    // }
    // // checkOpenGLError(); // still gets GL_INVALID_ENUM after glew though. can
    // // safely ingore.
    // glGetError(); // clear error

}

GLGui::~GLGui(){

}

void GLGui::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)  {

	GLGui::keyStore.key = key;
	GLGui::keyStore.keyActionStore = action;           }

void GLGui::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods){//here set doSelection to true and do the call in the app loop. not meant to do opengl stuff in callbacks.

	GLGui::keyStore.buttonStore = button;
	GLGui::keyStore.mouseActionStore = action;
	        }

void GLGui::MousePosCallback (GLFWwindow* window, double xpos, double ypos){

    GLGui::keyStore.Mx = xpos;
    GLGui::keyStore.My = ypos;
    //cout << "mouse pos callback called" << endl;
    //cout << "pos is " << x << "and " << y <<endl;
}

void GLGui::processEvents(){//called in main loop and forwards events to windows for further handling

    //if multiple windows then iterate through window refrences and send them. one window for now.
    eventWindow->handleEvent(keyStore);

    if (windowResized == true)
    {
        std::cout << "boo\n";
        eventWindow->setSize(windowRect.size.width, windowRect.size.height);
        windowResized = false;
    }



}

void GLGui::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    GLGui::windowRect.size.width = width;
    GLGui::windowRect.size.height = height;
    UIWindow *currentWindow = static_cast<UIWindow*>(glfwGetWindowUserPointer(window));
    currentWindow->setSize(windowRect.size.width, windowRect.size.height);

    windowResized = true;
}


//to get around calling non static functions glfw lets us store pointers
void GLGui::window_refresh_callback(GLFWwindow* window){
    UIWindow *currentWindow = static_cast<UIWindow*>(glfwGetWindowUserPointer(window));
    currentWindow->ForceRefresh();

}

void GLGui::refresh(){

    eventWindow->ForceRefresh();

}

void GLGui::resizeWindow(int width, int height)
{




}

void GLGui::addWindow(UIWindow *window){

    //here maybe add UIwindow id checking so we can forwad messages to correct window
    GLGui::eventWindow = window;
    glfwSetWindowUserPointer(window->getWindow(), window);

}



void GLGui::CloseGUI(int return_code){
  glfwTerminate();
  exit(return_code);
}

void GLGui::setCallBacks(GLFWwindow *window){


    glfwSetKeyCallback(window, GLGui::keyCallback);
	glfwSetMouseButtonCallback(window, GLGui::MouseButtonCallback);
	glfwSetCursorPosCallback(window, GLGui::MousePosCallback);
    glfwSetWindowSizeCallback(window, GLGui::framebuffer_size_callback);
    glfwSetWindowRefreshCallback(window, GLGui::window_refresh_callback);

}

void GLGui::enableMousePosCallback(GLFWwindow *window)
{
    glfwSetCursorPosCallback(window, GLGui::MousePosCallback);
}

void GLGui::disableMousePosCallback(GLFWwindow *window)
{
    glfwSetCursorPosCallback(window, NULL);
}

void GLGui::enableMouseButtonCallback(GLFWwindow *window)
{
    glfwSetMouseButtonCallback(window, GLGui::MouseButtonCallback);
}

void GLGui::disableMouseButtonCallback(GLFWwindow *window)
{
    glfwSetMouseButtonCallback(window, NULL);

}

