#pragma once

#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "PlugTypes.h"
#include <string>




using namespace std;

class UIWindow;
class Node;
struct keyStoreStruct;


class UIView {

    protected:

        // needs to be before rootWindow so that constructor can test it against self
        //store a pointer to main window. We dont own it so its a raw pointer
        //only need to refer to it.
        UIView* parent;

         // to be able to create a uiview and have it own its own window that is embedded in. also needs to be before rootWindow otherwise it gets reinitialized to nullptr after potentially embedding a window
        std::unique_ptr<UIWindow> embeddedWindow = nullptr;

        UIWindow *rootWindow;
       
        //store indices to children.
        vector<int> children;

        //how many view are its children. mainly for controlling vpCntlr dividers
        std::size_t viewCount();

        //helper variables for drawing in selection mode
        GLubyte drawIDColour[3];
        GLfloat viewColour[4];


        bool drawable;
        bool childrenDrawable;
        bool registered;

        //local helper storage for control management
        bool lmbPressed;;


    public:



        UIView(UIView* parent, int width, int height, std::string text = {});
        UIView(const UIView& uiview) = default;
        virtual ~UIView();
        virtual void Draw();// make it virtual so that it will call the derived function even if we refer to the object as a UIView
        void DrawSubViews();
        virtual void DrawSelectPass();
        void DrawSelectPassSubViews();
        virtual void viewClicked(keyStoreStruct key, int senderID);//default just calls parent viewClicked. if parent == NULL then do nothinng.
        virtual void viewDragged(keyStoreStruct key, int senderID);
        virtual void viewReleased(keyStoreStruct key, int senderID);
        virtual void resolveSize();//to notify children of parent resizing.
        virtual void addSubView(UIView* newView);


        //void setRoot(GLGui *root); //done in constructor
        void movePosition(float moveX, float moveY);
        void setPosition(float setX, float setY);
        void setSize(int sizeX, int sizeY);
        void offsetSize(int sizeX, int sizeY);
        UIRect getRect();
        void setDrawable(bool amIdrawable);
        void setChildrenDrawable(bool areChildrenDrawable);
        void setColour (GLfloat R, GLfloat G, GLfloat B, GLfloat A);
        UIView* Parent();
        UIPoint getWorldPos();
        void printID();
        std::size_t id();
        void setId(std::size_t id);
        void setLocalId(std::size_t id);
        void setParent(UIView* newParent);
        void deRegisterChildren();
        bool isRootWindow();
        void removeFromChildren(std::size_t id);
        std::string text;
        int width(){return viewRect.size.width;};
        int height(){return viewRect.size.height;};


        //bool stopWorldPosSearch;//for VPts to know when they don't need to search any more higher.

        UIRect globalRect;
        UIRect viewRect;
        UIRect fbRect;
        int globalIndexID;
        int localID;


};
