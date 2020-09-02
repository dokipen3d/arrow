#pragma once


#include "GLGui.h"
#include <memory>
#include "AppCore.h"
#include "UIWindow.h"
#include "sparestack.hpp"

// owns windows and connects app logic to ui

class Application{


private:

    //window backend
    static std::unique_ptr<GLGui> appGui;

    // application always has at least one window
    static std::vector<UIWindow*> windows;
    static sparestack<UIWindow*> windowstack;
    //this is just our test program. user programs would probably have this at a higher level, alongside
    // the application
    std::unique_ptr<AppCore> appCore;

    static bool bProgramRunning;


public:

    Application();
    virtual ~Application();

    // if this is closed, the app closes
    static UIWindow *mainWindow;
    static UIWindow *activeWindow;


    static void addWindow(UIWindow* window);
    static void addWindow2(UIWindow* window);

    static std::unique_ptr<UIWindow> createBasicWindow(int width, int height, std::string title );

    // calls the register view of the activeWindow
    static void Application::registerView(UIView* newView, bool selfRegister = false);
    static void Application::forceRefresh(std::size_t id);
    static void Application::framebuffer_size_callback(std::size_t id, int width, int height);
    static void handleEvent(std::size_t id, keyStoreStruct keyStore);
    static void swapBuffers(std::size_t id);

    void setGui(std::unique_ptr<GLGui> gui);
    void setAppCore(std::unique_ptr<AppCore> appC);

    void connectNodes(int outputNode_id, int inputNode_id, int fromPlugID, int toPlugID );
    void callUINodeDraw();
    void callNodeDrawSelect();
    void setCurrentSelectedNode(int id);
    void exec();                    //enter main loop. might swap this out for a app loop object. rename to exec like qt?
    Node* createNode();
    static void quit();

    void connectEventToWindow();
    void processEvents();//get keySTore from GLGui and do something with it
    void closeGUI();


};

