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

//each view stores a refernce to the main window it is in

using namespace std;
// TODO here we need to check if parent is null, and if it is, then create a
// uiwindow to embed ourselves in and register the window with the application

// heres what happens when we have a uiview by itself (like the qwidget example .show())
// 1. in user app, we create an application (which has its own window for now, maybe we should disable that?)
// 2. we create a uiview (like a button). it doesn't have a parent passed in so it goes to the else
// 3. in the else, we create a UIWindow which calls its constructor which in turn calls UIView(this) as parent
// 4. this will cause the UIWindow->UIView constructor to go into the if(parent) branch. here we should check if parent is == this!
UIView::UIView(UIView* parentIn, int width, int height, std::string text, bool deferRegistration)
    : parent(parentIn)
    , rootWindow([&]() -> UIWindow* {
        cout << "in uiview lambda " << text << " \n";
        UIWindow* root = nullptr;
        if (parentIn && parentIn->isRootWindow() && isRootWindow()) { // this means the UIWindow was created
            // manually so set to self
                cout << "in uiview construct of UIWindow! setting root to self\n";
                root = static_cast<UIWindow*>(parent);
        } else if (parentIn) { // standard way of parenting views, can just steal
            // the root from the parent
            cout << "in uiview construct of something else! setting root to "
                    "parent root\n";
            root = parentIn->rootWindow;
        } else {
            cout << "no parent. is an orphan, setting up a window!\n";
            // this UIView owns the window
            embeddedWindow = std::make_unique<UIWindow>(width, height, text+"_window");
            // window has to be constructed for self registration to happen as the stakc wouldnt be ready yet.
            //Application::registerView(this, isRootWindow());
            //embeddedWindow->fixParentRelationShip();
            root = embeddedWindow.get();
        }
        return root;
    }())
    , drawable(true)
    , childrenDrawable(true)
    , lmbPressed(false)
    , viewColour{1.0, 1.0, 0.0, 1.0}
    , viewRect { { 0.0, 0.0 }, { width, height } }
    , globalRect { { 0.0, 0.0 }, { 0, 0 } }
    , registered(false),
      text(text)
// initialize to 0 for non VP type UIViews so that world pos is gettable without local pos affecting it.

{

    cout << "in uiview constructor " << text << "\n";

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
    } else if (!isRootWindow()) {  // do it this order because of 0 based index
        localID = parent->viewCount();
        parent->UIViewIndexStore.push_back(globalIndexID);
    }
}

std::size_t  UIView::id() {
    return globalIndexID;
}

std::size_t UIView::viewCount()
{
    return UIViewIndexStore.size();
}

bool UIView::isRootWindow()
{
    return parent == this;
}

void UIView::setId(std::size_t id)
{
    globalIndexID = id;
}

void UIView::setLocalId(std::size_t id)
{
    localID = id;
}

//starts at position of given local id and bubbles up and decrements local id
void UIView::removeFromChildren(std::size_t localId){
    if (UIViewIndexStore.size() > 0){
        //go through from this view's local id to the end of the child list and update
        for (int i = localId-1; i < UIViewIndexStore.size(); i++){
            //get id of node at the childlist at the specific index we are currently stored at
            int viewToDecID = UIViewIndexStore.at(i);

            //copy back the id of the node in front
            UIViewIndexStore.at(i) = parent->UIViewIndexStore.at(i+1);

            UIView *nodeToDecrementLocalID = rootWindow->getNodeFromID(viewToDecID);
            nodeToDecrementLocalID->localID--;
        }
        //finally pop back the child list
        UIViewIndexStore.pop_back();

    }

}


UIView::~UIView()
{
    // first remove this node from parents list of children
    if(parent) {
        //parent->removeFromChildren(localID);
    }

    // then deregister this node. dont need to deregister children as their destructors will get called?
    rootWindow->deRegisterView(globalIndexID);

    for (auto index : UIViewIndexStore ){
        rootWindow->deRegisterView(index);
    }
}


void UIView::resolveSize()
{
    //cout << "resolving " << id() << " " << text << " from parent " << parent->id() << ". view is currently " << viewRect.size.width << " " << viewRect.size.height << " pos "  << globalRect.point.x << " " << globalRect.point.y << "\n";
    for (auto& index : UIViewIndexStore) {
        rootWindow->getNodeFromID(index)->resolveSize();
    }
}

void UIView::addSubView(UIView* newView)
{
    // do it this order because of 0 based index
    if (!isRootWindow()) {
        newView->setLocalId(viewCount());
        UIViewIndexStore.push_back(newView->id());
        newView->setParent(this);
    }
}

void UIView::setParent(UIView* newParent){

    parent = newParent;
}


UIView* UIView::Parent()
{
    return parent;
}

UIPoint UIView::getWorldPos()
{
    UIPoint tempRect = UIPoint(0.0, 0.0);

    if (!isRootWindow()) //if not window so we stop there....
    { //call recursivly
        tempRect = Parent()->getWorldPos();
    }

    return UIPoint(std::max(globalRect.point.x, tempRect.x), std::max(globalRect.point.y, tempRect.y));
}

void UIView::Draw()
{
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    //cout << "Drawing node " << globalIndexID << endl;
    //cout << "Children drawable " << childrenDrawable << endl;
    if (drawable == true) {

        cout << "drawing " << globalIndexID << " " << text << "\n";


        glColor4f(viewColour[0], viewColour[1], viewColour[2], viewColour[3]);
        glColor4f(viewColour[0], viewColour[1], viewColour[2], viewColour[3]);
        glBegin(GL_POLYGON);
        //setColour(1.0, 1.0, 0.0, 1.0 );

        glVertex3f(viewRect.point.x, viewRect.point.y, 1.0);
        glVertex3f(viewRect.point.x + viewRect.size.width, viewRect.point.y, 1.0);
        glVertex3f(viewRect.point.x + viewRect.size.width, viewRect.point.y + viewRect.size.height, 1.0);
        glVertex3f(viewRect.point.x, viewRect.point.y + viewRect.size.height, 1.0);

        glEnd();
        //rootWindow->textEngine->render_text("view is the\n best", 0.0, 0.0, 1.0, 1.0);
    }

    if (childrenDrawable == true && viewCount() > 0) {

        DrawSubViews();
    }
}

void UIView::DrawSubViews()
{

    for (auto it = UIViewIndexStore.begin(); it < UIViewIndexStore.end(); ++it) {

        //cout << "in vertex index iterator. fist node id is " << *it << endl;
        // if(!rootWindow){
        //     cout << "root null!\n";
        // }
        rootWindow->getNodeFromID((*it))->Draw();
    }
}

void UIView::DrawSelectPass()
{
 //   if (drawable == true) {
        cout << "drawing selectPass " << id() << " " << text << "\n";
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);

        glColor3ub(drawIDColour[0], drawIDColour[1], drawIDColour[2]);
        glBegin(GL_POLYGON);

        glVertex3f(viewRect.point.x, viewRect.point.y, 0.0);
        glVertex3f(viewRect.point.x + viewRect.size.width, viewRect.point.y, 0.0);
        glVertex3f(viewRect.point.x + viewRect.size.width, viewRect.point.y + viewRect.size.height, 0.0);
        glVertex3f(viewRect.point.x, viewRect.point.y + viewRect.size.height, 0.0);

        glEnd();

        if (viewCount() > 0) {
            //cout << "sub views!\n ";

            DrawSelectPassSubViews();
        }
 //   }
}

void UIView::printID()
{
    //cout << "PRINT: id is " << globalIndexID << "\n";
}

void UIView::deRegisterChildren()
{
    for (auto it = UIViewIndexStore.begin(); it < UIViewIndexStore.end(); ++it) {
        rootWindow->deRegisterView(*it);
    }
}

void UIView::DrawSelectPassSubViews()
{
    for (auto it = UIViewIndexStore.begin(); it < UIViewIndexStore.end(); ++it) {
        //cout << "in it. getting node from id " << *it << "\n";
        if (!rootWindow) {
            cout << "root is null!\n ";
        }
        if (isRootWindow()) {
            cout << "im a root!\n ";
        }
        rootWindow->getNodeFromID(*it)->DrawSelectPass();
    }
}

void UIView::setDrawable(bool amIdrawable)
{

    drawable = amIdrawable;
}

void UIView::setChildrenDrawable(bool areChildrenDrawable)
{

    childrenDrawable = areChildrenDrawable;
}

void UIView::setColour(GLfloat R, GLfloat G, GLfloat B, GLfloat A)
{

    viewColour[0] = R;
    viewColour[1] = G;
    viewColour[2] = B;
    viewColour[3] = A;
}

UIRect UIView::getRect()
{

    return viewRect;
}

void UIView::viewClicked(keyStoreStruct key, int senderID)
{
    if (!isRootWindow()) {
        parent->viewClicked(key, localID);
    }
}

void UIView::viewDragged(keyStoreStruct key, int senderID)
{
    if (!isRootWindow()) {
        parent->viewDragged(key, localID);
    }
}

void UIView::viewReleased(keyStoreStruct key, int senderID)
{
    if (!isRootWindow()) {
        parent->viewReleased(key, localID);
    }
}

void UIView::movePosition(float moveX, float moveY)
{

    viewRect.point.x += moveX;
    viewRect.point.y += moveY;
}

void UIView::setPosition(float setX, float setY)
{

    viewRect.point.x = setX;
    viewRect.point.y = setY;
}

void UIView::setSize(int sizeX, int sizeY)
{

    viewRect.size.width = sizeX;
    viewRect.size.height = sizeY;
    //cout << "size changed to " << viewRect.size.width << " x " << viewRect.size.height << "\n";
}

void UIView::offsetSize(int sizeX, int sizeY)
{

    viewRect.size.width += sizeX;
    viewRect.size.height += sizeY;
    // cout << "size offset to " << viewRect.size.width << " x " << viewRect.size.height << "\n";
}
