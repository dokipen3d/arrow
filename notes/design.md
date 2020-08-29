
ViewController
What main uses
this is something that users should create?
Not a view
Ties the window, app and renderer together
Where we enter the main loop
AppController?
Maybe have a create window?



View
Base class
Stores pointer to its owning window (which is a view also)
Knows its parent id (int)
Keeps a vector of int children
The UI is basically a tree of views connected by their parent and a vector



Window : View
Single OS window which can contain subviews
Owns all uiviews for that window in a vector. They are retrievable by their pos/handle/id
Shouldn’t require users register the window into itself. Should find a way to make the window do that itself

Viewport : View
Only owned right now by a viewportController
Can control the drawable area (in effect its the GL viewport, so sets where the children will be drawn)
Can be zoomable
Basically just sets glviewport
Uses
globalRect
viewRect
Adds
Zoomfactors
Projections matrices
Helpers for mouse positions

ViewportController : View
Do we want to spit this into two objects? A view divider and a zoomableView?


AppCore
Deals with our nodes
Currently knows about ViewController (but doesn’t do anything with it, do we need it? UI will set stuff on appcore, but does the app need to update the UI? Prob but need to look into observer and RPC calls/signals)
