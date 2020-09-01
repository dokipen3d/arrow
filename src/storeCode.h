

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


void createNode();
void GLGui::createNode(){
    //cout << "in createNode" << std::endl;
    Node *nodePointer = viewController->createNode();
    createUIView(nodePointer);
    creationY += 85;
	creationX += 65;
}


void createUIView(Node *nodePointer);
void GLGui::createUIView(Node *nodePointer){

    UIView *tempUIView = new UIView(this, NULL, globalViewCount+1);
    addViewToList(tempUIView);
    int rootInt = tempUIView->globalIndexID;
    rootViews.push_back(rootInt);
    tempUIView->setNodePointer(nodePointer);
    tempUIView->setPosition(creationX,creationY );
}








int GLGui::returnViewCount(){

    cout << "returning viewCount" << std::endl;
    return globalViewCount;


}

UIView* getNodeFromID(int id);

void GLGui::ZoomOrtho(){

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

void GLGui::zoomRelease(){
    cout << " releasing" << std::endl;
    zooming = false;
    keyStore.LMx = 0;
    keyStore.LMy = 0;

    // recenter by taking midpoint between new left and right clipping planes
    //FS.centerX = (FS.right+FS.left)/2.0;
    //FS.centerX = (FS.bottom+FS.top)/2.0;
}

void GLGui::pan(){

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

		for (drawIterator = rootViews.begin() ; drawIterator < rootViews.end(); drawIterator++){

			vUIViewGlobalStore.at((*drawIterator)-1)->Draw();;

		}

}

void selectNodeAtMousePos();
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

		}
		else printf("selected background with id of %d\n", id);

		delete(pRGB);
		glClearColor(0.5, 0.5, 0.5, 1.0);


}


    void ZoomOrtho();
    void zoomRelease();
    void pan();
    void panRelease();



switch (keyStore.mouseActionStore){
        case GLFW_PRESS:// MOUSE BUTTON CLICKED
            switch (keyStore.buttonStore){
                case GLFW_MOUSE_BUTTON_LEFT:      //LMB PRESS
                eventHandler->enableMousePosCallback();

                if (lmbPressed == false)
                {
                    //draw select pass because nothing is selected right now (or shouldnt be)
                    //selectNodeAtMousePos();
                    cout << "left mouse button clicked" << endl;
                }

                lmbPressed = true;

                break;

                case  GLFW_MOUSE_BUTTON_RIGHT:                      //RMB PRESS
                eventHandler->enableMousePosCallback();
                break;

                case  GLFW_MOUSE_BUTTON_MIDDLE:                     //MMB PRESS
                eventHandler->enableMousePosCallback();
                //pan
                break;
                }

        break;

        case GLFW_RELEASE: //MOUSE RELEASE
            switch (keyStore.buttonStore){
                case GLFW_MOUSE_BUTTON_LEFT:                //LMB RELEASE
                lmbPressed = false;
                eventHandler->disableMousePosCallback();

                //
                break;

                case  GLFW_MOUSE_BUTTON_RIGHT:                      //RMB RELEASE
                eventHandler->disableMousePosCallback();
                //
                break;


                case  GLFW_MOUSE_BUTTON_MIDDLE:                     //MMB RELEASE
                eventHandler->disableMousePosCallback();
                break;
                }
        break;
	}


float x2 = x + a->c[*p].bl * sx;

        float y2 = -y - a->c[*p].bt * sy;
        cout << x2 << " " << y2 << endl;
        float w = a->c[*p].bw * sx;
        float h = a->c[*p].bh * sy;
        /* Advance the cursor to the start of the next character */
        x += a->c[*p].ax * sx;
        y += a->c[*p].ay * sy;
        /* Skip glyphs that have no pixels */
        if (!w || !h)
            continue;

        point tmpPoint = {x2, -y2, a->c[*p].tx, a->c[*p].ty};
        coords[co++] = tmpPoint;
        tmpPoint = {x2 + w, -y2, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty};
        coords[co++] = tmpPoint;
        tmpPoint = {x2, -y2 - h, a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h};
        coords[co++] = tmpPoint;
        tmpPoint = {x2 + w, -y2, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty};
        coords[co++] = tmpPoint;
        tmpPoint = {x2, -y2 - h, a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h};
        coords[co++] = tmpPoint;
        tmpPoint = {x2 + w, -y2 - h, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty + a->c[*p].bh / a->h};
        coords[co++] = tmpPoint;
