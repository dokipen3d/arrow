#include "UIView.h"
#include "Application.h"
#include "Node.h"
#include "UIWindow.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>

// each view stores a refernce to the main window it is in

using namespace std;
// TODO here we need to check if parent is null, and if it is, then create a
// uiwindow to embed ourselves in and register the window with the application

// heres what happens when we have a uiview by itself (like the qwidget example
// .show())
// 1. in user app, we create an application (which has its own window for now,
// maybe we should disable that?)
// 2. we create a uiview (like a button). it doesn't have a parent passed in so
// it goes to the else
// 3. in the else, we create a UIWindow which calls its constructor which in
// turn calls UIView(this) as parent
// 4. this will cause the UIWindow->UIView constructor to go into the if(parent)
// branch. here we should check if parent is == this!
UIView::UIView(UIView* parentIn, int width, int height, std::string text)
    : parent(parentIn)
    , rootWindow([&]() -> UIWindow* {
        cout << "in uiview lambda " << text << " \n";
        UIWindow* root = nullptr;
        if (parentIn && parentIn->isRootWindow() && isRootWindow()) {
            // this means the UIWindow was created
            // manually (either in the else of an owning parentless view, or a
            // window directly, so set to self
            cout << "in uiview construct of UIWindow! setting root to self\n";
            root = static_cast<UIWindow*>(parent);
        } else if (parentIn) {
            // standard way of parenting views, can just steal
            // the root from the parent
            cout << "in uiview construct of something else! setting root to "
                    "parent root\n";
            root = parentIn->rootWindow;
        } else {
            cout << "no parent. is an orphan, setting up a window!\n";
            // this UIView owns the window
            embeddedWindow =
                std::make_unique<UIWindow>(width, height, text + "_window");
            root = embeddedWindow.get();
        }
        return root;
    }())
    , drawable(true)
    , childrenDrawable(true)
    , lmbPressed(false)
    , viewColour{1.0, 1.0, 0.0, 1.0}
    , viewRect{{0.0, 0.0}, {width, height}}
    , globalRect{{0.0, 0.0}, {0, 0}}
    , registered(false)
    , text(text)
    , expandToFillParent(false)
// initialize to 0 for non VP type UIViews so that world pos is gettable without
// local pos affecting it.

{
    cout << "in uiview constructor " << text << "\n";

    // so that we register to the right one
    if (parent) {
        Application::setActiveWindow(parent->rootWindow);
    }

    if (!isRootWindow()) { // windows register in their own constructor

        Application::registerView(this, isRootWindow());
    }
    // globalID should have been set by now
    cout << "GID: " << globalIndexID << "\n";
    if (parent) {
        cout << "PID: " << parent->id() << "\n";
    }
    drawIDColour[0] = globalIndexID % 256;
    drawIDColour[1] = (globalIndexID >> 8) % 256;
    drawIDColour[2] = (globalIndexID >> 16) % 256;

    // have to embed after registration so can't do it in init lambda
    if (embeddedWindow) {
        embeddedWindow->addSubView(this);
    } else if (!isRootWindow()) { // do it this order because of 0 based index
        localID = parent->viewCount();
        parent->children.push_back(globalIndexID);
    }
}

//UIView::UIView(const UIView& uiview) = default;

std::size_t UIView::id() {
    return globalIndexID;
}

std::size_t UIView::viewCount() {
    return children.size();
}

bool UIView::isRootWindow() {
    return parent == this;
}

void UIView::setId(std::size_t id) {
    globalIndexID = id;
}

void UIView::setLocalId(std::size_t id) {
    localID = id;
}

// starts at position of given local id and bubbles up and decrements local id
void UIView::removeFromChildren(std::size_t localId) {
    if (children.size() > 0) {
        // go through from this view's local id to the end of the child list and
        // update
        for (int i = localId - 1; i < children.size(); i++) {
            // get id of node at the childlist at the specific index we are
            // currently stored at
            int viewToDecID = children.at(i);

            // copy back the id of the node in front
            children.at(i) = parent->children.at(i + 1);

            UIView* nodeToDecrementLocalID =
                rootWindow->getNodeFromID(viewToDecID);
            nodeToDecrementLocalID->localID--;
        }
        // finally pop back the child list
        children.pop_back();
    }
}

void UIView::setExpandable(bool expandable) {
    expandToFillParent = true;
}

UIView::~UIView() {
    // first remove this node from parents list of children
    if (parent) {
        // parent->removeFromChildren(localID);
    }

    // then deregister this node. dont need to deregister children as their
    // destructors will get called?
    rootWindow->deRegisterView(globalIndexID);

    for (auto index : children) {
        rootWindow->deRegisterView(index);
    }
}

void UIView::resolveSize() {

    // TODO need to check if resolving is needed as force refresh and frame
    // buffer change might trigger a resolve
    //cout << "resolving " << text << "(" << id() << ")"
    //     << " from parent " << parent->text << "(" << parent->id() << ") to "
    //     << width() << " " << height()
    //     << "\n";
    if (expandToFillParent) {
        viewRect =
            parent->getRect(); // do we need to get world pos here? surely if
                               // child is a vpcntlr is should know how to embed
    }

    for (auto& index : children) {
        rootWindow->getNodeFromID(index)->resolveSize();
    }
}

void UIView::addSubView(UIView* newView) {
    // do it this order because of 0 based index
    if (!isRootWindow()) {
        newView->setLocalId(viewCount());
        children.push_back(newView->id());
        newView->setParent(this);
    }
}

void UIView::setParent(UIView* newParent) {
    parent = newParent;
}

UIView* UIView::Parent() {
    return parent;
}

UIPoint UIView::getWorldPos() {
    UIPoint tempRect = UIPoint(0.0, 0.0);

    if (!isRootWindow()) // if not window so we stop there....
    {                    // call recursivly
        tempRect = Parent()->getWorldPos();
    }

    //return UIPoint(std::max(globalRect.point.x, tempRect.x),
    //               std::max(globalRect.point.y, tempRect.y));
    /*UIRect parentRect = parent->getRect();
    UIPoint difference = UIPoint(viewRect.point.x - parentRect.point.x,
                               viewRect.point.y - parentRect.point.y);
                                   /*UIRect parentRect = parent->getRect();
                                   * */

    return UIPoint(tempRect.x + viewRect.point.x,
                   tempRect.y + viewRect.point.y);
}

void UIView::Draw() {
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    if (drawable == true) {
        //cout << "drawing " << globalIndexID << " " << text << " "
        //     << viewRect.point.x << " " << viewRect.point.y << " "
        //     << viewRect.size.width << " " << viewRect.size.height
        //     << "\n";
        glColor4f(viewColour[0], viewColour[1], viewColour[2], viewColour[3]);
        glBegin(GL_POLYGON);

        glVertex3f(viewRect.point.x, viewRect.point.y, 1.0);
        glVertex3f(viewRect.point.x + viewRect.size.width, viewRect.point.y,
                   1.0);
        glColor4f(viewColour[0]*0.5, viewColour[1]*0.5, viewColour[2]*0.5, viewColour[3]);
        glVertex3f(viewRect.point.x + viewRect.size.width,
                   viewRect.point.y + viewRect.size.height, 1.0);
        glVertex3f(viewRect.point.x, viewRect.point.y + viewRect.size.height,
                   1.0);

        glEnd();
        // rootWindow->textEngine->render_text("view is the\n best", 0.0,
        // 0.0, 1.0, 1.0);
    }

    if (childrenDrawable == true && viewCount() > 0) {
        DrawSubViews();
    }
}

void UIView::DrawSubViews() {
    // cout << text << " has " << children.size() << " children\n";

    for (auto it = children.begin(); it < children.end(); ++it) {
        rootWindow->getNodeFromID((*it))->Draw();
    }
}

void UIView::DrawSelectPass() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    glColor3ub(drawIDColour[0], drawIDColour[1], drawIDColour[2]);
    glBegin(GL_POLYGON);

    glVertex3f(viewRect.point.x, viewRect.point.y, 0.0);
    glVertex3f(viewRect.point.x + viewRect.size.width, viewRect.point.y, 0.0);
    glVertex3f(viewRect.point.x + viewRect.size.width,
               viewRect.point.y + viewRect.size.height, 0.0);
    glVertex3f(viewRect.point.x, viewRect.point.y + viewRect.size.height, 0.0);

    glEnd();

    if (viewCount() > 0) {

        DrawSelectPassSubViews();
    }
}

void UIView::deRegisterChildren() {
    for (auto it = children.begin(); it < children.end(); ++it) {
        rootWindow->deRegisterView(*it);
    }
}

void UIView::DrawSelectPassSubViews() {
    for (auto it = children.begin(); it < children.end(); ++it) {
        rootWindow->getNodeFromID(*it)->DrawSelectPass();
    }
}

void UIView::setDrawable(bool amIdrawable) {
    drawable = amIdrawable;
}

void UIView::setChildrenDrawable(bool areChildrenDrawable) {
    childrenDrawable = areChildrenDrawable;
}

void UIView::setColour(GLfloat R, GLfloat G, GLfloat B, GLfloat A) {
    viewColour[0] = R;
    viewColour[1] = G;
    viewColour[2] = B;
    viewColour[3] = A;
}

UIRect UIView::getRect() {
    return viewRect;
}

void UIView::viewClicked(keyStoreStruct key, int senderID) {
    if (!isRootWindow()) {
        parent->viewClicked(key, localID);
    }
}

void UIView::viewDragged(keyStoreStruct key, int senderID) {
    if (!isRootWindow()) {
        parent->viewDragged(key, localID);
    }
}

void UIView::viewReleased(keyStoreStruct key, int senderID) {
    if (!isRootWindow()) {
        parent->viewReleased(key, localID);
    }
}

bool UIView::movePosition(float moveX, float moveY) {

    bool allowed = !expandToFillParent;
    if (allowed) {
        viewRect.point.x += moveX;
        viewRect.point.y += moveY;
    }
    return allowed;
}

bool UIView::setPosition(float setX, float setY) {

    bool allowed = !expandToFillParent;
    if (allowed) {
        viewRect.point.x = setX;
        viewRect.point.y = setY;
    }

    return allowed;
}

bool UIView::setSize(int sizeX, int sizeY) {

    bool allowed = !expandToFillParent;
    if (allowed) {
        viewRect.size.width = sizeX;
        viewRect.size.height = sizeY;
    }
    return allowed;
}

bool UIView::offsetSize(int sizeX, int sizeY) {

    bool allowed = !expandToFillParent;
    if (allowed) {
        viewRect.size.width += sizeX;
        viewRect.size.height += sizeY;
    }
    return allowed;
}
