#include "UIWindow.h"
#include <iostream>
#include "UIViewController.h"
#include "UIViewPortController.h"
#include "PlugTypes.h"

using namespace std;

void UIWindow::Init() {

  UIView::Init();



  rootWindow = this; // set to self so that when doing draw pass we draw from
                     // top down draw select pass needs pointer to root
                     // window(ourself)
  vpCntlr = new UIViewPortController(this, viewRect.size.width,
                                     viewRect.size.height, HORIZONTAL);
  registerView(vpCntlr, (UIView*)this);
  vpCntlr->divide(0.06);

  UIViewPortController *testCntlr =
      new UIViewPortController(this, 0.0, 0.0, VERTICAL);
  vpCntlr->addSubViewPortController(testCntlr, 0);
  testCntlr->divide(0.1);

  UIViewPortController *testCntlr2 =
      new UIViewPortController(this, 0.0, 0.0, VERTICAL);
  testCntlr->addSubViewPortController(testCntlr2, 1);
  testCntlr2->divide(0.6);

  UIViewPortController *testCntlr3 =
      new UIViewPortController(this, 0.0, 0.0, HORIZONTAL);
  testCntlr2->addSubViewPortController(testCntlr3, 1);
  testCntlr3->divide(0.5);

  eventHandler = NULL;
  parentViewID = -1; // parent is self
  programRunning = true;
}

void UIWindow::GLLoop() {

  cout << "in GLLoop" << std::endl;
  while (programRunning) {
    //

    viewController->processEvents();
    DrawGui();
    glfwSwapBuffers(window);
    glfwWaitEvents();

    // glfwPollEvents();
  }
  cout << "exiting loop" << endl;
}

void UIWindow::ForceRefresh() {

  DrawGui();
  glfwSwapBuffers(window);
  // glfwWaitEvents();
}

void UIWindow::resetViewport() {
  glViewport(0.0, 0.0, fbRect.size.width, fbRect.size.height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // cout << "frustrum right " << frustrum.right << std::endl;
  glOrtho(frustrum.left, frustrum.right, frustrum.bottom, frustrum.top, 100,
          -100);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void UIWindow::DrawGui() {

  resetViewport();

  // cout << "about to draw VPCntlr" << endl;
  // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);

  // glDisable(GL_TEXTURE_2D);

  vpCntlr->Draw();

  // resetViewport();

  // textEngine->render_text("The quick brown fox jumps",  0, 250, 1.0f, 1.0f);
  // textEngine->render_text("Yep sure is",  200.0f, 500.0f, 1.0f, 1.0f);

  // cout << "got here" << endl;
  // glTranslatef(0.0, 0.0, -1);
}

void UIWindow::connectNodes(int outputNode_id, int inputNode_id, int fromPlugID,
                            int toPlugID) {
  viewController->connectNodes(outputNode_id, inputNode_id, fromPlugID,
                               toPlugID);
}

UIWindow::~UIWindow() {
    cout  << "in UIWindow destructor" << endl;
    for (auto x : vUIViewGlobalStore){
        if ((x != NULL) && (x->globalIndexID != 0)){
            cout << "deleting remaining UIView in global vector as a memory cleanup. lets hope you saved!" << endl;
            delete x;
        }
    }
    //delete vpCntlr;
}

void UIWindow::setViewController(UIViewController *controller) {

  viewController = controller;
}

void UIWindow::InitGL(const char *name) {

  frustrum.centerX = 0;
  frustrum.centerY = 0;

  // checkOpenGLError();
  aspect_ratio = (viewRect.size.width / viewRect.size.height);

  cout << "aspect ratio is " << aspect_ratio << std::endl;

  // mainCore = mainAppCore;

  // 800 x 600, 16 bit color, no depth, alpha or stencil buffers, windowed
  // glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE );
  // glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

  // glfwOpenWindowHint ( GLFW_REFRESH_RATE, 60);
  cout << "setting glfw swap" << std::endl;

  glfwSwapInterval(1);
  window = glfwCreateWindow(viewRect.size.width, viewRect.size.height, "arrow",
                            NULL, NULL);
  glfwGetFramebufferSize(window, &fbRect.size.width, &fbRect.size.height);

  cout << fbRect.size.width << "";
  cout << fbRect.size.height << "";

  scaleFactor = (fbRect.size.width / viewRect.size.width);
  cout << " scale factor is " << scaleFactor << endl;
  glfwMakeContextCurrent(window);

  if (!window) {
    glfwTerminate();
    cout << "NOOOOOO!" << endl;
    exit(EXIT_FAILURE);
  }

  glewExperimental =
      GL_TRUE; // so that glew can getStringi and retrieve pointers
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    // Problem: glewInit failed, something is seriously wrong.
    cout << "glewInit failed, aborting." << endl;
  }
  // checkOpenGLError(); // still gets GL_INVALID_ENUM after glew though. can
  // safely ingore.
  glGetError(); // clear error

  // viewController->closeGUI();
  cout << "setting window name as " << name << std::endl;
  // glfwSetWindowTitle(name);
  viewController->setCallBacks();
  glViewport(0.0, 0.0, fbRect.size.width, fbRect.size.height);
  // checkOpenGLError();
  // set the projection matrix to a normal frustum with a max depth of 50

  // glOrtho((minusInitialWindowCoordWidth * zoomOut) * aspect_ratio ,
  // (initialWindowCoordWidth * zoomOut) * aspect_ratio,
  // (minusInitialWindowCoordWidth * zoomOut), (initialWindowCoordWidth *
  // zoomOut), 100, -100);// * aspect_ratio
  /*
  frustrum.left = 0-(viewRect.size.width/2.0);
  frustrum.right = 0+(viewRect.size.width/2.0);
  frustrum.bottom = 0+(viewRect.size.height/2.0);
  frustrum.top =  0-(viewRect.size.height/2.0);
  */

  frustrum.left = 0.0; //-(viewRect.size.width/2.0);
  frustrum.right = viewRect.size.width;
  ; //+(viewRect.size.width/2.0);
  frustrum.bottom = 0.0;
  frustrum.top = viewRect.size.height;
  cout << "first frustrum right " << frustrum.right << std::endl;
  cout << "viewRectWidth is " << viewRect.size.width << std::endl;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(frustrum.left, frustrum.right, frustrum.bottom, frustrum.top, 100,
          -100);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glClearColor(0.5, 0.5, 0.5, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  // glfwSwapBuffers();
  cout << "GLGui created" << std::endl;

  cout << "about to create text engine" << std::endl;

  // textEngine = new TextEngine();
  {}
  // textEngine->initResources();

  // windowShader = new ShaderObject("textShader.vert", "textShader.frag");
  // GLuint newProg = glCreateProgram();
}

GLFWwindow *UIWindow::getWindow() { return window; }

void UIWindow::registerView(UIView *newView, UIView *sender) {

  if (vUIViewGlobalSpare.size() > 0) { // if sparestack has something in it....
    int accessElement =
        vUIViewGlobalSpare.front(); // store the int value that is stored in the
                                    // last element of sparestack. this will be
                                    // the index to the vector position to fill
                                    // up.
    printf("spare index is %d\n",
           accessElement); // print the element to see it in console
    // this->viewIterator = vUIViewStore.begin();					//set iterator to start
    // of
    // vUIViewStore

    vUIViewGlobalStore.at(accessElement - 1) = newView;
    vUIViewGlobalSpare.pop_back();
    globalViewCount += 1;
    printf("spare on stack! view count is %d, vector size is %d and spareStack "
           "is %d\n",
           globalViewCount - 1, (int)vUIViewGlobalStore.size(),
           (int)vUIViewGlobalSpare.size());
    cout << "newly added view id is " << vUIViewGlobalStore.at(globalViewCount-1) << endl;
    newView->globalIndexID = accessElement - 1;
    newView->setParentID(sender->globalIndexID);
    newView->Init();
  }

  else {

    vUIViewGlobalStore.push_back(newView);
    cout << "assigning number to view -> " << globalViewCount << endl;
    newView->globalIndexID = globalViewCount;
    globalViewCount += 1;
    printf(
        "no spares.increasing vector. view count is %d and vector size is %d\n",
        globalViewCount, (int)vUIViewGlobalStore.size());
    cout << "newly added view id is " << vUIViewGlobalStore.at(globalViewCount-1)->globalIndexID << endl;
    cout << "first added view id is " << vUIViewGlobalStore.at(0)->globalIndexID << endl;

    newView->setParentID(sender->globalIndexID);
    newView->Init();
  }
}

void UIWindow::deRegisterView(int id) // delete a view. and all its children. if you want a child, take it out first!
{
    cout << "trying to deregister id " << id << " global store size is " << (int)vUIViewGlobalStore.size() << endl;

  // id less than vector size means it falls within range
  if ((vUIViewGlobalStore[id] != NULL) &&
      (id < vUIViewGlobalStore.size())) {
    // if UIView id is at the end
    if (id == (int)vUIViewGlobalStore.size()) {
      cout << "deleting last UIView in vector store" << endl;
      vUIViewGlobalStore.at(id)->deRegisterChildren();
      delete vUIViewGlobalStore.at(id);
      vUIViewGlobalStore.at(id) = NULL;
      vUIViewGlobalStore.pop_back();
      globalViewCount--;
      printf("UIviewCount is %d\n",
             (int)(globalViewCount - (this->spareRootIds.size())));
      printf("although vector is still size %d and vSpareStack is size %d\n",
             (int)vUIViewGlobalStore.size(), (int)this->vUIViewGlobalSpare.size());

    }

    else {
        cout << "else deleting" << endl;
      // this->nodeIterator = vNodeStore.begin();      //why iterate again?
        cout << "current id is " << globalIndexID << endl;
        cout << "size of global store is " << (int)vUIViewGlobalStore.size() << endl;
        cout << "deregistering id in global at id " << vUIViewGlobalStore[id]->globalIndexID << endl;
        vUIViewGlobalStore.at(id)->deRegisterChildren();
        delete vUIViewGlobalStore.at(id); // vNodeStore.erase(myIT+id-1);
                                         // //delete the element at the index
                                         // specified by id
        //deRegisterView(id);
        cout << "deleted node id " << id << endl;

      vUIViewGlobalSpare.push_back(id);        // add the spare slot to sparestack
      vUIViewGlobalStore[id] = NULL;
      globalViewCount--;
      printf("viewCount is %d.If 1 is left then it is the main window view last\n",
             (int)(globalViewCount - (this->spareRootIds.size())));
      printf("although vector is still size %d and vSpareStack is size %d\n",
             (int)vUIViewGlobalStore.size(), (int)this->vUIViewGlobalSpare.size());
    }

  } else
    printf("sorry node doesn't exist");
}

UIView *UIWindow::getNodeFromID(int id) { return vUIViewGlobalStore.at(id); }

void UIWindow::setHandler(GLGui *eventHandlerPassThrough) {
  eventHandler = eventHandlerPassThrough;
}

void UIWindow::handleEvent(keyStoreStruct key) {

  if (key.key == GLFW_KEY_ESCAPE) {
    programRunning = false;
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
        vUIViewGlobalStore.at(selectedViewID - 1)->printID();
        vUIViewGlobalStore.at(selectedViewID - 1)->viewClicked(key, 0);
      }

      if (lmbPressed == true) // DRAG
      {
        vUIViewGlobalStore.at(selectedViewID - 1)->viewDragged(key, 0);
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
        vUIViewGlobalStore.at(selectedViewID - 1)->viewReleased(key, 0);
      }
      break;
    }
    break;
  }
}

int UIWindow::nodeIDUnderMousePos(keyStoreStruct key) {
  // resetViewport();
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // glfwGetMousePos( &Mouse.Mx, &Mouse.My );
  DrawSelectPass();

  unsigned char *pRGB;

  pRGB = new unsigned char[4];

  glReadPixels(key.Mx * scaleFactor,
               (viewRect.size.height - key.My) * scaleFactor, 1, 1, GL_RGBA,
               GL_UNSIGNED_BYTE, pRGB);
  cout << "passed mouse value is " << key.Mx << endl;

  int id = (pRGB[0]) + (pRGB[1] * 256) + (pRGB[2] * 256 * 256);
  if (!(id == 0)) {
    // int nodeID = vUIViewGlobalStore.at(id-1)->getPointedToNodeID();
    // viewController->setCurrentSelectedNode(nodeID);
    printf("selected view is id %d\n", id);

  } else
    printf("selected background with id of %d\n", id);

  delete (pRGB);
  glClearColor(0.5, 0.5, 0.5, 1.0);
  return id;
}

void UIWindow::checkOpenGLError() {
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    printf("Error! %s\n", gluErrorString(error));
  }
}
