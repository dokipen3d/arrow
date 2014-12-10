/*
 * appCore.h
 *
 *  Created on: 20 Apr 2010
 *      Author: dme
 */

#ifndef CAPPCORE_H_
#define CAPPCORE_H_
#include <vector>
#include "Node.h"
#include "NodeTypes.h"
//#include "UIViewController.h"

using namespace std;

class UIViewController; //forward declaration to make sure that we are aware of appcore. since appcore contiains the glgui header, the header wont get included.



class AppCore {
public:
	AppCore();
	virtual ~AppCore();

    Node * createNode();				//create a node object and put it in the array.
    Node * createNode(int id);				//for when loading files we recreate ids. need to find way of dealing with clashing ids.

    void deleteNode(int id);		//delete a node from the vector using its id.disconnect all plugs first
    void connectNodes(int outputNode_id, int inputNode_id, int fromPlugID, int toPlugID ); //connect two nodes specifying which nodes and which inputs....
	void disconnectNodes(int outputNode_id, int inputNode_id, int fromPlugID, int toPlugID);
    void queueUpCurrentFocusNodeTree();	//queue up the nodes that are connected up the tree to get ready to render.make the tree recursively check if nodes need rendering and add themselves to the render queue
    void setCurrentFocusNode(int id);//set which node we want to render from. (similar to highlighting it)
    void setCurrentSelectedNode(int id);//set which node we want to render from. (similar to highlighting it)
    //void appLoop();                    //enter main loop. might swap this out for a app loop object
    void prepareQueue();              //call nodes to recursively queue up the queue ready for computing.
    void callNodeDraw();//call all the nodes to draw themselves.
	void callNodeDrawSelect();//draw all nodes with id colors to backbuffer
    void setViewController(UIViewController *controller); // can dynamically change viewController so we can have different behaviour at runtime


private:
    vector<Node*> vNodeStore;		//vector for storing node pointers in scene
	vector<Node*>::iterator nodeIterator;
	vector<Node*>::reverse_iterator nodeReverseIterator;
	vector<Node*> nodeQueue;//for rendering in order
    vector<int> vSpareStack;		//for storing any spaces in the vNodestore that have been deleted.
    vector<Node*> renderQueue;				//A vector for adding all the node id's that need rendering in the right order.
	int currentNodeRenderFocus;		//The current node to render
	int currentNodeSelectedFocus;		//The current node that is selected
    int nodeCount;					//a counter for storing how many nodes are in the array
    void dirtyCurrentNode(int touch_ID);   //force node to become dirty
    void renderCurrentQueue();             //render currently line up queue
	bool treeChanged;				//to keep track of wether the tree has been rearranged with nodes being disconnected and if we need to rebuild the queue.

	//GLGui *GUI;
	UIViewController *viewController;







};

#endif /* CAPPCORE_H_ */


/*
public:
	AppCore();
	virtual ~CppCore();



	void readSceneFile();			//read a txt scene file to populate tree and source data
	void writeSceneFile();			//write exisiting tree to a txt scene file

	void listRenderQueue();			//list the render queue order

private:

	vector<Node*> vNodeEnds;		//array for storing the nodes that are at the end of the tree and that will start the render


	int currentNodeAttributeFocus;	//The current node to display attributes for.
    int nodeToTouch;

*/
