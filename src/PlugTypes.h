
#ifndef PLUGTYPES_H_
#define PLUGTYPES_H_

#include <GL/glew.h>
#include "/usr/local/include/GLFW/glfw3.h"

#include <cstdio>
#include <cstdlib>

//public member variables. need to make public so that appcore can do type testing.
    enum plugType_t {DUMMY, GENERIC, TRANSFORM, VOLUME, COLLISIONHINT, FLOAT, VECTOR, BOOLTYPE, EXTERNAL_FLOAT, EXTERNAL_VECTOR, COLOUR, ALPHA, ARB_CHANNEL} ;//different plugtypes to test if connection allowed. EXTERNAL is for plugs that are not affected by the graph so can be used in a cyclic way (downstream nodes plugging into upstream nodes
	enum plugDirection_t {IN,OUT};//So we can test that an outgoing plug can only connect to an incoming and vice versa.

	enum guiEntity_t {NODEVIEW, FLOAT_SLIDER, INT_SLIDER, TEXTBOX};//
	enum orientation {HORIZONTAL, VERTICAL};

	struct keyStoreStruct{
            int Mx,My;
            int LMx, LMy; //mouse potitions last frame
            int key;
            int keyActionStore;
            int buttonStore;
            int mouseActionStore;
            };

    struct frustrumStruct{
        float centerX;
        float centerY;
        float left;
        float right;
        float top;
        float bottom;
        };


    struct UIPoint {//position
        UIPoint() : x(0.0), y(0.0) {}
        UIPoint(float X, float Y) : x(X), y(Y) {}
        int x;
        int y;
    };



    struct UISize{//dimensions

        int width;
        int  height;
    };

    struct UIRect{
        UIPoint point;
        UISize size;
    };




/*
	struct CGPoint{

        float x;
        float y;
	}

    struct CGSize{

        float width;
        float height;
    }

    struct CGRect{

        CGPoint point;
        CGSize size;

    }

*/

#endif
