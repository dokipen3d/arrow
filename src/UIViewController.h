#ifndef UIVIEWCONTROLLER_H
#define UIVIEWCONTROLLER_H


#include "GLGui.h"
//#include "SFMLHandler.h"
#include "AppCore.h"
#include "UIWindow.h"

//class AppCore;
//class GLGui;

class UIViewController{


private:


    GLGui *appGui;
    UIWindow *mainWindow;
    AppCore *appCore;




public:

    UIViewController();
    virtual ~UIViewController();

    void setGui(GLGui *gui);
    void setAppCore(AppCore *appC);

    void connectNodes(int outputNode_id, int inputNode_id, int fromPlugID, int toPlugID );
    void callUINodeDraw();
    void callNodeDrawSelect();
    void setCurrentSelectedNode(int id);
    void appLoop();                    //enter main loop. might swap this out for a app loop object
    Node* createNode();

    void connectEventToWindow();
    void processEvents();//get keySTore from GLGui and do something with it
    void setCallBacks();
    void closeGUI();


};










#endif
