#include "UIViewController.h"
#include <iostream>

using namespace std;

UIViewController::UIViewController()
{

    appCore = new AppCore;
    appCore->setViewController(this);

    appGui = new GLGui;
    appGui->setViewController(this);

    appCore->setViewController(this);

    mainWindow = new UIWindow(NULL, 1024, 576); //set windows to be 0 as it is stored by itself at index 0.
    
    // for now as we start this refactor, put the main window into the vec
    windows.push_back(mainWindow);
    
    //register self
    mainWindow->registerView(mainWindow, (UIView *)this);
    mainWindow->setViewController(this);
    //mainWindow->setHandler(appGui);
    cout << "about to run UIController appgui init" << std::endl;
    mainWindow->InitGL("arrowtest");
    cout << "ran UIController appgui init" << std::endl;

    appGui->addWindow(mainWindow);
    bProgramRunning = true;


    cout << "Initialised. about to run UIController apploop" << std::endl;
}

void UIViewController::quit(){

    bProgramRunning = false;
}

UIViewController::~UIViewController()
{

    delete appCore;
    delete appGui;
    //here is a bug!
    cout << "calling vc destructor" << endl;
    mainWindow->deRegisterChildren();
    delete mainWindow;
}

Node *UIViewController::createNode()
{

    cout << "in UIVc createNode" << std::endl;
    Node *nodePointer = appCore->createNode();
    return nodePointer;
}

void UIViewController::connectNodes(int outputNode_id, int inputNode_id, int fromPlugID, int toPlugID)
{ //connect two nodes specifying which nodes and which inputs....

    appCore->connectNodes(outputNode_id, inputNode_id, fromPlugID, toPlugID);
}

void UIViewController::exec()
{
    while (bProgramRunning)
    {
        for (auto& window : windows){
            processEvents();
            window->DrawGui();
            glfwSwapBuffers(window->getWindow());
            glfwWaitEvents();
        }
    }
}

void UIViewController::callUINodeDraw()
{

    //cout << "calling node draw" << std::endl;
    appCore->callNodeDraw();
}

void UIViewController::callNodeDrawSelect()
{

    appCore->callNodeDrawSelect();
}

void UIViewController::setCurrentSelectedNode(int id)
{

    appCore->setCurrentSelectedNode(id);
}

void UIViewController::setAppCore(AppCore *appC)
{

    appCore = appC;
}

void UIViewController::setGui(GLGui *gui)
{

    appGui = gui;
}

void UIViewController::processEvents()
{
    appGui->processEvents();
}

void UIViewController::setCallBacks()
{

    appGui->setCallBacks(mainWindow->getWindow());
}

void UIViewController::closeGUI()
{

    appGui->CloseGUI(1);
}
