 - add override to vpcontroller (and final?)
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
 - file viewer
 - dont store glfw window in UIWindow. UI window shoud be backend agnostic. we should store a map in the controller class since it owns the window and connects everything together
 - glgui should really be an interface to allow sublassing for ANY API
 - glgui in its callback doesn't need to call refresh on the UIWindow directly.
 - why does the UIwindow have a loop? make it happen in controller? just do a loop over the owned windows and swap them.
 - need a way to have instanced views over multiple windows so if we move a node in a canvas, it is reflected in the other window (maybe just check if its an instance with a bool and when drawing, send a notify to other windows to update instanced views. the window will need to store a sepeate vector of instanced view id to directly update them without having to draw from root and check all).
- glgui only deals with glfw specific stuff. we need a separate renderer that actually does gl stuff. GLRenderer, vulkan renderer etc. this is owned by application/controller too. it just knows how to get the rects/geo from the window. maybe with sfml, these can be one object? ie inherit from both as it is both the windowing lib and the renderer (if we want to use that too instead of raw gl)
- why is the windowResized needed in processEvents?
- processEvents should come out of glfwgui. instead, the callbacks should set the keystoreinfo in the appcontroller. then the controller can call handle events on the windows directly without the glfwgui needing to know about uiwindows



1) move loop to application
2) remove from window


 separation plan
 - controller has a vector of window handle/id to UIWindows
 - we create the window in controller, then in GLFWGui, when we add the window, we add the handle/id and set the userpointer to the int value. application->createWindow calls the GLFW to create its window, then creates the UIWindow.
 - in the resize callbacks. we get user pointer and cast to int. use that to call resize on the controller with the id and the controller knows how to call the UI window resize
 - if moving the loop out of the window itself and into the controller (like qt application), then we need a way to swapbuffers and pollevents without knowing the type of windows lib. how do we pass window to glgui to tell it which one? the glgui will have to store its own id to glfwwindow stack. the ids that are in the controller will have to be in sync. we will also need to provide an interface for polling and waiting.


any glfwgui backend class NEEDS to
 - call app->setSize(int windowID, width, height);
 - call app->refresh(int windowID)
 - when buttons are pushed, call app->setKeyStore 