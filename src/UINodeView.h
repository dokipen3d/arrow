#ifndef UINODEUIEW_H
#define UINODEVIEW_H

#include "UIView.h"


class UINodeView : public UIView{

private:

    Node *nodePointedTo;//put this in subclassed UINodeView

public:

    UINodeView(UIWindow *root, int width, int height):UIView(root, width, height) {}
    ~UINodeView();



    void setNodePointer(Node* nodeToPointTo);//put this in subclassed UINodeView
    int getPointedToNodeID();//put this in subclassed UINodeView



};





#endif
