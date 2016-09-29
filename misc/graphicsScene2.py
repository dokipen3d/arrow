
import sys
from PySide import QtGui, QtCore

#custom graphicsScene to overide items() for intersection highlighting. 
#Mouse tracking needs to be enabled on the view with view.setMouseTracking(True)

class dkGraphicsScene(QtGui.QGraphicsScene):
    def __init__(self, parent = None):
        QtGui.QGraphicsScene.__init__(self,parent)

        #store curve type for testing on intersection
        self.CurveType = type(Curve())
        self.itemsUnderCursor = []
        self.itemsUnderCursorToClear = set()


    #when mouse moves, call items to see what intersects
    def mouseMoveEvent(self, event):
        QtGui.QGraphicsScene.mouseMoveEvent(self,event)
        mousePos = event.scenePos()
        #print(mousePos)
        
        for item in self.itemsUnderCursorToClear:
            if type(item) is self.CurveType:
                #print "yay1!"
                item.setHighLightOff()
                item.update()


        self.itemsUnderCursor = self.items(QtCore.QRectF(mousePos.x()-2, mousePos.y()-2, 4,4))
       
        # for item in items:
        #     print(type(item))

        #do selection highlight here!
        for item in self.itemsUnderCursor:
            if type(item) is self.CurveType:
                #print "yay!"
                item.setHighLight()
                item.update()
                self.itemsUnderCursorToClear.add(item)



class Curve(QtGui.QGraphicsItem):
    def __init__(self, parent = None):
        QtGui.QGraphicsItem.__init__(self,parent)
        self.path = QtGui.QPainterPath()
        #self.path.moveTo(20, 300)
        #self.path.lineTo(240, 30)
        self.path.cubicTo(0, 50, 100, 50, 100, 100)

        #disable selection for now. itemChange function not really needed then
        #self.setFlag(QtGui.QGraphicsItem.ItemIsSelectable)
        self.setCacheMode(QtGui.QGraphicsItem.DeviceCoordinateCache);

        self.pen = QtGui.QPen(QtGui.QPen(QtCore.Qt.black, 1, QtCore.Qt.SolidLine))
        self.bHighlight = False

    #overrise paint function
    def paint(self, painter, option, widget):
        

        painter.setPen(self.pen)
        painter.setRenderHint(QtGui.QPainter.Antialiasing)
        if self.bHighlight == True:
            self.pen = QtGui.QPen(QtGui.QPen(QtGui.QColor(0,255,255), 1, QtCore.Qt.SolidLine))
        else:
            self.pen = QtGui.QPen(QtGui.QPen(QtCore.Qt.black, 1, QtCore.Qt.SolidLine))

        painter.drawPath(self.path)
        self.bHighlight = False

    def boundingRect(self):
        return  self.path.boundingRect()

        #for changing colour based on selection status
    def itemChange(self, change, value):
        if change == QtGui.QGraphicsItem.ItemSelectedHasChanged:
            if value == False:
                print "deselcted"  
                self.pen = QtGui.QPen(QtGui.QPen(QtCore.Qt.black, 1, QtCore.Qt.SolidLine))
            if value == True:
                print "selected"
                self.pen = QtGui.QPen(QtGui.QPen(QtCore.Qt.white, 1, QtCore.Qt.SolidLine))
        return QtGui.QGraphicsItem.itemChange(self, change, value)

    #returns a custom shape that defines where the graphics item is selectable. we use path stroker to get more detailed than a rect
    def shape(self):
        stroker = QtGui.QPainterPathStroker()
        stroker.setWidth(4)
        outlinePath = stroker.createStroke(self.path)
        return outlinePath

    def setHighLight(self):
        self.bHighlight = True

    def setHighLightOff(self):
        self.bHighlight = False



class Node(QtGui.QGraphicsItem):
    def __init__(self, parent = None):
        QtGui.QGraphicsItem.__init__(self,parent)

        #make item movable
        self.setFlag(QtGui.QGraphicsItem.ItemIsMovable)
        
        #make item selectable
        self.setFlag(QtGui.QGraphicsItem.ItemIsSelectable)
        

        #local class variable for node size (used in paint function)
        self.textItem = QtGui.QGraphicsSimpleTextItem("hello",self)

        #set local rect size to be adjusted to text size
        #self._rect =  QtCore.QRectF(0,0 , 200, 80)
        self._rect = self.textItem.boundingRect()
        self._rect.adjust(-40,-20,40,20)
        self.setCacheMode(QtGui.QGraphicsItem.DeviceCoordinateCache);

        self.x = self._rect.x()
        self.y = self._rect.y()
        self.width = self._rect.width()
        self.height = self._rect.height()

        self.selected = False
        self.selectedColour = QtGui.QColor(0,255,0)
        
    #override otherwise wont draw
    def boundingRect(self):
        return  self._rect

        

    #overrise paint function
    def paint(self, painter, option, widget):
        # show boundingRect for debug purposes
        #painter.setPen(QtGui.QPen(QtCore.Qt.red, 0, QtCore.Qt.SolidLine))
        painter.setBrush(QtGui.QBrush(self.selectedColour))
        painter.setPen(QtGui.QPen(QtCore.Qt.black, 1, QtCore.Qt.SolidLine))
        painter.setRenderHint(QtGui.QPainter.Antialiasing)

    
        painter.drawRoundedRect(QtCore.QRectF(self.x+1, self.y+1,  self.width-1, self.height-1), 10, 10)

        
        # Paint rectangle
        #painter.drawRoundedRect(self._rect, 10,10)
        #painter.drawText(self._rect, QtCore.Qt.AlignCenter,"dkFluidObject")

    #for changing colour based on selection status
    def itemChange(self, change, value):
        if change == QtGui.QGraphicsItem.ItemSelectedHasChanged:
            if value == False:
                print "deselcted"  
                self.selectedColour =  QtGui.QColor(0,255,0)     
            if value == True:
                print "selected"
                self.selectedColour =  QtGui.QColor(255,0,0)   

        return QtGui.QGraphicsItem.itemChange(self, change, value)

app = QtGui.QApplication(sys.argv)

scene = dkGraphicsScene()
#scene.addText("test")

bgBrush = QtGui.QBrush(QtGui.QColor(55,55,55))
scene.setBackgroundBrush(bgBrush)

item1 = Node()
item2 = Node()
item2.setPos(100,100)

curve = Curve()
scene.addItem(item1)
scene.addItem(item2)
scene.addItem(curve)


view = QtGui.QGraphicsView(scene)
view.setFixedSize(640,480)
view.setMouseTracking(True)
view.show()
view.raise_()

sys.exit(app.exec_())