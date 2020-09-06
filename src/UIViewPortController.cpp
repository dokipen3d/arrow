#include "UIViewPortController.h"
#include "UIViewPort.h"
#include "UIWindow.h"
#include <iostream>

// class for controlling divisions of the screen. we can add further
// subdivisions to viewports through this class. we never actually get down to
// accessing viewports themselves. we add to them through the controller.

using namespace std;

UIViewPortController::UIViewPortController(UIView* parent, int width,
                                           int height, orientation orient,
                                           std::string text)
    : UIView(parent, width, height, text)
    , divider(nullptr)
    , viewRight(nullptr)
    , m_bDivided(false)
    , divPosition(0)
    , dragging(false)
    , dividerThickness(5.0)
    , oriented(orient)
    , divPercentLocation(1) {
    // set up main viewport. there is one by default before dividing.
    viewLeft =
        std::make_unique<UIView>(this, width, height, text + "_leftView");
    viewLeft->setColour(0.5, 0.0, 0.5, 1.0);
    expandToFillParent = true;
    setDrawable(false);
    // kind of need this in the constructor other wise we dont see the VP being
    // drawn
    resolveSize();
}

UIViewPortController::UIViewPortController(UIView* parent, orientation orient,
                                           std::string text)
    : UIViewPortController(parent, parent->width(), parent->height(), orient,
                           text) {
}

UIViewPortController::UIViewPortController(UIViewPortController* parent,
                                           int positionIndex,
                                           orientation orient, std::string text)
    : UIViewPortController(parent->viewAtIndex(positionIndex),
                           parent->viewAtIndex(positionIndex)->width(),
                           parent->viewAtIndex(positionIndex)->height(), orient,
                           text) {
    cout << "Adding VPCNTRLER " << text << "with VP parent "
         << parent->viewAtIndex(positionIndex)->text << " and width "
         << parent->viewAtIndex(positionIndex)->width() << " and height "
         << parent->viewAtIndex(positionIndex)->height() << "\n";
}

UIViewPortController::~UIViewPortController() {

    cout << "in vpctr de"
         << "\n";
    //    delete viewLeft;
    //    if(viewRight){
    //        delete viewRight;
    //    }
    //    if(divider){
    //        delete divider;
    //    }
}

UIView* UIViewPortController::viewAtIndex(int position) {
    if (position == 0) {
        return viewLeft.get();
    } else if (m_bDivided && position == 1) {
        cout << "returning view at right\n";
        return viewRight.get();
    } else {
        assert(false);
    }
}

int UIViewPortController::width(int position) {
    if (m_bDivided && position > 0) {
        return viewRight->width();
    } else {
        return viewLeft->width();
    }
}

int UIViewPortController::height(int position) {
    if (m_bDivided && position > 0) {
        return viewRight->height();
    } else {
        return viewLeft->height();
    }
}

void UIViewPortController::divide(float divPC) {

    m_bDivided = true;

    cout << "dividing"
         << "\n";

    if (oriented == VERTICAL) {
        divPercentLocation = divPC;

        if (divider == nullptr) {
            divider = std::make_unique<UIView>(
                this, dividerThickness, viewRect.size.height, text + "_div");
        }
        if (viewRight == nullptr) {
            viewRight =
                std::make_unique<UIView>(this, 0, 0, text + "rightView");
        }

        resolveSize();
    }

    if (oriented == HORIZONTAL) {
        divPercentLocation =
            1.0 - divPC; // reverse because of opengl y coordinate

        if (divider == nullptr) {
            divider = std::make_unique<UIView>(this, viewRect.size.width,
                                               dividerThickness, text + "div");
            divider->setColour(1.0, 0.5, 0.5, 1.0);
        }
        if (viewRight == nullptr) {
            viewRight = std::make_unique<UIView>(
                this, 0, 0, text + "bottom"); // create it with no size,
                                              // it wil get
            // resolved later on...
            // viewRight->setColour(0.5, 0.5, 0.0, 1.0);
        }

        resolveSize();
    }
}

UIPoint UIViewPortController::getWorldPos() {
    return parent->getWorldPos();
}

void UIViewPortController::resetViewport() {
    parent->resetViewport();
}

void UIViewPortController::resetViewportBegin() {
    parent->resetViewportBegin();
}

void UIViewPortController::resetViewportEnd() {
    parent->resetViewportEnd();
}

void UIViewPortController::resolveSize() {

    // set self to be parent viewports viewRect because it might not have been
    // set before
    viewRect = parent->getRect();
    globalRect.point = parent->getWorldPos();

    /*globalRect.point =
        UIPoint{viewRect.point.x + parentPos.x, viewRect.point.y + parentPos.y};*/


    // vpcontrolers always stay 0,0 as they get their worl pos from the parent
    viewRect.point = {0, 0};
    // cout << "resolving VPCNtrol " << id() << " from parent " << parent->id()
    // << " to " << viewRect.size.width << " " << viewRect.size.height << "\n";
    resizeGlobalSubViewPorts();
    for (auto& index : children) {
        rootWindow->getNodeFromID(index)->resolveSize();
    }
}

void UIViewPortController::setColour(int pos, float R, float G, float B,
                                     float A) {
    if (pos == 0) {
        viewLeft->setColour(R, G, B, A);
    } else if (m_bDivided && pos == 1) {
        viewRight->setColour(R, G, B, A);
    }
}

//void UIViewPortController::DrawSelectPass() {
//    for (auto it = children.begin(); it < children.end(); ++it) {
//        rootWindow->getNodeFromID(*it)->DrawSelectPass();
//    }
//}

//void UIViewPortController::Draw() {
//
//    for (auto it = children.begin(); it < children.end(); ++it) {
//        rootWindow->getNodeFromID((*it))->Draw();
//    }
//}



void UIViewPortController::
    resizeGlobalSubViewPorts() // for setting up sizes after division.
                               // another function takes cares of local
                               // resizing from mouse (but this might get
                               // called as a consequence of that other
                               // simpler function
{ // this doesn't need to call resolve size because the
  // caller (vpCntlr::resolveSize) does it

    // grab global pos of VPCtlr
   // UIPoint globalPosOfParentVPCtlr = getWorldPos();
    //globalRect = UIRect{globalPosOfParentVPCtlr, viewRect.size};
    // cout << "global Pos of VPCTlr " << id() << " is "
    //     << globalPosOfParentVPCtlr.x << " " << globalPosOfParentVPCtlr.y
    //     << "\n";
    float divPositionX = 0;
    float divPositionY = 0;
    float offsetSizeX = 0;
    float offsetSizeY = 0;
    float finalWidth = viewRect.size.width;
    float finalDividerWidth = viewRect.size.width;
    float finalHeight = viewRect.size.height;
    float finalDividerHeight = viewRect.size.height;

    // only override the needed variables to avoid having to call all
    // functions in two branches in future, we should make this a templated
    // function or if constexpr
    if (oriented == VERTICAL) {
        finalDividerWidth = dividerThickness;
        divPositionX = std::ceil(viewRect.size.width * divPercentLocation);
        finalWidth = divPositionX;
        offsetSizeX = divPositionX + dividerThickness;
    } else if (oriented == HORIZONTAL) {
        finalDividerHeight = dividerThickness;
        divPositionY = std::ceil(viewRect.size.height * divPercentLocation);
        finalHeight = divPositionY;
        offsetSizeY = divPositionY + dividerThickness;
    }

    if (children.size() == 1) // if its only one viewport....
    { // set it's size to be the VPCtlr viewRect...remember keep VPCtlrs pos
      // 0.0, 0.0....
        viewLeft->setPosition(0.0, 0.0);
        // cout << "setting left view to " << viewRect.size.width << " "
        //     << viewRect.size.height << "\n";
        viewLeft->setSize(viewRect.size.width, viewRect.size.height);
        // viewLeft->setGlobalPosition(globalPosOfParentVPCtlr.x,
        //                            globalPosOfParentVPCtlr.y);
        // cout << "resolving left view\n";
        // viewLeft->resolveSize();

    } else if (children.size() == 3) {
        // viewLeft->setColour(0.5, 0.0, 0.5, 1.0);
        viewLeft->setPosition(0.0, 0.0);
        viewLeft->setSize(finalWidth, finalHeight);
        // viewLeft->setGlobalPosition(globalPosOfParentVPCtlr.x,
        //                            globalPosOfParentVPCtlr.y);
        // viewLeft->resolveSize();

        divider->setPosition(divPositionX, divPositionY);
        divider->setSize(finalDividerWidth, finalDividerHeight);
        // divider->setGlobalPosition(globalPosOfParentVPCtlr.x,
        //                           globalPosOfParentVPCtlr.y);

        viewRight->setPosition(offsetSizeX, offsetSizeY);
        viewRight->setSize(viewRect.size.width - offsetSizeX,
                           viewRect.size.height - offsetSizeY);
        // viewRight->setGlobalPosition(globalPosOfParentVPCtlr.x +
        // offsetSizeX,
        //                             globalPosOfParentVPCtlr.y +
        //                             offsetSizeY);
        // viewRight->resolveSize();
    }
}

void UIViewPortController::setOrientation(orientation orient) {

    oriented = orient;
}

void UIViewPortController::addChildView(UIView* newView, int index) {

    // add it to the viewport that was created at div 1-2
    // dont need to resolve here as the new views will be resolved in their
    // contructor
    if (index == 0) {
        viewLeft->addSubView(newView);
    }

    if (index == 1) {
        viewRight->addSubView(newView);
    }
}

void UIViewPortController::addSubViewPortController(
    UIViewPortController* newView, int index) {

    // add it to the viewport that was created at div 1-2
    // set it to the size of that viewport if its another vpcntlr
    // dont need to resolve here as the new views will be resolved in their
    // contructor

    if (index == 0) {
        // format incoming viewportController size so that the viewRect
        // matches the viewport its going into.
        viewLeft->addSubView(newView);
    }

    if (index == 1) {
        viewRight->addSubView(newView);
    }
}

void UIViewPortController::viewClicked(keyStoreStruct key, int senderID) {

    // mouse click for dragging
    // if sender is less than my viewcount (ie is the caller one of my
    // children) if odd then its a divider. if even its a sub view that
    // doesnt reaarange. then store mouse pos and set mousebutton pressed.
    // cout << "viewport contrler recieved click" << endl;
    if ((senderID % 2) == 1) // called from divider, use mod incase we want
                             // to implement multiple divides
    {

        localKeyStore.LMx = key.Mx;
        localKeyStore.LMy = key.My;
        // cout << localKeyStore.LMx << endl;
        // cout << localKeyStore.LMy << endl;
    }
}

void UIViewPortController::viewDragged(keyStoreStruct key, int senderID) {
    if ((senderID % 2) == 1) {
        if (oriented == VERTICAL) {
            // cout << " in VP dragged" << endl;
            // cout << localKeyStore.LMx << endl;
            localKeyStore.Mx = key.Mx;
            // cout << localKeyStore.Mx << endl;
            int offset = localKeyStore.Mx - localKeyStore.LMx;
            // cout << "offset is " << offset << endl;
            localKeyStore.LMx = key.Mx;
            resizeFromDivider(offset, 0);
        }

        if (oriented == HORIZONTAL) {
            // cout << " in VP dragged" << endl;
            // cout << localKeyStore.LMy << endl;
            localKeyStore.My = key.My;
            // cout << localKeyStore.My << endl;
            int offset = localKeyStore.My - localKeyStore.LMy;
            // cout << "offset is " << offset << endl;
            localKeyStore.LMy = key.My;
            resizeFromDivider(0, offset);
        }
    }
}

void UIViewPortController::viewReleased(keyStoreStruct key, int senderID) {

    // draggin release
    // reset divPos
    // cout << "in VP release " << senderID << endl;
    if ((senderID % 2) == 1) {
        // cout << " in VP released" << endl;
        if (oriented == VERTICAL) {
            // cout << viewLeft->getRect().size.width << endl;
            // cout << viewRect.size.width << endl;
            float newPercent = (float)viewLeft->getRect().size.width /
                               (float)viewRect.size.width;
            // cout << "new percent = " << newPercent << endl;
            divPercentLocation = newPercent;
        }

        if (oriented == HORIZONTAL) {
            // cout << viewLeft->getRect().size.width << endl;
            // cout << viewRect.size.width << endl;
            float newPercent = (float)viewLeft->getRect().size.height /
                               (float)viewRect.size.height;
            // cout << "new percent = " << newPercent << endl;
            divPercentLocation = newPercent;
        }
    }
}

void UIViewPortController::resizeFromDivider(int offsetX, int offsetY) {
    // cout << "resize from divider  \n";

    if (oriented == VERTICAL) {
        // cout << " in resizefromdivider. offset is " << offsetX << endl;
        viewLeft->offsetSize(offsetX, 0);
        viewLeft->resolveSize();
        // viewLeft->offsetGlobalPosition(offsetX,
        // globalPosOfParentVPCtlr.y); viewLeft->resolveSize();

        divider->movePosition(offsetX, 0);
        // divider->offsetGlobalPosition(offsetX, 0);

        // dragging down the mouse is negative so we actually increase the
        // right view (as it is on the top!)
        viewRight->offsetSize(-1 * offsetX, 0);
        viewRight->movePosition(offsetX, 0);
        viewRight->resolveSize();
    }

    if (oriented == HORIZONTAL) {
        // cout << " in resizefromdivider. offset is " << offsetY << endl;
        viewLeft->offsetSize(0, -offsetY);
        viewLeft->resolveSize();

        divider->movePosition(0, -offsetY);
        // divider->offsetGlobalPosition(0, -offsetY);

        viewRight->offsetSize(0.f, static_cast<float>(offsetY));
        viewRight->movePosition(0.f, static_cast<float>(-offsetY));
        viewRight->resolveSize();

        // viewRight->offsetGlobalPosition(0.f,
        // static_cast<float>(-offsetY));
    }
}
