#ifndef UIVIEWCONTROLLER_H
#define UIVIEWCONTROLLER_H


#include "GLGui.h"
//#include "SFMLHandler.h"
#include "AppCore.h"
#include "UIWindow.h"

//class AppCore;
//class GLGui;

// owns windows and connects app logic to ui

class UIViewController{


private:

    //window backend
    GLGui *appGui;

    // application always has at least one window
    UIWindow *mainWindow;
    std::vector<UIWindow*> windows;

    //this is just our test program. user programs would probably have this at a higher level, alongside
    // the application
    AppCore *appCore;

    bool bProgramRunning = false;


public:

    UIViewController();
    virtual ~UIViewController();

    void setGui(GLGui *gui);
    void setAppCore(AppCore *appC);

    void connectNodes(int outputNode_id, int inputNode_id, int fromPlugID, int toPlugID );
    void callUINodeDraw();
    void callNodeDrawSelect();
    void setCurrentSelectedNode(int id);
    void exec();                    //enter main loop. might swap this out for a app loop object. rename to exec like qt?
    Node* createNode();
    void quit();

    void connectEventToWindow();
    void processEvents();//get keySTore from GLGui and do something with it
    void setCallBacks();
    void closeGUI();


};










#endif
