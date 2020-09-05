#include "Application.h"
#include "sparestack.hpp"
#include <future>
#include <iostream>
#include <memory>

// std::vector<UIWindow*> Application::windows{new UIWindow(1024, 576)};
// UIWindow* Application::mainWindow(Application::windows.front());

std::vector<UIWindow*> Application::windows{};
sparestack<UIWindow*> Application::windowstack{};

UIWindow* Application::mainWindow;
UIWindow* Application::activeWindow;

std::unique_ptr<GLGui> Application::appGui;
bool Application::bProgramRunning(true);

Application::Application() {

    appCore = std::make_unique<AppCore>();

    appGui = std::make_unique<GLGui>();
}

void Application::addWindow(UIWindow* window) {
    cout << "adding window" << std::endl;

    auto idgiven = windowstack.push(window);
    windowstack[idgiven]->rootID = idgiven;
    appGui->createWindow(idgiven, window->width(), window->height(),
                         window->text);
    if (!mainWindow) {
        mainWindow = window;
    }

    activeWindow = window;
}

std::unique_ptr<UIWindow> Application::createBasicWindow(int width, int height,
                                                         std::string title) {

    auto windowUptr = std::make_unique<UIWindow>(width, height, title);
    auto windowRawPtr = windowUptr.get();
    if (!mainWindow) {
        mainWindow = windowRawPtr;
        activeWindow = windowRawPtr;
    }

    // window stack takes ownership
    auto id = windowstack.push(windowRawPtr);
    appGui->createWindow(id, width, height, title);

    return std::move(windowUptr);
}

void Application::registerView(UIView* newView, bool selfRegister) {

    // if its a window self registering then we shoud then do that!
    if (selfRegister) {
        static_cast<UIWindow*>(newView)->registerView(newView);
    } else if (activeWindow) { // up to user to make sure that the right window
                               // is active
        activeWindow->registerView(newView);
    } else {
        // ?
    }
}

void Application::forceRefresh(std::size_t id) {

    windowstack[id]->ForceRefresh();

    windowstack[id]->DrawGui();
    //cout << "refresh\n";
}

void Application::framebuffer_size_callback(std::size_t id, int width,
                                            int height) {

    cout << width << " " << height << "\n";
    windowstack[id]->setSize(width, height);
    windowstack[id]->resolveSize();
    //appGui->swapBuffers(id);

}

void Application::quit() {

    bProgramRunning = false;
}

Application::~Application() {

    // here is a bug!
    cout << "calling vc destructor" << endl;
    // mainWindow->deRegisterChildren();
    // delete mainWindow;
}

Node* Application::createNode() {

    cout << "in UIVc createNode" << std::endl;
    Node* nodePointer = appCore->createNode();
    return nodePointer;
}

void Application::connectNodes(
    int outputNode_id, int inputNode_id, int fromPlugID,
    int toPlugID) { // connect two nodes specifying which nodes and which
                    // inputs....

    appCore->connectNodes(outputNode_id, inputNode_id, fromPlugID, toPlugID);
}

void Application::exec() {

    while (bProgramRunning) {

        for (auto i = 0; i < windowstack.size(); ++i) {
                //cout << "in loop " << i << "\n";
                appGui->makeWindowContextCurrent(i);
                // windowstack[i]->resolveSize();
                windowstack[i]->DrawGui();
                appGui->swapBuffers(i);
                
            // processEvents();
        }
        appGui->waitEvents();
    }
}

void Application::swapBuffers(std::size_t id) {
    appGui->swapBuffers(id);
}

void Application::setActiveWindow(UIWindow* window) {
    activeWindow = window;
}

void Application::callUINodeDraw() {

    // cout << "calling node draw" << std::endl;
    appCore->callNodeDraw();
}

void Application::callNodeDrawSelect() {

    appCore->callNodeDrawSelect();
}

void Application::setCurrentSelectedNode(int id) {

    appCore->setCurrentSelectedNode(id);
}

void Application::setAppCore(std::unique_ptr<AppCore> appC) {

    appCore = std::move(appC);
}

void Application::setGui(std::unique_ptr<GLGui> gui) {

    appGui = std::move(gui);
}

void Application::processEvents() {
    appGui->processEvents();
}

void Application::handleEvent(std::size_t id, keyStoreStruct keyStore) {
    windowstack[id]->handleEvent(keyStore);
}

void Application::closeGUI() {

    appGui->CloseGUI(1);
}
