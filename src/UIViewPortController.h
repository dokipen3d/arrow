
#ifndef UIVIEWPORTCONTROLLER_H
#define UIVIEWPORTCONTROLLER_H

#include "UIView.h"
#include "UIWindow.h"
#include "UIViewPort.h"

//for controlling the rows and coloums within a windows and handle the resizing of divides

class UIWindow;

class UIViewPortController: public UIView{


private:

    bool dragging;
    keyStoreStruct localKeyStore;
    orientation oriented;
    float dividerThickness;
    UIViewPort *viewLeft;
    UIViewPort *divider;
    UIViewPort *viewRight;
    float divPosition;
    float offsetSize;
    float divPercentLocation;

    void resizeFromDivider(int offsetX, int offsetY);

public:
    UIViewPortController(UIWindow *root, int width, int height, orientation orient):UIView(root, width, height) {
        dividerThickness = 8.0;
        oriented = orient;
        }

    ~UIViewPortController();

    virtual void Init();
    void divide(float divPC);
    void setOrientation(orientation orient);
    void viewClicked(keyStoreStruct key, int senderID);//default just calls parent viewClicked. if parent == NULL then do nothinng.
    void viewDragged(keyStoreStruct key, int senderID);
    void viewReleased(keyStoreStruct key, int senderID);
    void addChildView(UIView *newView, int index);
    void addSubViewPortController(UIViewPortController *newView, int index);
    void resizeGlobalSubViewPorts(); //recalc the rect (local and global) of subviews
    void resolveSize();
    bool DIVIDED;

};






#endif

