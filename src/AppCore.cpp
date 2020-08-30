/*
 * CAppCore.cpp
 *
 *  Created on: 20 Apr 2010
 *      Author: dme
 */




#include "AppCore.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <cstdlib>
#include "Application.h"


using namespace std;

AppCore::AppCore() {

    treeChanged = true;

    nodeCount = 0;
    currentNodeRenderFocus = 0;
    currentNodeSelectedFocus = 0;



	//GUI = new GLGui;

}

AppCore::~AppCore() {
	// TODO Auto-generated destructor stub
}




void AppCore::callNodeDraw(){//this will go because appcore doesnt recieve events and so wont need to draw
    //cout << "in appcore callnodedraw" << std::endl;
    for (nodeIterator = vNodeStore.begin() ; nodeIterator < vNodeStore.end(); nodeIterator++){
            //cout<< "in iterator" << std::endl;
			(*nodeIterator)->Draw();

		}
}

void AppCore::callNodeDrawSelect(){//this will go because appcore doesnt recieve events and so wont need to draw

	for (nodeIterator = vNodeStore.begin() ; nodeIterator < vNodeStore.end(); nodeIterator++){

			(*nodeIterator)->DrawSelectPass();

		}

}

void AppCore::setViewController(Application *controller){


viewController = controller;


}


Node* AppCore::createNode(){// here give the node an id. if there is a spare one in the id store then use that. if not then give a new one.
    cout << "in appcore CreateNode" << std::endl;
	if (vSpareStack.size() > 0  ){							//if sparestack has something in it....
		int accessElement = vSpareStack.front();				//store the int value that is stored in the last element of sparestack. this will be the index to the vector position to fill up.
		printf("spare index is %d\n",accessElement);						//print the element to see it in console
		this->nodeIterator = vNodeStore.begin();					//set iterator to start of vNodeStore
		Node *tempNode = new Node(accessElement);
		vNodeStore.at(accessElement-1) = tempNode;
		vSpareStack.pop_back();
		nodeCount += 1;
		printf("spare on stack! node count is %d, vector size is %d and spareStack is %d\n",nodeCount,(int)vNodeStore.size(),(int)vSpareStack.size() );
		return tempNode;
	}

	else	{//rethink this for when reading in files with ids that are already set.
	Node *tempNode = new Node(nodeCount+1);
	vNodeStore.push_back(tempNode);
	nodeCount += 1;
	printf("no spares.increasing vector. node count is %d and vector size is %d\n",nodeCount, (int)vNodeStore.size());
	return tempNode;
	}






}


Node* AppCore::createNode( int id){//overloaded for deaing with recreated graphs

	if ((id-1) > vNodeStore.size()){
		Node *tempNode = new Node(id);
		vNodeStore.resize(id, NULL);
		vNodeStore.at(id-1) = tempNode;
		nodeCount += 1;
		return tempNode;

	}
    else return NULL;
}

void AppCore::deleteNode(int id){

    if ((vNodeStore[id-1] != NULL) && (id-1 < (int)vNodeStore.size()) ) { //id less than vector size means it falls within range
                if (id == (int)vNodeStore.size() ){//if node id is at the end
                    printf("deleting last node in vector store\n");
                    delete vNodeStore.at(id-1);
                    vNodeStore.pop_back();
                    nodeCount -= 1;
                    printf("nodeCount is %d\n",(int)(nodeCount-(this->vSpareStack.size()	)	)   );
                    printf("although vector is still size %d and vSpareStack is size %d\n", (int)vNodeStore.size(), (int)this->vSpareStack.size());

                }

                else{
                    //this->nodeIterator = vNodeStore.begin();      //why iterate again?
                    delete vNodeStore[id-1];							//vNodeStore.erase(myIT+id-1);					//delete the element at the index specified by id
                    vSpareStack.push_back(id);						//add the spare slot to sparestack
                    vNodeStore[id-1] = NULL;
                    nodeCount -= 1;
                    printf("nodeCount is %d\n",(int)(nodeCount-(this->vSpareStack.size()	)	)   );
                    printf("although vector is still size %d and vSpareStack is size %d\n", (int)vNodeStore.size(), (int)this->vSpareStack.size());
                }
    }
    else printf("sorry node doesn't exist");
}


void AppCore::connectNodes(int outputNode_id, int inputNode_id, int fromPlugID, int toPlugID ){

    //get a pointer to our node
    Node *tempOUTNode = vNodeStore.at(outputNode_id-1);
    Node *tempInNode = vNodeStore.at(inputNode_id-1);

    //do IN first
    tempInNode->connectPlugs(IN, tempOUTNode, fromPlugID, toPlugID);

    //an now the other way around. bi directional communication.
    tempOUTNode->connectPlugs(OUT, tempInNode, toPlugID, fromPlugID);

    treeChanged = true;
	//printf("about to list connections\n");
	//vNodeStore.at(inputNode_id-1)->listConnections();

}

void AppCore::disconnectNodes(int outputNode_id, int inputNode_id, int fromPlugID, int toPlugID){

	Node *tempOUTNode = vNodeStore.at(outputNode_id-1);
    Node *tempInNode = vNodeStore.at(inputNode_id-1);

	tempOUTNode->disConnectPlugs(OUT,fromPlugID );
	tempInNode->disConnectPlugs(IN,toPlugID);

}

void AppCore::setCurrentFocusNode(int id){

    currentNodeRenderFocus = id;
}

void AppCore::setCurrentSelectedNode(int id){

	printf("selecting node %d\n" , id);
    currentNodeSelectedFocus = id;
}

void AppCore::dirtyCurrentNode (int touch_ID){//will get called in changed attr function. then can render queue without redoing tree as no new connections have been made

	if (currentNodeSelectedFocus != touch_ID || touch_ID == 0){
		printf("new node touched. touching then retracing tree tree\n");//dont need to rebuild as tree hasn't changed shape just attributes.
				vNodeStore.at(touch_ID-1)->dirtyNode();
				//treeChanged = true;
				//nodeToTouch = touch_ID;
				//renderQueue.clear();
				//queueUpCurrentFocusNodeTree();

    }
}



void AppCore::prepareQueue(){

    if (treeChanged == true){
		printf("clearing renderqueue vector\n");
		renderQueue.clear();
    }

	printf("in prepareQueue(), current focus node is %d\n", currentNodeRenderFocus);
	vNodeStore.at(currentNodeSelectedFocus-1)->addToQueue(&renderQueue);
	vNodeStore.at(currentNodeSelectedFocus-1)->recursivelyQueueGraph(&renderQueue);
	treeChanged = false;         //set it to false so that if tree order hasnt changed, then we dont need to rebuild the order of the queue. if connect node is invoked, set treechanged to true


}

void AppCore::renderCurrentQueue(){


	if (renderQueue.size() == 0){
		printf("renderqueue is 0!\n");
		return;
	}

	printf("renderqueue has something to show!\n");
	for (nodeReverseIterator = renderQueue.rbegin() ; nodeReverseIterator < renderQueue.rend(); nodeReverseIterator++){
		//do rendering here. iterate through render queue, render data and pass on to next in queue
		(*nodeReverseIterator)->compute();

	}
	//renderQueue.clear();//all nodes have been rendered so clear the queue. now if one is touched, we only need to render the touched ones and the queue will only rebuild if tree is changed or touch adds them.
}





