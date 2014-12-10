#include "UIViewPort.h"
#include <iostream>




void UIViewPort::setZoomable(bool doIZoom){



    zoomable = doIZoom;


}


void UIViewPort::setResizeable(bool doIResize){


    resizeable = doIResize;


}

void UIViewPort::setGlobalPosition(float posX, float posY)
{
    globalRect.point.x = posX;
    globalRect.point.y = posY;
    cout << "global rect offset is " << globalRect.point.x << endl;
}

void UIViewPort::offsetGlobalPosition(float posX, float posY)
{
    globalRect.point.x += posX;
    globalRect.point.y += posY;
    cout << "global rect moved is " << globalRect.point.x << endl;

}




void UIViewPort::offsetWidthHeight(float Width, float Height)
{
    globalRect.size.width += Width;
    globalRect.size.height += Height;
}

void UIViewPort::Draw()
{

    glViewport(globalRect.point.x, globalRect.point.y,viewRect.size.width,viewRect.size.height);
    //cout << "set viewportx to " << globalRect.point.x << "with size " << viewRect.size.width << endl;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, viewRect.size.width,0.0 , viewRect.size.height, 1000, -1000);
    //cout << "set glOrtho sizeX as" << viewRect.size.width << endl;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    UIView::Draw();
}

void UIViewPort::DrawSelectPass()
{
    cout << "in VP DrawSelectPass" << endl;
    glViewport(globalRect.point.x, globalRect.point.y,viewRect.size.width,viewRect.size.height);
    //cout << "set viewportx to " << globalRect.point.x << "with size " << viewRect.size.width << endl;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, viewRect.size.width,0.0 , viewRect.size.height, 100, -100);
    //cout << "set glOrtho sizeX as" << viewRect.size.width << endl;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    UIView::DrawSelectPass();

}

void UIViewPort::Init(){


    UIView::Init();




    zoomFactor0 = 1.0;
    zoomFactorRatio = 1.0;
    zoomFactor = 1.0;
    zoomInc = 0.03;
    zoomMin = 0.2;
    zoomMax = 3.0;
    posX = 0.0;
    posY = 0.0;
    posZ = 0.0;//holds final values



}


UIViewPort::~UIViewPort(){



}
