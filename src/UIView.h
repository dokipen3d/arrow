#ifndef UIVIEW_H
#define UIVIEW_H

#include <vector>
#include <GL/glew.h>
//#define GLEW_STATIC
#include <GLFW/glfw3.h>
#include "PlugTypes.h"




using namespace std;

class UIWindow;
class Node;
struct keyStoreStruct;



class UIView {


    protected:

        //store a pointer to main window. We dont own it so its a raw pointer
        //only need to refer to it.
        UIWindow *rootWindow;

        //the id of this views parent. We can ask the main window to give it to us
        int parentViewID;

        //store indexes to children.
        //only store indexes because I only want ONE place to store the actuall addresses of all the views
        //best place I can think of is in the window that owns them all.
        //As soon as I start having multiple places where addresses can be stores it can get complex.
        vector<int> UIViewIndexStore;
        vector<int>::iterator viewIndexIterator;

        //how many view are its children. mainly for controlling vpCntlr dividers
        int viewCount;

        //helper variables for drawing in selection mode
        GLubyte drawIDColour[3];
        GLfloat viewColour[4];


        bool drawable;
        bool childrenDrawable;

        //local helper storage for control management
        bool lmbPressed;


    public:



        UIView(UIWindow *root, int width, int height);//HAVE to pass a root node that keeps track of this globally
        virtual ~UIView();
        virtual void Init();
        virtual void Draw();// make it virtual so that it will call the derived function even if we refer to the object as a UIView
        void DrawSubViews();
        virtual void DrawSelectPass();
        void DrawSelectPassSubViews();
        virtual void viewClicked(keyStoreStruct key, int senderID);//default just calls parent viewClicked. if parent == NULL then do nothinng.
        virtual void viewDragged(keyStoreStruct key, int senderID);
        virtual void viewReleased(keyStoreStruct key, int senderID);
        virtual void resolveSize();//to notify children of parent resizing.

        //void setRoot(GLGui *root); //done in constructor
        void movePosition(float moveX, float moveY);
        void setPosition(float setX, float setY);
        void setSize(int sizeX, int sizeY);
        void offsetSize(int sizeX, int sizeY);
        UIRect getRect();
        void setDrawable(bool amIdrawable);
        void setChildrenDrawable(bool areChildrenDrawable);
        void setColour (GLfloat R, GLfloat G, GLfloat B, GLfloat A);
        UIView* getParent();
        UIPoint getWorldPos();
        void addSubView(UIView* newView);
        void printID();
        void deRegisterChildren();


        //bool stopWorldPosSearch;//for VPts to know when they don't need to search any more higher.

        UIRect globalRect;
        UIRect viewRect;
        UIRect fbRect;
        int globalIndexID;
        int localID;
        void setParentID(int parentID);


};

#endif
