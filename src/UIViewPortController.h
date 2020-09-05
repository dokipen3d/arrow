#pragma once

#include "UIView.h"
//#include "UIViewPort.h"
#include "UIWindow.h"
#include <memory>

// for controlling the rows and coloums within a windows and handle the resizing
// of divides

class UIWindow;

class UIViewPortController : public UIView {

  private:
    bool dragging;
    keyStoreStruct localKeyStore;
    orientation oriented;
    float dividerThickness;
    std::unique_ptr<UIView> viewLeft;
    std::unique_ptr<UIView> divider;
    std::unique_ptr<UIView> viewRight;
    float divPosition;
    float offsetSize;
    float divPercentLocation;

    void resizeFromDivider(int offsetX, int offsetY);

  public:
    UIViewPortController(UIView* parent, orientation orient,
                         std::string text = "vpCntrl");
    UIViewPortController(UIView* parent, int width, int height,
                         orientation orient, std::string text = "vpCntrl");
    UIViewPortController(UIViewPortController* parent, int positionIndex,
                         orientation orient, std::string text = "vpCntrl");

    ~UIViewPortController();
    int width(int position);
    int height(int position);
    UIView* viewAtIndex(int position);

    void divide(float divPC);
    void setOrientation(orientation orient);
    void viewClicked(keyStoreStruct key,
                     int senderID); // default just calls parent viewClicked. if
                                    // parent == NULL then do nothinng.
    void viewDragged(keyStoreStruct key, int senderID);
    void viewReleased(keyStoreStruct key, int senderID);
    void addChildView(UIView* newView, int index);
    void addSubViewPortController(UIViewPortController* newView, int index);
    void resizeGlobalSubViewPorts(); // recalc the rect (local and global) of
                                     // subviews
    void resolveSize() override;
    void setColour(int pos, float R, float G, float B, float A);
    bool m_bDivided;
    // no op as vpcntre doesn't really get drawn. just draws children
    //void DrawSelectPass() override;
    //void Draw() override;
    //int width() override {
    //    return width(0);
    //};
    //int height() override {
    //    return height(0);
    //};
    // pass through
    UIPoint getWorldPos() override;
};

