from PythonQt import QtGui, Qt
from gepetto.corbaserver import Client

def separator():
    line = QtGui.QFrame()
    line.frameShape = QtGui.QFrame.HLine
    line.frameShadow = QtGui.QFrame.Sunken
    return line

### This class represents one special tab of the new QDockWidget
class _Widget (QtGui.QWidget):
    def __init__(self, parent, plugin):
        super(_Widget, self).__init__ (parent)
        self.plugin = plugin
        self.bodies = []
        self.makeWidget ()

    def makeWidget(self):
        box = QtGui.QVBoxLayout(self)

        box.addWidget(self.bindFunctionToButton("Get selected bodies", self.updateBodyList))

        box.addWidget(QtGui.QLabel("Current selected bodies:"))
        self.bodyList = QtGui.QListWidget()
        self.bodyList
        box.addWidget(self.bodyList)

        box.addWidget(separator())

        box.addWidget(self.bindFunctionToButton("Export model", self.exportModel))

        box.addWidget(separator())

        self.transformFrame = QtGui.QLabel ("output.yaml")
        box.addWidget(self.addWidgetsInHBox( [
            self.transformFrame, 
            self.bindFunctionToButton("Select transform file", self.changeTransformFile)
            ]))

        box.addWidget(separator())

        onRefresh = self.bindFunctionToButton("Automatic export", self.setOnRefresh)
        onRefresh.checkable = True
        box.addWidget(onRefresh)

        box.addWidget(self.bindFunctionToButton("Write current frame", self.writeCurrentFrame))

    def updateBodyList(self):
        self.bodies = [ str(b.text()) for b in self.plugin.main.bodyTree().selectedBodies() ]
        self.bodyList.clear()
        for b in self.bodies: self.bodyList.addItem(b)

    def exportModel(self):
        fn = QtGui.QFileDialog.getSaveFileName()
        self.plugin.gui.writeBlenderScript(str(fn), self.bodies)

    def changeTransformFile(self):
        fn = QtGui.QFileDialog.getSaveFileName()
        self.transformFrame.text = fn
        self.plugin.gui.setCaptureTransform(str(fn), self.bodies)

    def writeCurrentFrame(self):
        self.plugin.gui.captureTransform()

    def setOnRefresh(self, checked):
        self.plugin.gui.captureTransformOnRefresh(checked)

    def addWidgetsInHBox(self, widgets):
        nameParentW = QtGui.QWidget(self)
        hboxName = QtGui.QHBoxLayout(nameParentW)
        for w in widgets:
            hboxName.addWidget(w)
        return nameParentW

    def bindFunctionToButton (self, buttonLabel, func):
        button = QtGui.QPushButton(self)
        button.text = buttonLabel
        button.connect ('clicked(bool)', func)
        return button

class Plugin(QtGui.QDockWidget):
    """
    Blender export helper plugin
    """
    def __init__ (self, mainWindow, flags = None):
        if flags is None:
            super(Plugin, self).__init__ ("Blender export plugin", mainWindow)
        else:
            super(Plugin, self).__init__ ("Blender export plugin", mainWindow, flags)
        self.resetConnection()
        # Initialize the widget
        mainWidget = _Widget(self, self)
        self.setWidget (mainWidget)
        self.main = mainWindow

        self.main.registerShortcut(self.windowTitle, self.toggleViewAction())

    def resetConnection(self):
        self.client = Client()
        self.gui = self.client.gui
