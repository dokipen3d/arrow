#include "UINodeView.h"
#include "Node.h"




void UINodeView::setNodePointer(Node* nodeToPointTo){

              nodePointedTo=(Node*)nodeToPointTo;

}




int UINodeView::getPointedToNodeID(){


    return nodePointedTo->queryNodeID();

}
