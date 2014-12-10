#include "UIViewPortController.h"
#include "UIWindow.h"
#include "UIViewPort.h"
#include <iostream>

//class for controlling divisions of the screen. we can add further subdivisions to viewports through this class.
//we never actually get down to accessing viewports themselves. we add to them through the controller.


using namespace std;

UIViewPortController::~UIViewPortController(){

}

void UIViewPortController::Init(){

    UIView::Init();

    //set up main viewport. there is one by default before dividing.
    viewLeft = new UIViewPort(rootWindow, 0, 0);
    rootWindow->registerView(viewLeft, this);
    divider = NULL;
    viewRight = NULL;//set this to null and only create if we divide.
    DIVIDED = false;
    divPosition = 0;

    //set up some values
    setDrawable(false);
    dragging = false;
    //divPercentLocation = 0.1;
    //divPosition = viewRect.size.width * (divPercentLocation);
    //offsetSize = divPosition+dividerWidth;

    if (oriented == HORIZONTAL){
    }

    if (oriented == VERTICAL)
    {
        //divide is here for testing purposes. normally called from outside of class.
        //divide();

    }
    cout << "in VPCtlr id " << globalIndexID << "about to resolveSize" << endl;
    resolveSize();


}

void UIViewPortController::divide(float divPC){

    DIVIDED = true;

    cout << "dividing" << endl;

    if (oriented == VERTICAL){
        divPercentLocation = divPC;
        //create the views for adding into the VPCtrlr
        if (viewRight == NULL)
        {
            viewRight = new UIViewPort(rootWindow, 0, 0);
            rootWindow->registerView(viewRight, this);
        }

        if (divider == NULL)
        {
            divider = new UIViewPort(rootWindow, dividerWidth, viewRect.size.height);
            rootWindow->registerView(divider, this);
        }
        resolveSize();
    }

    if (oriented == HORIZONTAL)
    {
        divPercentLocation = 1.0-divPC;//reverse because of opengl y coordinate
        //create the views for adding into the VPCtrlr
        if (viewRight == NULL)
        {
            viewRight = new UIViewPort(rootWindow, 0, 0);//create it with no size, it wil get resolved later on...
            rootWindow->registerView(viewRight, this);
        }

        if (divider == NULL)
        {
            divider = new UIViewPort(rootWindow,viewRect.size.width , dividerWidth);
            rootWindow->registerView(divider, this);
        }
        resolveSize();
    }
}

void UIViewPortController::resolveSize()
{
    //set self to be parent viewports viewRect because it might now have been set before
    viewRect = getParent()->getRect();

    if (DIVIDED)
    {
        cout << "divPos is " << divPosition << endl;

    }

    cout << "resolvedSize is " << viewRect.size.width << endl;
    cout << "resolvePos is " << viewRect.point.x << endl;
    //call to resize children
    resizeGlobalSubViewPorts();

}


void UIViewPortController::resizeGlobalSubViewPorts()//for setting up sizes after division. another function takes cares of local resizing from mouse (but this might get called as a consequence of that other simpler function
{
    //grab global pos of VPCtlr
    UIPoint globalPosOfParentVPCtlr = getWorldPos();
    cout << "global Pos of parent VPCTlr is " << globalPosOfParentVPCtlr.x << endl;

    if (oriented == VERTICAL)
    {
        divPosition = viewRect.size.width * (divPercentLocation);//floating point errors causing extra pixels?
        offsetSize = divPosition+dividerWidth;
        //iterate through sub views and resize. hopefully works for multiple divs but limit to 3 for now


        if (UIViewIndexStore.size() == 1)//if its only one viewport....
        {//set it's size to be the VPCtlr viewRect...remember keep VPCtlrs pos 0.0, 0.0....
            viewLeft->setPosition(0.0, 0.0);
            viewLeft->setSize(viewRect.size.width, viewRect.size.height);
            viewLeft->setGlobalPosition(globalPosOfParentVPCtlr.x, globalPosOfParentVPCtlr.y);
            viewLeft->resolveSize();

        }

        else if (UIViewIndexStore.size() == 3)
        {
            viewLeft->setColour(0.5, 0.0, 0.5, 1.0);
            viewLeft->setPosition(0.0, 0.0);
            viewLeft->setSize(divPosition, viewRect.size.height);
            viewLeft->setGlobalPosition(globalPosOfParentVPCtlr.x, globalPosOfParentVPCtlr.y);
            viewLeft->resolveSize();

            divider->setColour(0.0, 0.5, 0.5, 1.0);
            divider->setPosition(0.0, 0.0);
            divider->setSize(dividerWidth, viewRect.size.height);
            divider->setGlobalPosition(globalPosOfParentVPCtlr.x+divPosition, globalPosOfParentVPCtlr.y);

            viewRight->setColour(0.5, 0.5, 0.0, 1.0);
            viewRight->setPosition(0.0, 0.0);
            viewRight->setSize(viewRect.size.width-offsetSize,viewRect.size.height);
            viewRight->setGlobalPosition(globalPosOfParentVPCtlr.x+offsetSize, globalPosOfParentVPCtlr.y);
            viewRight->resolveSize();
        }
    }

    else if (oriented == HORIZONTAL)
    {
        divPosition = viewRect.size.height * (divPercentLocation);//floating point errors causing extra pixels?
        offsetSize = divPosition+dividerWidth;
        //iterate through sub views and resize. hopefully works for multiple divs but limit to 3 for now


        if (UIViewIndexStore.size() == 1)//if its only one viewport....
        {//set it's size to be the VPCtlr viewRect...remember keep VPCtlrs pos 0.0, 0.0....
            viewLeft->setPosition(0.0, 0.0);
            viewLeft->setSize(viewRect.size.width, viewRect.size.height);
            viewLeft->setGlobalPosition(globalPosOfParentVPCtlr.x, globalPosOfParentVPCtlr.y);
            viewLeft->resolveSize();

        }

        else if (UIViewIndexStore.size() == 3)
        {
            viewLeft->setColour(0.5, 0.0, 0.5, 1.0);
            viewLeft->setPosition(0.0, 0.0);
            viewLeft->setSize(viewRect.size.width , divPosition);
            viewLeft->setGlobalPosition(globalPosOfParentVPCtlr.x, globalPosOfParentVPCtlr.y);
            viewLeft->resolveSize();

            divider->setColour(0.0, 0.5, 0.5, 1.0);
            divider->setPosition(0.0, 0.0);
            divider->setSize(viewRect.size.width, dividerWidth);
            divider->setGlobalPosition(globalPosOfParentVPCtlr.x, globalPosOfParentVPCtlr.y+divPosition);

            viewRight->setColour(0.5, 0.5, 0.0, 1.0);
            viewRight->setPosition(0.0, 0.0);
            viewRight->setSize(viewRect.size.width, viewRect.size.height-offsetSize);
            viewRight->setGlobalPosition(globalPosOfParentVPCtlr.x, globalPosOfParentVPCtlr.y+offsetSize);
            viewRight->resolveSize();
        }


    }

    UIView::resolveSize();
    //stopWorldPosSearch = true;//so sub VPts can stop searching.
}




void UIViewPortController::setOrientation(orientation orient){

    oriented = orient;

}

void UIViewPortController::addChildView(UIView *newView, int index){

    //add it to the viewport that was created at div 1-2
    if (index == 0)
    {
        viewLeft->addSubView(newView);
        resizeGlobalSubViewPorts();
    }

    if (index == 1)
    {
        viewRight->addSubView(newView);
        resizeGlobalSubViewPorts();
    }
}

void UIViewPortController::addSubViewPortController(UIViewPortController *newView, int index){

    //add it to the viewport that was created at div 1-2
    //set it to the size of that viewport if its another vpcntlr
    if (index == 0)
    {
        //format incoming viewportController size so that the viewRect matches the viewport its going into.
        viewLeft->addSubView(newView);
        viewLeft->resolveSize();
    }

    if (index == 1)
    {
        viewRight->addSubView(newView);
        viewRight->resolveSize();

    }

}



void UIViewPortController::viewClicked(keyStoreStruct key, int senderID){

    //mouse click for dragging
    //if sender is less than my viewcount (ie is the caller one of my children)
    //if odd then its a divider. if even its a sub view that doesnt reaarange.
    //then store mouse pos and set mousebutton pressed.
    cout << "viewport contrler recieved click" << endl;
    if ((senderID % 2) == 1 ) //called from divider, use mod incase we want to implement multiple divides
    {

            localKeyStore.LMx = key.Mx;
            localKeyStore.LMy = key.My;
            cout << localKeyStore.LMx << endl;
            cout << localKeyStore.LMy << endl;

    }


}

void UIViewPortController::viewDragged(keyStoreStruct key, int senderID){

    //mouse drag for dragging
    //if lmb pressed == true
    //work out offset from stored mouse pos
    //send offset to resize the view-1 and reposition and resize view+1

    //float mouseOffSetX = localKeyStore.LMx - keyStoreStruct.Mx;
    //float mouseOffSetY = localKeyStore.LMy - keyStoreStruct.My;

    //resizeFromDivider(int offsetX, int offsetY, int localID);
    cout << "senderID was " << senderID << endl;



    if ((senderID % 2) == 1 )
    {
        if (oriented == VERTICAL)
        {
            cout << " in VP dragged" << endl;
            cout << localKeyStore.LMx << endl;
            localKeyStore.Mx = key.Mx;
            cout << localKeyStore.Mx << endl;
            int offset = localKeyStore.Mx - localKeyStore.LMx;
            cout << "offset is " << offset << endl;
            localKeyStore.LMx = key.Mx;
            resizeFromDivider(offset, 0 );
        }

        if (oriented == HORIZONTAL)
        {
            cout << " in VP dragged" << endl;
            cout << localKeyStore.LMy << endl;
            localKeyStore.My = key.My;
            cout << localKeyStore.My << endl;
            int offset = localKeyStore.My - localKeyStore.LMy;
            cout << "offset is " << offset << endl;
            localKeyStore.LMy = key.My;
            resizeFromDivider(0, offset );
        }


    }


}


void UIViewPortController::viewReleased(keyStoreStruct key, int senderID){


    //draggin release
    //reset divPos
    cout << "in VP release " << senderID << endl;
    if ((senderID % 2) == 1 )
    {
        cout << " in VP released" << endl;
        if (oriented == VERTICAL)
        {
            //cout << viewLeft->getRect().size.width << endl;
            //cout << viewRect.size.width << endl;
            float newPercent = (float)viewLeft->getRect().size.width / (float)viewRect.size.width;
            cout << "new percent = " << newPercent << endl;
            divPercentLocation = newPercent;
        }

        if (oriented == HORIZONTAL)
        {
            //cout << viewLeft->getRect().size.width << endl;
            //cout << viewRect.size.width << endl;
            float newPercent = (float)viewLeft->getRect().size.height / (float)viewRect.size.height;
            cout << "new percent = " << newPercent << endl;
            divPercentLocation = newPercent;
        }
    }

}

void UIViewPortController::resizeFromDivider(int offsetX, int offsetY)
{

            if (oriented == VERTICAL)
            {
                cout << " in resizefromdivider. offset is " << offsetX << endl;
                viewLeft->offsetSize(offsetX, 0);

                //viewLeft->offsetGlobalPosition(offsetX, globalPosOfParentVPCtlr.y);
                viewLeft->resolveSize();

                divider->offsetGlobalPosition(offsetX, 0);

                viewRight->offsetSize(-1*offsetX,0);
                cout << " got here." << endl;

                viewRight->offsetGlobalPosition(offsetX, 0);
                viewRight->resolveSize();
            }

            if (oriented == HORIZONTAL)
            {
                cout << " in resizefromdivider. offset is " << offsetY << endl;
                viewLeft->offsetSize(0, -offsetY);
                //viewRight->offsetSize(0,-1*offsetY);

                //viewLeft->offsetGlobalPosition(offsetX, globalPosOfParentVPCtlr.y);
                viewLeft->resolveSize();

                divider->offsetGlobalPosition(0, -offsetY);

                //cout << " got here." << endl;
                viewRight->offsetSize(0, offsetY);
                viewRight->offsetGlobalPosition(0, -offsetY);
                viewRight->resolveSize();

            }




}

