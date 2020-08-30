#include "Application.h"
#include <iostream>
#include <future>

using namespace std;

std::vector<UIWindow*> Application::windows{new UIWindow(1024, 576)};
UIWindow* Application::mainWindow(Application::windows.front());

Application::Application() 
{

    appCore = std::make_unique<AppCore>();
    ;
    appCore->setViewController(this);

    appGui = std::make_unique<GLGui>();

    //mainWindow = new UIWindow(NULL, 1024, 576); //set windows to be 0 as it is stored by itself at index 0.

    // for now as we start this refactor, put the main window into the vec

    //register self
    mainWindow->registerView(mainWindow, (UIView*)this);
    mainWindow->setViewController(this);
    //mainWindow->setHandler(appGui);
    cout << "about to run UIController appgui init" << std::endl;
    mainWindow->InitGL("arrowtest");
    cout << "ran UIController appgui init" << std::endl;

    appGui->addWindow(mainWindow);
    bProgramRunning = true;

    cout << "Initialised. about to run UIController apploop" << std::endl;
}

void Application::addWindow(UIWindow* window){
    windows.push_back(window);
}


void Application::quit()
{

    bProgramRunning = false;
}

Application::~Application()
{

    //here is a bug!
    cout << "calling vc destructor" << endl;
    mainWindow->deRegisterChildren();
    delete mainWindow;
}

Node *Application::createNode()
{

    cout << "in UIVc createNode" << std::endl;
    Node *nodePointer = appCore->createNode();
    return nodePointer;
}

void Application::connectNodes(int outputNode_id, int inputNode_id, int fromPlugID, int toPlugID)
{ //connect two nodes specifying which nodes and which inputs....

    appCore->connectNodes(outputNode_id, inputNode_id, fromPlugID, toPlugID);
}

void Application::exec()
{
    std::vector<std::future<void>> windowfutures;

    for (auto &window : windows)
    {

        while (bProgramRunning)
        {

            processEvents();
            window->DrawGui();
            glfwSwapBuffers(window->getWindow());
            glfwWaitEvents();
        }
    }
}

void Application::callUINodeDraw()
{

    //cout << "calling node draw" << std::endl;
    appCore->callNodeDraw();
}

void Application::callNodeDrawSelect()
{

    appCore->callNodeDrawSelect();
}

void Application::setCurrentSelectedNode(int id)
{

    appCore->setCurrentSelectedNode(id);
}

void Application::setAppCore(std::unique_ptr<AppCore> appC)
{

    appCore = std::move(appC);
}

void Application::setGui(std::unique_ptr<GLGui> gui)
{

    appGui = std::move(gui);
}

void Application::processEvents()
{
    appGui->processEvents();
}

void Application::setCallBacks()
{

    appGui->setCallBacks(mainWindow->getWindow());
}

void Application::closeGUI()
{

    appGui->CloseGUI(1);
}
