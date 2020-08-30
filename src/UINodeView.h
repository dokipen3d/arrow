#pragma once

#include "UIView.h"


class UINodeView : public UIView{

private:

    Node *nodePointedTo;//put this in subclassed UINodeView

public:

    UINodeView(UIView *root, int width, int height): UIView(root, width, height) {}
    ~UINodeView();



    void setNodePointer(Node* nodeToPointTo);//put this in subclassed UINodeView
    int getPointedToNodeID();//put this in subclassed UINodeView



};