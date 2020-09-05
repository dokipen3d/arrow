 
Idea for vertex representation

2 floats for 2d lower left origin of quad.
1 float for angle of where upper left point is on the 0 plane
1 float is the distance from origin

Depth?


1) move init GL to GLFWGUI and somehow forward width/height
 2) remove all viewController from window. (will now access using static methods)
 3) look at UIView init(). replace the check for root window. sort out registering etc
 
 - add override to vpcontroller (and final?)
 - if UIView doesn't have a parent, then it should create it's own internal window member, somehow add it to the application (static method) and embed iself in the window. If we then reparent the UIView by adding it to another UIView (layout/viewportcontroller or another window) then we 
 - drawGUI needs to return a bool bNeedsUpdate to say if anything acually was redrawn (if drags changed the layout, or selection caused a highlight).
 - if something needs to update from the app itself, do we just swap directly or do we post an empty event?
 - GLFWGui needs to take a pointer to the structure that store the keys/actions. that way it doesn't need to store the Application
 - rename keyStore to inputInfo and windowRect to windowInfo?
- rename apploop in application to exec?
 - window currently handles events, but we should abstract the button presses to be api agnostic. 
 - add a vector of UI rects to draw to the UI window. Instead of glbegin/glend, write into the vector and draw all at once. this will allow us to update only the existing views needed (as long as none are being added/removed). Also we can then map memory for opengl.
 - instead of setting the gl viewport, we will now need to actually write the final positions (or have a vector of transforms that the shader uses). as we draw, we can update the transform so that children can query it and update themselves. 
 - change iters to auto
 - use unique ptrs
 - remove check gl error from window
 - remove scale factor from window and create a dedicated canvas view (viewport?) that can zoom itself and has a grid acceleration structure
 - be able to hide/unhide viewports
 - draw select pass once instead of every mouse move.
 - file menu with special view types for drop downs that store std functions.
 - tree viewer
 - tab view
 - slider
 - button
 - dialog
 - label
 - file viewer
 - for nodes views, they will need a 'border' to be able to select and resize
 - dont store glfw window in UIWindow. UI window shoud be backend agnostic. we should store a map in the controller class since it owns the window and connects everything together
 - glgui should really be an interface to allow sublassing for ANY API
 - glgui in its callback doesn't need to call refresh on the UIWindow directly.
 - why does the UIwindow have a loop? make it happen in controller? just do a loop over the owned windows and swap them.
 - need a way to have instanced views over multiple windows so if we move a node in a canvas, it is reflected in the other window (maybe just check if its an instance with a bool and when drawing, send a notify to other windows to update instanced views. the window will need to store a sepeate vector of instanced view id to directly update them without having to draw from root and check all).
- glgui only deals with glfw specific stuff. we need a separate renderer that actually does gl stuff. GLRenderer, vulkan renderer etc. this is owned by application/controller too. it just knows how to get the rects/geo from the window. maybe with sfml, these can be one object? ie inherit from both as it is both the windowing lib and the renderer (if we want to use that too instead of raw gl)
- why is the windowResized needed in processEvents?
- processEvents should come out of glfwgui. instead, the callbacks should set the keystoreinfo in the appcontroller. then the controller can call handle events on the windows directly without the glfwgui needing to know about uiwindows
- when we create a window, we should either pass in the application in the constructor or have it a global thing we can access. pass in nullptr as the UIView (sender, currently its wrong!).
- main window can register itself upon construction
- change registerView to addView. also have it return the pointer so we can call make_shared in the function itself. so auto view = window->addView(make_shared)
- make a pure virtual interface for the windower backend (GLFWGUI)
- also a pure virtual interface for the renderer
- make viewportcontroller a bit more like qsplitter with the ability to have multiple splits and chile widgets. will need to make splits 'redistribute' as you move one AND add the ability to hold a modifier key to hold other split handles.

 separation plan
 - controller has a vector of window handle/id to UIWindows
 - we create the window in controller, then in GLFWGui, when we add the window, we add the handle/id and set the userpointer to the int value. application->createWindow calls the GLFW to create its window, then creates the UIWindow.
 - in the resize callbacks. we get user pointer and cast to int. use that to call resize on the controller with the id and the controller knows how to call the UI window resize
 - if moving the loop out of the window itself and into the controller (like qt application), then we need a way to swapbuffers and pollevents without knowing the type of windows lib. how do we pass window to glgui to tell it which one? the glgui will have to store its own id to glfwwindow stack. the ids that are in the controller will have to be in sync. we will also need to provide an interface for polling and waiting.


any glfwgui backend class NEEDS to
 - call app->setSize(int windowID, width, height);
 - call app->refresh(int windowID)
 - when buttons are pushed, call app->setKeyStore 




 UIWindow, Application and GLFWGUI relationship
 - GLFW events need to tell UIWindow to resize through Application 
 - Application has a createWindow function that returns a UIWindow* (so that UIViews can create them). unique_ptr<UIwindow> in Application. This must call the glfwgui to create the GLFWwindow and there must be a window id (stored in UIWindow?) and also set in the user pointer! we store it in the UIWindow so that upon destruction, we can tell the application to delete the glfw window 



BUGS
1) getWorldPos needs to recursively go up and accumulate the difference between the current viewRect and its parents viewRect

REMOVING THE NEED FOR ADDSUBVIEWCONTROLLER
1) when adding a viewportController to a view , simply make the viewportController take on the dimensions and transform of the enclosing view
    - change (or add) the constructor to not take width and height, but just inherit the parent.
    - the viewport is the global opengl viewport so need to recursively find the global coords by traversing up. could use caching to early stop

    this is kind of done. we added the expandable flag, but we don't inherit the transform yet?
2) also when adding a viewport controller to another viewport controller, use one of the viewports as the enclosing view
    - will need to add another constructor to take the index of which VP to use as the encloser.
3) will need to make finding the global positions a solid process.
4) find out exactly when we need to resolve dimensions
5) add flags to check if resolving is actually needed

SIMPLE VIEWPORTCNTRLR?
- why not just make UIView the one that does the viewport? then viewport controller is just a type of splitter that has a divider
- do we need a simple viewport controller that all views have so that they can embed children? I guess a layout would do that? or layout would be the more advanced version of that much like the current vpcntlr is
- also dont need to make viewportcontroller have 3 views that you parent to? altough that is useful for having a background colour.
- ultimatley this is all just to get geometry and as long as the hight level views that the user uses are have a nice interface, does it matter how many views we have under hood to magange heirarchy (apart from performance of course)
- is it as simple as getting the world pos in UIVIew and then before drawing CHILDREN, set the viewport to global coords? drawing itself in viewRect coords its fine because it's assumed that the parent UIView would have set the viewport (like the current one has just done for its children) 

SORT OUT resolving size happening more than once. figure out which event it is coming from and only do it once.

IDEA: have a pool for each type of view that we allocate from, and as we resolve, we sort the views in the pool? OR could switch to variants and use the static vector (or static sparestack?) to put the variants in. then we sort those as we resolve which makes updating them next time much faster possibly 

NOTE: with UIView controlling their own viewport, we won't need the virtual draw functions. UIview::draw *should* be enough for all subclassed object to use directly. they just need to take care of their own local children and WHAT they are as opposed to HOW they draw.




plan next
1) change global update to so that it happens during resolve and is cached. add a flag to make it known that global update is ready. not known yet if that will be needed.
2) make UIView control it's own viewport so that it makes it easier to sublass and make custom viewcontrollers (like 'divider/splitter') and (layouts) that just draw in local coords
3) will we need to change the divider resize global sub views? might just mean we remove the global coords and dont do viewport change in that. all new classes wont need to either!