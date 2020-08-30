#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <string.h>
#include <vector>
#include "PlugTypes.h"

#define GLFW_INCLUDE_GLU

using namespace std;

class Node;
class UIWindow;


class GLGui {



private:

    UIWindow *eventWindow;
    UIWindow *userPointerWindow;



public:

    GLGui();
    virtual ~GLGui();

    static keyStoreStruct keyStore;
    static UIRect windowRect;

    // a flag to make sure we only resize one call at a time.
    static bool windowResized;
    void DrawGui();
    void CloseGUI(int return_code);
    void processEvents();//called in main loop and checkes key presses to maniuplate drawing of ui and perform functions. IE. interface!
    void addWindow(UIWindow *window);
    //GUI view stuff........

    void connectNodes(int outputNode_id, int inputNode_id, int fromPlugID, int toPlugID );

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