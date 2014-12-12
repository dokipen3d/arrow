#ifndef UIWINDOW_H
#define UIWINDOW_H

#include "UIView.h"
//#include "UIViewPort.h"
#include "PlugTypes.h"
#include "GLGui.h"

#include "TextEngine.h"
#include "ShaderObject.h"

#include <iostream>

//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderObject.h"


using namespace std;

//base class
class UIViewController;
class UIViewPortController;

class UIWindow : public UIView{

private:

    //OPENGL STUFF
    GLfloat aspect_ratio;
    vector<UIView*> vUIViewGlobalStore;//all view including children are stored here so we can grab selection and forward messages accordingly
    vector<int> vUIViewGlobalSpare;//index of spare slots to fill ids

    vector<int> rootViews; //index of view in vUIGlobalStore that we need to render.
    vector<int> spareRootIds;//yada yada yada
    vector<int>::iterator drawIterator;

    int globalViewCount;
    int returnViewCount();
    int selectedViewID;

    UIViewController *viewController;
    frustrumStruct frustrum;

    UIViewPortController *vpCntlr; //store a pointer to the app viewport controller
    GLGui *eventHandler;// points to already existing handler.

    GLFWwindow* window;
    ShaderObject *windowShader;
    void checkOpenGLError();





public:
    UIWindow(UIWindow *root, int width, int height):UIView(root, width, height) {

        cout << "UIWindow Constructor" << endl;
        globalViewCount = 0; // 0 is our windows so other views will be above that
        setDrawable(false);


    }


    ~UIWindow();

    void ResizeWindow( int width, int height); // GLGui will call this after it handles event.

    float scaleFactor;

    virtual void Init();
    void GLLoop();
    void InitGL(const char *name);
    void DrawGui();
    void setViewController(UIViewController *controller); // can dynamically change viewController so we can have different behaviour at runtime

    void registerView(UIView *newView, UIView *sender); //add view to list and get id assigned and invoke init
    void deRegisterView(int id); //delete a view.

    UIView* getNodeFromID(int id);

    const char *guiName;
    void connectNodes(int outputNode_id, int inputNode_id, int fromPlugID, int toPlugID ); //connect two nodes specifying which nodes and which inputs....

    void handleEvent(keyStoreStruct key);
    void setHandler(GLGui *eventHandlerPassThrough);

    int nodeIDUnderMousePos(keyStoreStruct key);

    void resetViewport();

    GLFWwindow* getWindow();
    TextEngine* textEngine;

};






#endif
