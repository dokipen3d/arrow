#include "UIWindow.h"
#include "Application.h"
#include "PlugTypes.h"
#include "UIViewPortController.h"
#include <iostream>

using namespace std;

UIWindow::UIWindow(int width, int height, std::string windowTitle)
    : UIView(this, width, height,
             windowTitle) // passing this will cause the UIView
// contructor to set itself as the root
{
    // first we set the application window which means the active one is set if
    // one wasn't before
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

    vpCntlr = std::make_unique<UIViewPortController>(
        this, viewRect.size.width, viewRect.size.height, HORIZONTAL);

    
    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(UIColour) * 2000, nullptr,
                 GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0); 

    glGenBuffers(1, &CBO);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(UIColour) * 2000, nullptr,
                 GL_DYNAMIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(1); 

    //glGenBuffers(1, &SBO);
    //glBindBuffer(GL_ARRAY_BUFFER, SBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(UIColour) * 2000, nullptr,
    //             GL_DYNAMIC_DRAW);

    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
    //                      (void*)0);
    //glEnableVertexAttribArray(2); 


    mProgramID = glCreateProgram();

    static std::string vertShaderTriangle = R"VERT( 
        #version 330 core
    
        layout (location = 0) in vec3 viewRect;
        layout (location = 1) in vec3 colours;
        layout (location = 2) in vec3 selectSolours;
        out vec3 colour;
        out vec3 selectColour;

        uniform mat4 transform;

        void main()
        {
            colour = colours;
            gl_Position = transform * vec4(viewRect,  1);
        }
    )VERT";

    static std::string fragTest = R"FRAG(
        #version 330 core
        
        in vec3 colour;
        out vec3 selectColour;

        out vec4 frag_colour;

        uniform float selectLerp;
                
        void main () {
            frag_colour = vec4(colour, 1.0f);
        }
    )FRAG";


    const char* ctr2 = vertShaderTriangle.c_str();
    const char* frag = fragTest.c_str();

    GLuint vertexShader =
        glCreateShader(GL_VERTEX_SHADER); // Create vertex shader
    glShaderSource(vertexShader, 1, &ctr2, NULL);
    glCompileShader(vertexShader);
    checkShader(vertexShader, "vertex");
    glAttachShader(mProgramID, vertexShader); // Attach vertex shader to program

    GLuint fragmentShader =
        glCreateShader(GL_FRAGMENT_SHADER);         // Create fragment shader
    glShaderSource(fragmentShader, 1, &frag, NULL); // Set fragment source
    checkShader(fragmentShader, "fragment");
    CheckGLError("shaders compiled");
    glAttachShader(mProgramID, fragmentShader);
    CheckGLError("attach frag");

    glLinkProgram(mProgramID);
    CheckGLError("shaders linked");

    glUseProgram(mProgramID);
    CheckGLError("use program");



    // cout << "about to reg first UIVPC" << std::endl;

    ////registerView(vpCntlr.get());
    // vpCntlr->divide(0.06);

    ////cout << "about to create 2nd UIVPC" << std::endl;

    // UIViewPortController* testCntlr = new UIViewPortController(this, 0.0,
    // 0.0, VERTICAL); cout << "about to add 2nd UIVPC as sub" << std::endl;

    // vpCntlr->addSubViewPortController(testCntlr, 0);
    // testCntlr->divide(0.1);

    // UIViewPortController* testCntlr2 = new UIViewPortController(this, 0.0,
    // 0.0, VERTICAL); testCntlr->addSubViewPortController(testCntlr2, 1);
    // testCntlr2->divide(0.6);

    // UIViewPortController* testCntlr3 = new UIViewPortController(this, 0.0,
    // 0.0, HORIZONTAL); testCntlr2->addSubViewPortController(testCntlr3, 1);
    // testCntlr3->divide(0.5);

    eventHandler = nullptr;
    programRunning = true;

    textEngine = std::make_unique<TextEngine>();
    textEngine->initResources();
}

void UIWindow::ForceRefresh() {
    
    //if ( dragSkip == 0) {
        //cout << "refreshed \n";
        DrawGui();
    //}
    

}

void UIWindow::resetViewport() {
    glViewport(0.0, 0.0, viewRect.size.width, viewRect.size.height);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();

    //// cout << "frustrum right " << frustrum.right << std::endl;
    //glOrtho(frustrum.left, frustrum.right, frustrum.bottom, frustrum.top, 100,
    //        -100);

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
}

void UIWindow::DrawGui() {

    

    rectPoints.clear();
    rectPoints.reserve((mUiViews.size()) * 6);
    
    // rectPoints.push_back(globalRect);
    colours.clear();
    colours.reserve((mUiViews.size()) * 6);
    CheckGLError("begin");



    glClearColor(0.5, 0.5, 0.5, 1.0);
    CheckGLError("set clear color");


    // cout << "about to draw VPCntlr" << endl;
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClear(GL_COLOR_BUFFER_BIT);
    CheckGLError("set clear");


    // glDisable(GL_TEXTURE_2D);
    //resolveSize();
    // DrawSubViews();
    vpCntlr->Draw();


    updateGLBuffersAndDraw();
    CheckGLError("drewSomething!");

    

    //resetViewport();

    // cout << "got here" << endl;
    // glTranslatef(0.0, 0.0, -1);
 /*   rootWindow->textEngine->render_text("view is the\n best", 0.0, 0.0, 1.0,
                                        1.0);*/
    //textEngine->render_text("The quick brown fox jumps",  0,
    // 200, 1.0f, 1.0f); textEngine->render_text("Yep sure is",  200.0f,
    // 500.0f, 1.0f, 1.0f);
}

void UIWindow::updateGLBuffersAndDraw() {

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    CheckGLError("just bound 1st buffer");

    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    sizeof(UIColour) * rectPoints.size(),
                    rectPoints.data());
    CheckGLError("just sent 1st buffer data");

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
    //                      (void*)0);
    CheckGLError("just set pointer");

    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(UIColour) * colours.size(),
                    colours.data());

    /*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);*/
    CheckGLError("just set pointer 2");
    CheckGLError("sub 2nd");
    glBindVertexArray(VAO);
    glUseProgram(mProgramID);
   

    glm::mat4 projMat = glm::ortho(0.f, (float)viewRect.size.width, 0.f,
                                   (float)viewRect.size.height, -1.f, 1.f);

    unsigned int transformLoc = glGetUniformLocation(mProgramID, "transform");

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(projMat));

    resetViewport();
    CheckGLError(" just reset");
    //cout << rectPoints.size() << "\n";
    glDrawArrays(GL_TRIANGLES, 0, rectPoints.size());
    CheckGLError("drew");
}

void UIWindow::drawImmediate() {
    glDrawArrays(GL_TRIANGLES, 0, rectPoints.size());
    CheckGLError("drew");
}

// void UIWindow::connectNodes(int outputNode_id, int inputNode_id, int
// fromPlugID,
//                             int toPlugID)

UIWindow::~UIWindow() {
    cout << "in UIWindow destructor" << endl;
    for (auto x : vUIViewGlobalStore) {
        if ((x != NULL) && (x->globalIndexID != 0)) {
            cout << "deleting remaining UIView in global vector as a memory "
                    "cleanup. "
                    "lets hope you saved!"
                 << endl;
            delete x;
        }
    }
    // delete vpCntlr;
}

int UIWindow::id() {
    return rootID;
}

GLFWwindow* UIWindow::getWindow() {
    return window;
}

void UIWindow::registerView(UIView* newView) {

    auto idAssigned = mUiViews.push(newView);
    newView->setId(idAssigned);
}

void UIWindow::addSubView(UIView* newView) {
    cout << "adding view as child of 0 on vpcontroller viewport\n";
    vpCntlr->addChildView(newView, 0);
}

void UIWindow::fixParentRelationShip() {
    cout << "fixing ids. vp " << vpCntlr->id() << " having its parent set to "
         << this->id() << "\n";
    vpCntlr->setParent(this);
}

void UIWindow::deRegisterView(int id) {
    mUiViews.eraseAt(id);
}

UIView* UIWindow::getNodeFromID(int id) {
    return mUiViews[id];
}

void UIWindow::setHandler(GLGui* eventHandlerPassThrough) {
    eventHandler = eventHandlerPassThrough;
}

void UIWindow::handleEvent(keyStoreStruct key) {

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
                cout << "mousepos in event handler is " << key.Mx << " _ "
                     << key.My << endl;
                // draw select pass because nothing is selected right now (or
                // shouldnt be)
                selectedViewID = nodeIDUnderMousePos(key);
                cout << "left mouse button clicked" << endl;
                cout << "in UIWindow .about to call node " << selectedViewID
                     << " viewClicked" << endl;
                cout << mUiViews[selectedViewID]->text << "\n";
                mUiViews[selectedViewID]->viewClicked(key, 0);
            }

            if (lmbPressed == true) // DRAG
            {
                //cout << "dragging \n"; 
                //if (dragSkip == 0) {
                    mUiViews[selectedViewID]->viewDragged(key, 0);
                //}
                dragSkip++;
                dragSkip = dragSkip % 3;
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

int UIWindow::nodeIDUnderMousePos(keyStoreStruct key) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    checkOpenGLError();

    
    rectPoints.clear();
    rectPoints.reserve((mUiViews.size()) * 6);
    
    colours.clear();
    colours.reserve((mUiViews.size()) * 6);
    DrawSelectPass();
    updateGLBuffersAndDraw();

    unsigned char pRGB[4];

    glReadPixels(key.Mx * scaleFactor,
                 (viewRect.size.height - key.My) * scaleFactor, 1, 1, GL_RGBA,
                 GL_UNSIGNED_BYTE, pRGB);

    checkOpenGLError();

    cout << "cols " << (int)pRGB[0] << " " << (int)pRGB[1] << " " << (int)pRGB[2]
         << "\n";

    int id = (pRGB[0]) + (pRGB[1] * 256) + (pRGB[2] * 256 * 256);
    if (!(id == 0)) {
        cout << "selected view is id " << id;
    } else {
        printf("selected background with id of %d\n", id);
    }

    glClearColor(0.5, 0.5, 0.5, 1.0);
    return id;
}

void UIWindow::checkOpenGLError() {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("Error! %s\n", gluErrorString(error));
    }
}
