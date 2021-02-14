#include "UIWindow.h"
#include "Application.h"
#include "PlugTypes.h"
#include "UIViewPortController.h"
#include <iostream>

using namespace std;

UIWindow::UIWindow(int width, int height, std::string windowTitle)
    : UIView(this, width, height, windowTitle) // passing this will cause the UIView
    // contructor to set itself as the root
{
    // first we set the application window which means the active one is set if one wasn't before
    Application::addWindow(this);

    Application::registerView(this, isRootWindow());


    cout << "UIWindow Constructor " << windowTitle << endl;
    globalViewCount = 0; // 0 is our windows so other views will be above that
    setDrawable(false);
    vUIViewGlobalStore.clear();

    cout << "wondow fixed GID: " << globalIndexID << "\n";
    cout << "window fixed PID: " << parent->id() << "\n";

    drawIDColour[0] = globalIndexID % 256;
    drawIDColour[1] = (globalIndexID >> 8) % 256;
    drawIDColour[2] = (globalIndexID >> 16) % 256;

    vpCntlr = std::make_unique<UIViewPortController>(this, viewRect.size.width,
    viewRect.size.height, HORIZONTAL);
    //cout << "about to reg first UIVPC" << std::endl;

    ////registerView(vpCntlr.get());
    //vpCntlr->divide(0.06);

    ////cout << "about to create 2nd UIVPC" << std::endl;

    //UIViewPortController* testCntlr = new UIViewPortController(this, 0.0, 0.0, VERTICAL);
    //cout << "about to add 2nd UIVPC as sub" << std::endl;

    //vpCntlr->addSubViewPortController(testCntlr, 0);
    //testCntlr->divide(0.1);

    //UIViewPortController* testCntlr2 = new UIViewPortController(this, 0.0, 0.0, VERTICAL);
    //testCntlr->addSubViewPortController(testCntlr2, 1);
    //testCntlr2->divide(0.6);

    //UIViewPortController* testCntlr3 = new UIViewPortController(this, 0.0, 0.0, HORIZONTAL);
    //testCntlr2->addSubViewPortController(testCntlr3, 1);
    //testCntlr3->divide(0.5);

    eventHandler = nullptr;
    programRunning = true;
}


void UIWindow::ForceRefresh()
{
    //resolveSize();
    //DrawGui();

}

void UIWindow::resetViewport()
{
    glViewport(0.0, 0.0, fbRect.size.width, fbRect.size.height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // cout << "frustrum right " << frustrum.right << std::endl;
    glOrtho(frustrum.left, frustrum.right, frustrum.bottom, frustrum.top, 100,
        -100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void UIWindow::DrawGui()
{

    resetViewport();

    // cout << "about to draw VPCntlr" << endl;
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    // glDisable(GL_TEXTURE_2D);

    //DrawSubViews();
    vpCntlr->Draw();

    resetViewport();

    // cout << "got here" << endl;
    // glTranslatef(0.0, 0.0, -1);

    // textEngine->render_text("The quick brown fox jumps",  0, 200, 1.0f, 1.0f);
    // textEngine->render_text("Yep sure is",  200.0f, 500.0f, 1.0f, 1.0f);
}

// void UIWindow::connectNodes(int outputNode_id, int inputNode_id, int
// fromPlugID,
//                             int toPlugID)

UIWindow::~UIWindow()
{
   cout << "in UIWindow destructor" << endl;
   for (auto x : vUIViewGlobalStore) {
       if ((x != NULL) && (x->globalIndexID != 0)) {
           cout << "deleting remaining UIView in global vector as a memory cleanup. "
                   "lets hope you saved!"
                << endl;
           delete x;
       }
   }
  // delete vpCntlr;
}

int UIWindow::id()
{
    return rootID;
}

GLFWwindow* UIWindow::getWindow() { return window; }

void UIWindow::registerView(UIView* newView)
{

    auto idAssigned = mUiViews.push(newView);
    newView->setId(idAssigned);
}

void UIWindow::addSubView(UIView* newView) {
    cout << "adding view as child of 0 on vpcontroller viewport\n";
    vpCntlr->addChildView(newView, 0);
}

void UIWindow::fixParentRelationShip()
{
    cout << "fixing ids. vp " << vpCntlr->id() << " having its parent set to " << this->id() << "\n";
    vpCntlr->setParent(this);
}


void UIWindow::deRegisterView(int id)
{
    mUiViews.eraseAt(id);
}

UIView* UIWindow::getNodeFromID(int id) { return mUiViews[id]; }

void UIWindow::setHandler(GLGui* eventHandlerPassThrough)
{
    eventHandler = eventHandlerPassThrough;
}

void UIWindow::handleEvent(keyStoreStruct key)
{

    if (key.key == GLFW_KEY_ESCAPE) {
        // programRunning = false;
        Application::quit();
        return;
    }
    // Mouse buttons on their own......
    // cout << "in event handler" << endl;
    switch (key.mouseActionStore) {
    case GLFW_PRESS: // MOUSE BUTTON CLICKED
        // cout << "got here" << endl;
        switch (key.buttonStore) {
        case GLFW_MOUSE_BUTTON_LEFT: // LMB PRESS

            // eventHandler->enableMousePosCallback();

            if (lmbPressed == false) // CLICK
            {
                lmbPressed = true;
                cout << "mousepos in event handler is " << key.Mx << " _ " << key.My
                     << endl;
                // draw select pass because nothing is selected right now (or shouldnt
                // be)
                selectedViewID = nodeIDUnderMousePos(key);
                cout << "left mouse button clicked" << endl;
                cout << "in UIWindow .about to call node " << selectedViewID
                     << " viewClicked" << endl;
                cout << mUiViews[selectedViewID]->text << "\n";
                mUiViews[selectedViewID]->viewClicked(key, 0);
            }

            if (lmbPressed == true) // DRAG
            {
                mUiViews[selectedViewID]->viewDragged(key, 0);
            }
            break;
        }
        break;

    case GLFW_RELEASE:
        switch (key.buttonStore) {
        case GLFW_MOUSE_BUTTON_LEFT: // LMB PRESSlmbPressed = false;
            // eventHandler->disableMousePosCallback();

            if (lmbPressed == true) {
                cout << "key released" << endl;
                lmbPressed = false;
                mUiViews[selectedViewID]->viewReleased(key, 0);
            }
            break;
        }
        break;
    }
}

int UIWindow::nodeIDUnderMousePos(keyStoreStruct key)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    checkOpenGLError();

    cout << "drawing select \n";

    DrawSelectPass();
    //Application::swapBuffers(rootID);
    cout << "drew select \n";

    unsigned char pRGB[4];

    glReadPixels(key.Mx * scaleFactor,
        (viewRect.size.height - key.My) * scaleFactor, 1, 1, GL_RGBA,
        GL_UNSIGNED_BYTE, pRGB);

    checkOpenGLError();

    int id = (pRGB[0]) + (pRGB[1] * 256) + (pRGB[2] * 256 * 256);
    if (!(id == 0)) {
        printf("selected view is id %d\n", id);
    } else {
        printf("selected background with id of %d\n", id);
    }

    glClearColor(0.5, 0.5, 0.5, 1.0);
    return id;
}

void UIWindow::checkOpenGLError()
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("Error! %s\n", gluErrorString(error));
    }
}
