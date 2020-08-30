#include "UIView.h"
#include "UIWindow.h"
#include "Application.h"
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstdlib>
#include "Node.h"
#include <math.h>

//each view stores a refernce to the main window it is in

using namespace std;
// TODO here we need to check if parent is null, and if it is, then create a
// uiwindow to embed ourselves in and register the window with the application

// heres what happens when we have a uiview by itself (like the qwidget example .show())
// 1. in user app, we create an application (which has its own window for now, maybe we should disable that?)
// 2. we create a uiview (like a button). it doesn't have a parent passed in so it goes to the else
// 3. in the else, we create a UIWindow which calls its constructor which in turn calls UIView(this) as parent
// 4. this will cause the UIWindow->UIView constructor to go into the if(parent) branch. here we should check if parent is == this!
UIView::UIView(UIView *parent, int width, int height)
    : rootWindow([&]() {
        UIWindow *root = nullptr; 
        if(parent->isRootWindow()){ //this means the UIWindow was created manually so set to self
            root = static_cast<UIWindow*>(parent);
        } else if(parent){  //standard way of parenting views, can just steal the root from the parent
            root = parent->rootWindow; 
        } else {
            root = new UIWindow(width, height);
        }
        return root;
      }()),
      parent(parent) {

  globalIndexID = NULL;

  // initialize to 0 for non VP type UIViews so that world pos is gettable
  // without local pos affecting it.
  globalRect.size.width = 0.0;
  globalRect.size.height = 0.0;
  globalRect.point.x = 0.0;
  globalRect.point.y = 0.0;
  // stopWorldPosSearch = false;

  // if(parent){
  //     rootWindow = parent->rootWindow;
  // }

  viewRect.point.x = 0.0;
  viewRect.point.y = 0.0;
  viewRect.size.width = (float)width;
  // cout << "assigning width as " << viewRect.size.width << std::endl;
  viewRect.size.height = (float)height;
  viewCount = 0;

  // store pointer to parent, get local ID from parent's viewcount and then
  // increment parent's viewCount

  // cout << "in UIView about to get init" << endl;
  // Init();
}

bool UIView::isRootWindow(){
    return parent == this;
}

UIView::~UIView(){
            cout << "in UIView id " << globalIndexID << " destructor" << "\n";
//            //get node parent

//            if (globalIndexID != 0){
//            UIView* parent = getParent();


//            //first remove this UIView from it's parent's children ID store and update the parents list
//            //make sure the child list has something in it.
//            if (parent->UIViewIndexStore.size() > 0){

//                //go through from this view's local id to the end of the child list and update
//                for (int i = localID-1; i < (int)parent->UIViewIndexStore.size(); i++){

//                        //get id of node at the childlist at the specific index we are currently stored at
//                        int viewToDecID = parent->UIViewIndexStore.at(i);

//                        //copy back the id of the node in front
//                        parent->UIViewIndexStore.at(i) = parent->UIViewIndexStore.at(i+1);


//                        UIView *nodeToDecLocID = rootWindow->getNodeFromID(viewToDecID);
//                        nodeToDecLocID->localID--;

//                        //reset the ids actually stored in
//                        }
//                //finally pop back the child list
//                parent->UIViewIndexStore.pop_back();

//            }
//            }
//            cout << "about to iterate, children size is " << UIViewIndexStore.size() << endl;

//            for (viewIndexIterator = UIViewIndexStore.begin() ; viewIndexIterator < UIViewIndexStore.end(); viewIndexIterator++){

//                cout << "deleting child node " << *viewIndexIterator << endl;
//                //delete rootWindow->getNodeFromID((*viewIndexIterator));
//                rootWindow->deRegisterView(*viewIndexIterator);

//            }
}

void UIView::setParentID(int parentID)
{
    parentViewID = parentID;
}


void UIView::resolveSize()
{
    for(auto& index : UIViewIndexStore){
        rootWindow->getNodeFromID(index)->resolveSize();
    }
}

void UIView::addSubView(UIView* newView)
{
    rootWindow->registerView(newView, (UIView*)this);
}


UIView* UIView::getParent(){

    //cout << "getting parent" << endl;
    return rootWindow->getNodeFromID(parentViewID);

}

UIPoint UIView::getWorldPos()
{
    UIPoint tempRect = UIPoint(0.0, 0.0);


    if (globalIndexID != 0)//if not window so we stop there....
    {//call recursivly
         tempRect = getParent()->getWorldPos();
    }

    //return UIPoint(globalRect.point.x + tempRect.x, globalRect.point.y + tempRect.y);
    return UIPoint(std::max(globalRect.point.x, tempRect.x), std::max(globalRect.point.y, tempRect.y));


}


void UIView::Init(){//do extra stuff
    //cout << "view init" << std::endl;
    cout << "GIID " << globalIndexID << "\n";
    cout << "PAID " << parentViewID << "\n";

    //call parent and inc viewCount and set local id
    if (globalIndexID != 0){ //  if this UIview is not the root window
        //UIView* parent = rootWindow->getNodeFromID(parentViewID);
        cout << "parent ID of node " << globalIndexID << " is" << parentViewID << "\n";

        localID = parent->viewCount;
        parent->viewCount++;
        //add to parents child list.
        parent->UIViewIndexStore.push_back(globalIndexID);
        }


    //UIView *tempView = new UIView(rootGUI, this, rootGUI->returnViewCount() );
    //rootWindow->registerView(this);

    drawIDColour[0] = globalIndexID % 256;
    drawIDColour[1] = (globalIndexID>>8) % 256;
    drawIDColour[2] = (globalIndexID>>16) % 256;
    drawable = true;
    childrenDrawable = true;
    setColour(1.0, 1.0, 0.0, 1.0 );
    lmbPressed = false;

}

void UIView::Draw(){

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    //cout << "Drawing node " << globalIndexID << endl;
    //cout << "Children drawable " << childrenDrawable << endl;
    if (drawable == true){

        glColor4f(viewColour[0], viewColour[1], viewColour[2], viewColour[3]);glColor4f(viewColour[0], viewColour[1], viewColour[2], viewColour[3]);
        glBegin(GL_POLYGON);
            //setColour(1.0, 1.0, 0.0, 1.0 );



            glVertex3f(viewRect.point.x, viewRect.point.y, 1.0);
            glVertex3f(viewRect.point.x + viewRect.size.width , viewRect.point.y, 1.0);
            glVertex3f(viewRect.point.x + viewRect.size.width,  viewRect.point.y + viewRect.size.height, 1.0);
            glVertex3f(viewRect.point.x, viewRect.point.y + viewRect.size.height, 1.0);

        glEnd();
        rootWindow->textEngine->render_text("view is the\n best", 0.0, 0.0, 1.0, 1.0);

        }

    if (childrenDrawable == true && viewCount > 0){

        DrawSubViews();
        }


}

void UIView::DrawSubViews(){



    for (viewIndexIterator = UIViewIndexStore.begin() ; viewIndexIterator < UIViewIndexStore.end(); viewIndexIterator++){

            //cout << "in vertex index iterator. fist node id is " << *viewIndexIterator << endl;
			rootWindow->getNodeFromID((*viewIndexIterator))->Draw();
        }


}

void UIView::DrawSelectPass(){

    //cout << "drawing selectPass" << "\n";
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    glColor3ub(drawIDColour[0],drawIDColour[1],drawIDColour[2] );
    glBegin(GL_POLYGON);


        glVertex3f(viewRect.point.x, viewRect.point.y, 0.0);
        glVertex3f(viewRect.point.x + viewRect.size.width , viewRect.point.y, 0.0);
        glVertex3f(viewRect.point.x + viewRect.size.width,  viewRect.point.y + viewRect.size.height, 0.0);
        glVertex3f(viewRect.point.x, viewRect.point.y + viewRect.size.height, 0.0);

    glEnd();

    if (viewCount > 0)
    {
        DrawSelectPassSubViews();
    }

}

void UIView::printID()
{
    //cout << "PRINT: id is " << globalIndexID << "\n";
}

void UIView::deRegisterChildren()
{
    //cout << "in UIView id " << globalIndexID << " deregistering children" << "\n";
    //get node parent

    //window doesnt have parents so doesnt need to rearrange the parents children list
    if (globalIndexID != 0){
    UIView* parent = getParent();


    //first remove this UIView from it's parent's children ID store and update the parents list
    //make sure the child list has something in it.
//    if (parent->UIViewIndexStore.size() > 0){

//        //go through from this view's local id to the end of the child list and update
//        for (int i = localID-1; i < (int)parent->UIViewIndexStore.size(); i++){

//                //get id of node at the childlist at the specific index we are currently stored at
//                int viewToDecID = parent->UIViewIndexStore.at(i);

//                //copy back the id of the node in front
//                parent->UIViewIndexStore.at(i) = parent->UIViewIndexStore.at(i+1);


//                UIView *nodeToDecLocID = rootWindow->getNodeFromID(viewToDecID);
//                nodeToDecLocID->localID--;

//                //reset the ids actually stored in
//                }
//        //finally pop back the child list
//        parent->UIViewIndexStore.pop_back();

//    }
    }
   // cout << "about to iterate, children size is " << UIViewIndexStore.size() << "\n";

    for (viewIndexIterator = UIViewIndexStore.begin() ; viewIndexIterator < UIViewIndexStore.end(); viewIndexIterator++){

        //cout << "deleting child node " << *viewIndexIterator << "\n";
        //delete rootWindow->getNodeFromID((*viewIndexIterator));
        rootWindow->deRegisterView(*viewIndexIterator);

    }

}

void UIView::DrawSelectPassSubViews()
{
    //cout << "viewCOunt is " << viewCount << "\n";
    //cout << "vector size is " << UIViewIndexStore.size() << "\n";
    for (viewIndexIterator = UIViewIndexStore.begin() ; viewIndexIterator < UIViewIndexStore.end(); viewIndexIterator++){

            //cout << "in vertex index iterator of node ID " << globalIndexID <<  " next node id is " << *viewIndexIterator << "\n";
			rootWindow->getNodeFromID(*viewIndexIterator)->DrawSelectPass();
			//cout << "drew pass" << "\n";
    }

}

void UIView::setDrawable(bool amIdrawable){

    drawable = amIdrawable;

}

void UIView::setChildrenDrawable(bool areChildrenDrawable){


    childrenDrawable = areChildrenDrawable;


}

void UIView::setColour (GLfloat R, GLfloat G, GLfloat B, GLfloat A){

    viewColour[0] = R;
    viewColour[1] = G;
    viewColour[2] = B;
    viewColour[3] = A;


}




UIRect UIView::getRect(){


    return viewRect;
}


void UIView::viewClicked(keyStoreStruct key, int senderID){//default just calls parent viewClicked. if parent == NULL then do nothinng.


    //cout << "view clicked called in node " << globalIndexID << "\n";
    //cout << "parentID is " << parentViewID << "\n";
    if (parentViewID < 0){

        //cout << "no parent. must be a window. returning." << "\n";
        return;

    }

    else{

        //calls mainwindow and asks for a pointer to the node which is out parent
        rootWindow->getNodeFromID(parentViewID)->viewClicked(key,localID );

    }


}
void UIView::viewDragged(keyStoreStruct key, int senderID){

    //cout << "view dragged called in node " << globalIndexID << "\n";
    //cout << "parentID is " << parentViewID << "\n";
    if (parentViewID < 0 ){
        //cout << "no parent. must be a window. returning" << "\n";
        return;

    }

    else{
        rootWindow->getNodeFromID(parentViewID)->viewDragged(key, localID);// call root->send id (itself) -> then call myself

    }

}
void UIView::viewReleased(keyStoreStruct key, int senderID){

     if (parentViewID < 0 ){
        //cout << "no parent. must be a window. returning." << "\n";
        return;

    }

    else{
        rootWindow->getNodeFromID(parentViewID)->viewReleased(key, localID);

    }
}






void UIView::movePosition(float moveX, float moveY){

    viewRect.point.x += moveX;
    viewRect.point.y += moveY;

}

void UIView::setPosition(float setX, float setY){

    viewRect.point.x = setX;
    viewRect.point.y = setY;


}

void UIView::setSize(int sizeX, int sizeY){

    viewRect.size.width = sizeX;
    viewRect.size.height = sizeY;
    //cout << "size changed to " << viewRect.size.width << " x " << viewRect.size.height << "\n";
    resolveSize();

}

void UIView::offsetSize(int sizeX, int sizeY){

    viewRect.size.width += sizeX;
    viewRect.size.height += sizeY;
   // cout << "size offset to " << viewRect.size.width << " x " << viewRect.size.height << "\n";

}

