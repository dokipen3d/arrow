#ifndef UIVIEWPORT_H
#define UIVIEWPORT_H

#include "UIView.h"
//#include "TextEngine.h"

//for setting the viewport of opengl

class UIViewPort: public UIView{


private:

    bool zoomable;
    bool resizeable;



    frustrumStruct FS;

    GLdouble zoomFactor0;
    GLdouble zoomFactorRatio;
    GLdouble zoomFactor;
    GLdouble zoomInc;
    GLdouble zoomMin;
    GLdouble zoomMax;
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;//mouse values to pass
    GLdouble posX, posY, posZ;//holds final values


public:
    UIViewPort(UIWindow *root, int width, int height):UIView(root, width, height) {}
    ~UIViewPort();
    void setZoomable(bool doIZoom);
    void setResizeable(bool doIResize);
    void Init();
    void setGlobalPosition(float posX, float posY);
    void offsetGlobalPosition(float posX, float posY);

    void offsetWidthHeight(float width, float height);
    void Draw();
    void DrawSelectPass();



};






#endif

