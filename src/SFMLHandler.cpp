
#include "GLGui.h"
#include <string.h>

//#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "UIViewController.h"
//#include "UIView.h"
#include "Node.h"




//class AppCore;

SFMLHandler::keyStoreStruct SFMLHandler::keyStore;
int globalViewCount;


SFMLHandler::SFMLHandler(){


}

SFMLHandler::~SFMLHandler(){


}


void SFMLHandler::GLLoop(){

    cout << "in GLLoop" << std::endl;
  while(1)
    {
    processEvents();
	DrawGui();
	glfwSwapBuffers();
    glfwWaitEvents();
	//glfwPollEvents();
    }
}



void SFMLHandler::InitGL(const char *name){

    FS.centerX = 0;
    FS.centerY = 0;
    //width = window_width;
    //height = window_height;
    window_width = 1024;
    window_height = 600;
    aspect_ratio = ((float)window_width/(float)window_height);
    doSelection = false;
    escapePressed = false;
    altPressed = false;
    lmbPressed = false;
    cout << "lmbPressed is " << std::boolalpha << lmbPressed <<std::endl;
    mmbPressed = false;
    rmbPressed = false;

    zooming = false;
    panning = false;

    dx =0;
    dy = 0;

    zoomFactor0 = 1.0;
    zoomFactorRatio = 1.0;
    zoomFactor = 1.0;
    zoomInc = 0.03;
    zoomMin = 0.2;
    zoomMax = 3.0;
    posX = 0.0;
    posY = 0.0;
    posZ = 0.0;//holds final values


    cout << "aspect ratio is " << aspect_ratio << "\n";
    guiName = name;
    globalViewCount = 0;
    //mainCore = mainAppCore;
    viewChanged = true;//do this here to have an intial draw on first go.


    if (glfwInit() != GL_TRUE)  {
        CloseGUI(1);            }
    // 800 x 600, 16 bit color, no depth, alpha or stencil buffers, windowed
    //glfwOpenWindowHint( GLFW_OPENGL_VERSION_MAJOR, 3 );
    //glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
    //glfwOpenWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE );
    //glfwOpenWindowHint ( GLFW_REFRESH_RATE, 60);
    glfwSwapInterval(1);
    if (glfwOpenWindow(window_width, window_height, 5, 6, 5,
                     0, 0, 0, GLFW_WINDOW) != GL_TRUE)  {
    CloseGUI(1);                                        }
    glfwSetWindowTitle(guiName);
    glfwSetKeyCallback(keyCallback);
	glfwSetMouseButtonCallback(MouseButtonCallback);
	glfwSetMousePosCallback(MousePosCallback);
	glfwSetWindowSizeCallback(GLGuiResizeWindow);
    glViewport(0, 0, window_width,window_height);
    // set the projection matrix to a normal frustum with a max depth of 50

    //glOrtho((minusInitialWindowCoordWidth * zoomOut) * aspect_ratio , (initialWindowCoordWidth * zoomOut) * aspect_ratio, (minusInitialWindowCoordWidth * zoomOut), (initialWindowCoordWidth * zoomOut), 100, -100);// * aspect_ratio
    OLD.left = 0-(window_width/2.0);
    OLD.right = 0+(window_width/2.0);
    OLD.bottom = 0+(window_height/2.0);
    OLD.top =  0-(window_height/2.0);
    FS.left = 0-(window_width/2.0);
    FS.right = 0+(window_width/2.0);
    FS.bottom = 0+(window_height/2.0);
    FS.top =  0-(window_height/2.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(OLD.left, OLD.right,OLD.bottom,OLD.top, 100, -100);
    //glFrustum(.5, -.5, -.5 * aspect_ratio, .5 * aspect_ratio, 1, 50);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.5,0.5,0.5,1.0);



    cout << "GLGui created"<< std::endl;


    creationY = 50;//node GUI starting pos
    creationX = 0;

    createNode();
    createNode();
    createNode();
    createNode();
	createNode();
    connectNodes(1, 3, 0, 0 );
    connectNodes(2, 3, 0, 1 );
	connectNodes(3, 4, 0, 0 );
	connectNodes(1, 4, 0, 1 );
	connectNodes(3, 5, 0, 0 );



         }



void SFMLHandler::processEvents(){//called in main loop and checkes key presses to maniuplate drawing of ui and perform functions. IE. interface!
    //cout << "in control logic" << std::endl;
	//cout << Key.keyStore <<std::endl;
	//cout <<"Mouse moved to x:" << Mouse.Mx << "y:" << Mouse.My << std::endl;

	//MODIFIER KEYS
	switch (keyStore.key){

        case GLFW_KEY_LALT://
            switch (keyStore.keyActionStore)
            {
                case GLFW_PRESS:                                                    //if ALT KET PRESSED
                    switch (keyStore.mouseActionStore){
                        case GLFW_PRESS:// ALT && LMB
                            switch (keyStore.buttonStore){
                                case GLFW_MOUSE_BUTTON_LEFT:
                                glfwSetMousePosCallback(MousePosCallback);//ALT && LMB PRESS

                                //rotate
                                break;

                                case  GLFW_MOUSE_BUTTON_RIGHT:                      //ALT && RMB PRESS
                                glfwSetMousePosCallback(MousePosCallback);
                                ZoomOrtho();
                                //zoom
                                break;

                                case  GLFW_MOUSE_BUTTON_MIDDLE:                     //ALT && MMB PRESS
                                glfwSetMousePosCallback(MousePosCallback);
                                pan();
                                break;
                                }
                        break;

                        case GLFW_RELEASE: //ALT && RMB
                            switch (keyStore.buttonStore){
                                case GLFW_MOUSE_BUTTON_LEFT:                         //ALT && LMB RELEASE
                                glfwSetMousePosCallback(NULL);

                                break;

                                case  GLFW_MOUSE_BUTTON_RIGHT:                      //ALT && RMB RELEASE
                                glfwSetMousePosCallback(NULL);
                                keyStore.buttonStore = NULL;
                                zoomRelease();
                                break;

                                case  GLFW_MOUSE_BUTTON_MIDDLE:                     //ALT && MMB RELEASE
                                glfwSetMousePosCallback(NULL);
                                keyStore.buttonStore = NULL;
                                panRelease();
                                break;


                                }
						break;
                    }

				case GLFW_RELEASE:
						break;
					//enter normal state

				break;

			//other keys here.
            }
        }
    //Mouse buttons on their own......
	switch (keyStore.mouseActionStore){
                        case GLFW_PRESS:// ALT && LMB
                            switch (keyStore.buttonStore){
                                case GLFW_MOUSE_BUTTON_LEFT:                        //ALT && LMB PRESS
                                glfwSetMousePosCallback(MousePosCallback);

                                if (GLGui::lmbPressed == false){
                                    selectNodeAtMousePos();
                                }

                                GLGui::lmbPressed = true;


                                //draw select pass 1
                                //draw select pass 2
                                //get node id
                                //get plug id
                                //if no plug id then select node
                                //move node by mouse offset(dont send model new coords yet, do that on release)
                                //if there is plug id...
                                //store fromPlug and fromNode
                                //enter connect mode (turn on a bool)
                                //check if mouse moved
                                //if true then draw connection line
                                //every time mouse moved, do a draw pass 1&2
                                //test if node plug is hit....
                                //if it is then store node and plug id as target toNode and toPlug
                                //check type to see if they match and are compatible
								//if not then draw NO sign and continue drawing
                                break;

                                case  GLFW_MOUSE_BUTTON_RIGHT:                      //ALT && RMB PRESS
                                glfwSetMousePosCallback(MousePosCallback);
                                //draw pass 1
                                //test if background
								//if it is then bring up node menu
								break;
                                case  GLFW_MOUSE_BUTTON_MIDDLE:                     //ALT && MMB PRESS
                                glfwSetMousePosCallback(MousePosCallback);
                                //pan
                                break;
                                }

                        break;

                        case GLFW_RELEASE: //ALT && RMB
                            switch (keyStore.buttonStore){
                                case GLFW_MOUSE_BUTTON_LEFT:
                                //check if in move or connect mode
                                //if node moved, send message to node model of new coordinates (view is already updated in PRESS function)
                                //if in connect mode then call connectPlugs using the from and to variables as arguments              //ALT && LMB RELEASE
                                lmbPressed = false;
                                glfwSetMousePosCallback(NULL);

                                //
                                break;

                                case  GLFW_MOUSE_BUTTON_RIGHT:                      //ALT && RMB RELEASE
                                glfwSetMousePosCallback(NULL);
                                //
                                break;


                                case  GLFW_MOUSE_BUTTON_MIDDLE:                     //ALT && MMB RELEASE
                                glfwSetMousePosCallback(NULL);
                                break;
                                }
						break;
	}
}


void SFMLHandler::createUIView(Node *nodePointer){

    UIView *tempUIView = new UIView(this, NULL, globalViewCount+1);
    addViewToList(tempUIView);
    int rootInt = tempUIView->globalIndexID;
    rootViews.push_back(rootInt);
    //tempUIView->setNodePointer(nodePointer);
    tempUIView->setPosition(creationX,creationY );
}

void SFMLHandler::createNode(){
    cout << "in createNode" << std::endl;
    Node *nodePointer = viewController->createNode();
    createUIView(nodePointer);
    creationY += 85;
	creationX += 65;
}

void SFMLHandler::connectNodes(int outputNode_id, int inputNode_id, int fromPlugID, int toPlugID ){
    viewController->connectNodes(outputNode_id, inputNode_id, fromPlugID,  toPlugID);
}


void SFMLHandler::addViewToList(UIView *newView){

    if (vUIViewGlobalSpare.size() > 0  ){							//if sparestack has something in it....
            int accessElement = vUIViewGlobalSpare.front();				//store the int value that is stored in the last element of sparestack. this will be the index to the vector position to fill up.
            printf("spare index is %d\n",accessElement);						//print the element to see it in console
            //this->viewIterator = vUIViewStore.begin();					//set iterator to start of vUIViewStore


            vUIViewGlobalStore.at(accessElement-1) = newView;
            vUIViewGlobalSpare.pop_back();
            globalViewCount += 1;
            printf("spare on stack! view count is %d, vector size is %d and spareStack is %d\n",globalViewCount,(int)vUIViewGlobalStore.size(),(int)vUIViewGlobalSpare.size() );
        }

        else	{

        vUIViewGlobalStore.push_back(newView);
        globalViewCount += 1;
        printf("no spares.increasing vector. node count is %d and vector size is %d\n",globalViewCount, (int)vUIViewGlobalStore.size());
        }
}

int SFMLHandler::returnViewCount(){

    return globalViewCount;


}
void SFMLHandler::ZoomOrtho(){

    if (zooming == false){
    keyStore.LMx = keyStore.Mx;
    keyStore.LMy = keyStore.My;
    //get mouse pos
    mouseStoreX = keyStore.Mx;
    mouseStoreY = keyStore.My;



    //get current projection matrices
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
    //flip Y for opengl reasons
    mouseStoreY = (float)viewport[3] - mouseStoreY;
    //get world mouse coordinate
    gluUnProject( mouseStoreX, mouseStoreY , 0.0, modelview, projection, viewport, &posX, &posY, &posZ);

    //cout << "yeah!" <<std::endl;
    //dx = posX -   FS.centerX;
    //dy = posY - FS.centerY;

    }

    zooming = true;

    //do mouse movement detection and increment zoomOut
    //#################################################
    int xDiff = keyStore.Mx - keyStore.LMx;
    //int yDiff = keyStore.My - keyStore.LMy;//

    if (xDiff > 0 && (zoomFactor >= zoomMin ) )   {
        zoomFactor -= zoomInc;
        if  (zoomFactor < zoomMin ) {zoomFactor = zoomMin;}
        }
    else if (xDiff < 0 && (zoomFactor <= zoomMax ))  {
        zoomFactor += zoomInc;
        if (zoomFactor > zoomMax){zoomFactor = zoomMax;}
        }
    //#################################################

    zoomFactorRatio = zoomFactor/zoomFactor0;
    //cout << zoomFactorRatio << std::endl;
    zoomFactor0 = zoomFactor;
    //fill structure with clipping plane values
    //FS.left =  ((FS.centerX - dx - (window_width/2.0))*zoomFactor) +dx;
    //FS.right = ((FS.centerX -dx + (window_width/2.0))*zoomFactor)+dx ;
    //FS.bottom = ((FS.centerY -dy + (window_width/2.0))*zoomFactor)+dy;
    //FS.top =    ((FS.centerY -dy  - (window_width/2.0))*zoomFactor) +dy;

    FS.left = posX - (posX - OLD.left) * zoomFactorRatio;
    FS.right = posX - (posX - OLD.right) * zoomFactorRatio;
    FS.bottom = posY - (posY - OLD.bottom) * zoomFactorRatio;
    FS.top = posY - (posY - OLD.top) * zoomFactorRatio;
    //cout << FS.left << std::endl;

    OLD.left = FS.left;
    OLD.right = FS.right;
    OLD.bottom = FS.bottom;
    OLD.top = FS.top;

    // store last mouse pos for next comparison.
    keyStore.LMx = keyStore.Mx;
    keyStore.LMy = keyStore.My;
}

void SFMLHandler::zoomRelease(){
    cout << " releasing" << std::endl;
    zooming = false;
    keyStore.LMx = 0;
    keyStore.LMy = 0;

    // recenter by taking midpoint between new left and right clipping planes
    //FS.centerX = (FS.right+FS.left)/2.0;
    //FS.centerX = (FS.bottom+FS.top)/2.0;
}

void SFMLHandler::pan(){

if (panning == false){
    keyStore.LMx = keyStore.Mx;
    keyStore.LMy = keyStore.My;
    mouseStoreX = keyStore.Mx;
    mouseStoreY = keyStore.My;
    OLD.left = FS.left;
    OLD.right = FS.right;
    OLD.bottom = FS.bottom;
    OLD.top = FS.top;
    }

    panning = true;
    //int xDiff = keyStore.Mx - keyStore.LMx;
    //int yDiff = keyStore.My - keyStore.LMy;//
    //panOffsetX += (xDiff); //((float)window_width/2.0)*
    //panOffsetY += yDiff;//((float)window_height/2.0)*

    float xOffset =  (keyStore.Mx - mouseStoreX)*zoomFactor;
    float yOffset = (keyStore.My - mouseStoreY)*zoomFactor;


    FS.left = (OLD.left - xOffset);
    FS.right = (OLD.right - xOffset);
    FS.bottom = (OLD.bottom - yOffset);
    FS.top =  (OLD.top - yOffset);



    keyStore.LMx = keyStore.Mx;
    keyStore.LMy = keyStore.My;



}

void GLGui::panRelease(){
    panning = false;
    OLD.left = FS.left;
    OLD.right = FS.right;
    OLD.bottom = FS.bottom;
    OLD.top = FS.top;
    keyStore.LMx = 0;
    keyStore.LMy = 0;
}

void GLGui::DrawGui(){

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(FS.left, FS.right,FS.bottom, FS.top, 100, -100);

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
		//viewController->callUINodeDraw();

		for (drawIterator = rootViews.begin() ; drawIterator < rootViews.end(); drawIterator++){

			vUIViewGlobalStore.at((*drawIterator)-1)->Draw();;

		}
		viewChanged = false;//if view is changed in GUI it will get called
}

void GLGui::DrawSelectPass(){

    for (drawIterator = rootViews.begin() ; drawIterator < rootViews.end(); drawIterator++){

                vUIViewGlobalStore.at((*drawIterator)-1)->DrawSelectPass();;

            }
            viewChanged = false;//if view is changed in GUI it will get called



}

void GLGui::CloseGUI(int return_code){
  glfwTerminate();
  exit(return_code);
}

void GLGui::selectNodeAtMousePos(){

        glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//glfwGetMousePos( &Mouse.Mx, &Mouse.My );
		DrawSelectPass();

		unsigned char *pRGB;

		pRGB = new unsigned char[4];

		glReadPixels( keyStore.Mx, window_height-keyStore.My, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pRGB);

		int id = (pRGB[ 0 ]) + (pRGB[ 1 ]*256) + (pRGB[ 2 ]*256*256);
		if (!id == 0){
			//int nodeID = vUIViewGlobalStore.at(id-1)->getPointedToNodeID();
			//viewController->setCurrentSelectedNode(nodeID);

		}
		else printf("selected background with id of %d\n", id);

		delete(pRGB);
		glClearColor(0.5, 0.5, 0.5, 1.0);


}



void GLGui::setViewController(UIViewController *controller){


viewController = controller;


}

