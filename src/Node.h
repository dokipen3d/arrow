/*
 * node.h
 *
 *  Created on: 20 Apr 2010
 *      Author: dme
 *
 *      this is the class that the nodes of the tree are made up of. the nodes themselves don't contain any image data.
 *      the nodes will have an imageData class inside them that will contain the imagedata. the imageData class will
 *      have functions for setting and getting data.
 *
 *      the nodes also will have renderer classes that do the actuall math on the pixels. this way, nodes can be of a different type
 *      such as over add etc without having to change anything within the node.Also the imageData can change between bit depths and
 *      pixel formats without having to change renderer. the renderer handles that all independently. it will have to be told what
 *      it is being given though.
 *
 *
 *
 *      updated in feb 2013. base class for all nodes. once basic tree functionality is reimplemented with plugs, nodes can be abstracted to create different types.
 *      need to work out how to make type enum global so it only needs to be written in the derived class and not ahead. Key difference is that the plugs are introduced to allow only the right kind of plugs
 *      to connect to each other to force correct data flow. IMPORTANT CONCEPT IS THAT PLUGS NOTIFY OTHER PLUGS TO UPDATE THEIR OWNER. NO NODES SHOULD EVER TALK TO ANOTHER NODE DIRECTLY.
 *
 */

#ifndef NODE_H_
#define NODE_H_

#include <stdio.h>
#include <vector>
#include "PlugTypes.h"

/*
#ifdef _WIN32
//#define GLEW_STATIC
//#include "C:\Users\dokipen\Documents\Visual Studio 2010\libs\glew-1.9.0\include\GL\glew.h"
#include "C:\Users\dokipen\Documents\Visual Studio 2010\libs\glfw-2.7.7.bin.WIN32\include\GL\glfw.h"
#else
#include "/local1/delliott/glfw-2.7.5/include/GL/glfw.h"
#endif
*/

using namespace std;



class Node {

private:

    enum dirtyStatus {DIRTY, CLEAN};
	dirtyStatus isDirty;
	bool addedToQueue;//for tracking if the node is added to the renderQueue or not

    bool enabled;                   //is it turned on? if not then bypass data (forward pointers and dont compute)



	struct plug //internal structure in node to keep track of what nodes are connected. allows up to create empty plugs. also controls correct type and data flow.
        {
        int plugID;                     //unique interger for placement in array
		bool connected;                 //is there a pointer assigned to 'plug.connectedPlug'
		plug *connectedPlug;            //which plug is this pugged into. we can call the connected plugs owner
		Node *ownerNode;                //which node is this that I am owned by? can call functions from other plugs then (such as compute and pther getters).
		plugType_t plugType;            //test plug TYPE to avoid wrong data flow
		plugDirection_t plugDirection;  //test in or out.
		bool inputRequired;             //is the input required or can a default value work? Numericfloats and scalars might be okay. on a node basis. set in constructors.Probably only for upstream ones.
		void *plugData;                 //pointer to data. could be pointer to another object such as a volume object which contains more data about it's size. Or could just be pointer to a float.
        };

    struct guiObject {

            guiEntity_t objectType; //so gui can create the right type of gui object in node/view creation
            char *objectName;       //name of field. doesnt have to be same as variable it is changing just the
            //function pointer to allow gui nodes to set values within nodes using this setter function given to it.

    };

	plug* upConnections[16]; //Array of pointers to plug for up to 16 plug. can loop through these and perform upstream operations
	plug* downConnections[16];


public:

    //Constructors and Destructors
	//Node();								//set up stuff. set mainappCore to the reference of the mainapp that was pass.
	Node(int idToSetInConstructor);
	virtual ~Node();

    int nodeId;//unique identifier for each node

	//Main functions related to node operation
	void connectPlugs(plugDirection_t inOrOut, Node* fromNode, int fromIndex, int toIndex);    //connect a plug to a plug. main method of connection. gets called on both nodes by the app and sets both plugs to point to each other to allow bidrection communication if needed.
	void disConnectPlugs(plugDirection_t inOrOut, int plugIndex);                       //set pointers to Null before the plugs are deleted.
	void createPlug(plugType_t TYPE, plugDirection_t inOrOut);                          //allows a node to make a new plug on instantiation, specify type and stream direction. gets added to up/downConnections array
    plug* getPlugPtr(plugDirection_t inOrOut, int plugIndex);                           //get a pointer to a plug given a direction and an index from the GUI.

    void compute();//do what the node needs to do in here. it will check itself if it has all the right data coming in its plugs.
    //QueueFunctions
    void recursivelyQueueGraph(vector<Node*> *queue);//the main recursive function.
    void addToQueue(vector<Node*> *queue);//add to the main queue to render. not sure how to do that yet.
    void unQueue();//unqueue. set bool to false so detection is correct when dirtied.
    bool queuedStatus();//query if queued
	void Draw();// draw gui code
	void DrawSelectPass();//Draw node with id colors into back buffer

	//Setters and Getters
	plugType_t queryPlugType(plugDirection_t inOrOut, int plugIndex );//ask this node what type of plug its trying to connect to will
	plugDirection_t queryPlugDirection(plugDirection_t inOrOut, int plugIndex );
	int queryNodeID();//return nodeID
	bool isNodeDirty();//return dirty status to know if it needs recomputing.
	void dirtyNode();
	bool queryEnabledStatus();// is the bool enabled true or false?
	void enable();
	void disable();


	int upCount;//how many node connections there are. just saves having to do vectors for now as we prob wont need more than 16 connections.
	int downCount;
	float posX;
	float posY;




};

#endif
	/*



	int requestID();					//return the id of this CNode
private:
	CNode* pUpperCNode_A;					//Stores a pointer to the CNodes up the tree.
	CNode* pUpperCNode_B;
	bool isRendered;    				//TRUE if data is rendered and doesn't need to be updated. FALSE if parameters have been tweaked and it needs rendering. MAYBE HAVE NEXT ONE DOWN?
	void* mainAppCore;
    */

/* CNODE_H_ */
