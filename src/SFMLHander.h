#ifndef SFMLHANDLER_H
#define SFMLHANDLER_H



#include "/local1/delliott/SFML-2.0-rc/include/SFML/OpenGL.hpp"


class UIViewController;
class Node;

class SFMLHandler{

private:
    UIViewController *viewController;
    vector<UIView*> vUIViewGlobalStore;//all view including children are stored here so we can grab selection and forward messages accordingly
    vector<int> vUIViewGlobalSpare;//index of spare slots to fill ids

    vector<int> rootViews; //index of view in vUIGlobalStore that we need to render.
    vector<int> spareRootIds;//yada yada yada
    vector<int>::iterator drawIterator;


public:

    SFMLHandler();
    virtual ~SFMLHandler();

    float creationX;
	float creationY;
    int window_width, window_height;
    //int origWidth = window_width, origHeight = window_height;
    GLfloat aspect_ratio;
    bool doSelection;
    bool escapePressed;
    bool altPressed;
    bool lmbPressed;
    bool mmbPressed;
    bool rmbPressed;
    bool zooming;
    bool panning;
    GLdouble mouseStoreX;
    GLdouble mouseStoreY;
    GLdouble dx;
    GLdouble dy;
    GLdouble zoomFactor0;
    GLdouble zoomFactorRatio;
    GLdouble zoomFactor;
    GLdouble zoomInc;
    GLdouble zoomMin;
    GLdouble zoomMax;
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;//mouse values to pass
    GLdouble posX, posY, posZ;//holds final values

    struct frustrumStruct{
        GLfloat centerX;
        GLfloat centerY;
        GLfloat left;
        GLfloat right;
        GLfloat top;
        GLfloat bottom;};

    frustrumStruct OLD;
    frustrumStruct FS;

    bool viewChanged; //for checking if redraw is needed.

    struct keyStoreStruct{
            int Mx,My;
            int LMx, LMy; //mouse potitions last frame
            int key;
            int keyActionStore;
            int buttonStore;
            int mouseActionStore;};

    static keyStoreStruct keyStore;

    //AppCore* mainCore; //pointer to main app core which is the owner.
    const char *guiName;
    void InitGL(const char *name);
    void DrawGui();
    void DrawSelectPass();
    void GLLoop();

    void CloseGUI(int return_code);
    void ResizeWindow();
    void processEvents();//called in main loop and checkes key presses to maniuplate drawing of ui and perform functions. IE. interface!
    void selectNodeAtMousePos();
    //GUI view stuff........
    void createUIView(Node *nodePointer);
    void addViewToList(UIView *newView);
    void createNode();
    void connectNodes(int outputNode_id, int inputNode_id, int fromPlugID, int toPlugID );


    int globalViewCount;
    int returnViewCount();
    void ZoomOrtho();
    void zoomRelease();
    void pan();
    void panRelease();
    void MouseDown();
    void MouseUp();
    void setViewController(UIViewController *controller); // can dynamically change viewController so we can have different behaviour at runtime



};






#endif
