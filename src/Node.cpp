/*
 * node.cpp
 *
 *  Created on: 20 Apr 2010
 *      Author: dme
 */

#include <stdio.h>
#include <iostream>
#include "Node.h"

/*
#ifdef _WIN32
//#define GLEW_STATIC
//#include "C:\Users\dokipen\Documents\Visual Studio 2010\libs\glew-1.9.0\include\GL\glew.h"
#include "C:\Users\dokipen\Documents\Visual Studio 2010\libs\glfw-2.7.7.bin.WIN32\include\GL\glfw.h"
#else
#include "/local1/delliott/glfw-2.7.5/include/GL/glfw.h"
#endif
*/

//GLfloat nodeSize = 25 ;

//Node::Node() {
	// TODO Auto-generated constructor stub
//}

Node::Node(int idToSetInConstructor) {
        printf("constructing!\n");
        this->nodeId = idToSetInConstructor;
        upCount = 0;
        downCount = 0;
        createPlug(GENERIC,IN);
        createPlug(GENERIC,IN);
        createPlug(GENERIC,OUT);
        createPlug(GENERIC,OUT);
		dirtyNode();
		addedToQueue = false;



}

Node::~Node() {//go through all plugs, notify other plugs that the node is dying and that they should set pointers to NULL
        printf("node %d is dying. byeee!",this->nodeId); // delete all image data and renderer nodes in memory




}

void Node::Draw(){

    /*
//cout << "in node draw" << std::endl;

glBegin(GL_POLYGON);
 glColor4f(1.0, 1.0, 0.0, 1.0 );

 glVertex3f((-1 * nodeSize) + posX, nodeSize + posY, 0.0);
 glVertex3f(nodeSize + posX, nodeSize + posY, 0.0);
 glVertex3f(nodeSize + posX, -1 * nodeSize + posY, 0.0);
 glVertex3f((-1 * nodeSize) + posX, -1 * nodeSize + posY, 0.0);

 glEnd();

*/
}

void Node::DrawSelectPass(){


/*
glBegin(GL_POLYGON);

glColor3ub(drawIDColour[0],drawIDColour[1],drawIDColour[2] );


 glVertex3f(-1 * nodeSize + nodePosX, nodeSize + posY, 0.0);
 glVertex3f(nodeSize + nodePosX, nodeSize + posY, 0.0);
 glVertex3f(nodeSize + nodePosX, -1 * nodeSize + posY, 0.0);
 glVertex3f(-1 * nodeSize + nodePosX, -1 * nodeSize + posY, 0.0);

 glEnd();

 */
}




void Node::connectPlugs(plugDirection_t inOrOut, Node* fromNode, int fromIndex, int toIndex){//this will get called by the appcore to connect two nodes.


        // Check here if there is a cyclic connection. Call all node children and if ID is the same as me. then dont connect. maybe do this bit on GUI when hovering over plugs with mouse. for now do it in skeleton.



        printf("connecting plugs\n");
        if (inOrOut == IN){
            plug* tempPlug = fromNode->getPlugPtr(OUT, fromIndex);


                upConnections[toIndex]->connectedPlug = tempPlug;
                upConnections[toIndex]->connected = true;   //for later checking of connected status;
                return;


        }

        else {// remove this as unneccesary if(inOrOut == OUT)
            plug* tempPlug = fromNode->getPlugPtr(IN, fromIndex);


                downConnections[toIndex]->connectedPlug = tempPlug;
                downConnections[toIndex]->connected = true; //for later checking of connected status;
                return;

        }
        printf("couldn't connect\n");
        return;
}

void Node::createPlug(plugType_t TYPE, plugDirection_t inOrOut){//maybe redo this with function template or overloading.

    printf("creating plug\n");
    plug *newPlug = new plug;

    if (inOrOut == IN){
        upConnections[upCount] = newPlug;
        upCount++;
        newPlug->plugDirection = IN;
		newPlug->plugID = upCount;
    }

    else if(inOrOut == OUT){
        downConnections[downCount] = newPlug;
        downCount++;
        newPlug->plugDirection = OUT;
		newPlug->plugID = downCount;
    }

    newPlug->connectedPlug = NULL; //set as default on creation. connect plugs will get called after creation by appCore.
    newPlug->ownerNode = this;
    newPlug->ownerNode = this;
    newPlug->plugType = TYPE;
    newPlug->inputRequired = false;//only required for IN. will only get called on upstream nodes. So set it to false  anyway. on custom nodes we can override this.
	newPlug->connected = false;

}

void Node::disConnectPlugs(plugDirection_t inOrOut, int plugIndex){    //set pointers to Null before the plugs are deleted.

    if (inOrOut == IN){

        upConnections[plugIndex]->connectedPlug = NULL;
        upCount--;
        return;

    }

    else if(inOrOut == OUT){// remove this as unneccesary if(inOrOut == OUT)

        downConnections[downCount]->connectedPlug = NULL;
        downCount--;
        return;

        }

    //connect a new node to a plug
    else return;


}

void Node::dirtyNode(){

    isDirty = DIRTY;

}

bool Node::queryEnabledStatus(){

    return enabled;
}// is the bool enabled true or false?


void Node::enable(){

    enabled = true;
}

void Node::disable(){

    enabled = false;
}

plugType_t Node::queryPlugType(plugDirection_t inOrOut, int plugIndex ){//queriestype of the specified plug.  scoped to Node:: otherwise get compile error because it doesnt know where the enum came from.

	if (inOrOut == IN){return this->upConnections[plugIndex]->plugType;}
	else if(inOrOut == OUT){return this->downConnections[plugIndex]->plugType;}
	else{return GENERIC;}

}//ask this node what type of plug its trying to connect to

plugDirection_t Node::queryPlugDirection(plugDirection_t inOrOut, int plugIndex ){

    if (inOrOut == IN){return this->upConnections[plugIndex]->plugDirection;}
	else {return this->downConnections[plugIndex]->plugDirection;}// took out this beacause this icheck is unneccesary if(inOrOut == OUT)

}

bool Node::isNodeDirty(){

	if (Node::isDirty == DIRTY) return true;
	else return false;
}

void Node::compute(){//this will get overidden in the custom nodes

	printf("in compute %d\n", nodeId);
    isDirty = CLEAN;
}

int Node::queryNodeID(){

	return this->nodeId;
}

void Node::unQueue(){

    addedToQueue = false;
}

bool Node::queuedStatus(){

    return addedToQueue;
}

void Node::recursivelyQueueGraph(vector<Node*> *queue){//argument is a pointer to a vector of pointers. this will get forwarded to addToQueue to get added to the queue and then get forward on to the rest of the graph.


    for (int i=0;i<upCount;i++){                                                        //for all of the upstream connections


        if(upConnections[i]->connected){                                                //if node is connected then..... if its not then the pointer to the right bit of data will be pointing to the default internal value set by the node itself in constructer.

            if(upConnections[i]->connectedPlug->ownerNode->isNodeDirty() && !upConnections[i]->connectedPlug->ownerNode->queuedStatus() ){        //if upstream node is dirty and not in the queue. remember,  all pointers

                upConnections[i]->connectedPlug->ownerNode->addToQueue(queue);
                upConnections[i]->connectedPlug->ownerNode->recursivelyQueueGraph(queue);
            }
        }
    }

}//the main recursive function.

void Node::addToQueue(vector<Node*> *queue){
	printf("adding node to queue\n");
    //DO THIS!
    queue->push_back(this);
    addedToQueue = true;
}//add to the main queue to render. not sure how to do that yet.

Node::plug* Node::getPlugPtr(plugDirection_t inOrOut, int plugIndex){//have to scope to class

    if (inOrOut == IN){
        if (upConnections[plugIndex] != NULL) return upConnections[plugIndex];
    }

    else{
        if (downConnections[plugIndex] != NULL) return downConnections[plugIndex];
    }

    return NULL;

}
