#pragma once


#include "GLGui.h"
#include <memory>
#include "AppCore.h"
#include "UIWindow.h"

// owns windows and connects app logic to ui

class Application{


private:

    //window backend
    std::unique_ptr<GLGui> appGui;

    // application always has at least one window
    static std::vector<UIWindow*> windows;
    static UIWindow *mainWindow;

    //this is just our test program. user programs would probably have this at a higher level, alongside
    // the application
    std::unique_ptr<AppCore> appCore;

    bool bProgramRunning = false;


public:

    Application();
    virtual ~Application();

    static void addWindow(UIWindow* window);

    void setGui(std::unique_ptr<GLGui> gui);
    void setAppCore(std::unique_ptr<AppCore> appC);

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

