#ifndef GLGUI_H
#define GLGUI_H



#ifdef _WIN32
#include <GL/glew.h>
//#define GLEW_STATIC
#include <GLFW/glfw3.h>
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include <string.h>
#include <vector>
#include "PlugTypes.h"
//#include "UIViewController.h"

#define GLFW_INCLUDE_GLU

using namespace std;

class UIViewController;
class Node;
class UIWindow;


class GLGui {



private:

    UIViewController *viewController;
    UIWindow *eventWindow;
    UIWindow *userPointerWindow;



public:

    GLGui();
    virtual ~GLGui();

    static keyStoreStruct keyStore;
    static UIRect windowRect;
    static bool windowResized;
    void DrawGui();
    void CloseGUI(int return_code);
    void processEvents();//called in main loop and checkes key presses to maniuplate drawing of ui and perform functions. IE. interface!
    void addWindow(UIWindow *window);
    //GUI view stuff........

    void connectNodes(int outputNode_id, int inputNode_id, int fromPlugID, int toPlugID );
    void setViewController(UIViewController *controller); // can dynamically change viewController so we can have different behaviour at runtime

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

#endif // GLGUI_H
