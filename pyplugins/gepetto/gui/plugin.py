from PythonQt import QtGui, Qt
from gepetto.corbaserver import Client
import sys
sys.argv = ["none"]

class _NodeCreator (QtGui.QWidget):
    def __init__(self, parent, client):
        super(_NodeCreator, self).__init__ (parent)
        self.client = client
        box = QtGui.QVBoxLayout(self)

        # Name line edit
        self.nodeName = QtGui.QLineEdit("nodeName")
        box.addWidget(self.addWidgetsInHBox([QtGui.QLabel("Node name:"), self.nodeName]))

        # Create group
        box.addWidget(self.bindFunctionToButton("Create group", self.createGroup))

        # Add to group
        self.groupNodes = QtGui.QComboBox(self)
        self.groupNodes.editable = False
        box.addWidget(self.addWidgetsInHBox( [
            self.groupNodes,
            self.bindFunctionToButton("Add to group", self.addToGroup)
                ]))

        # Add mesh
        box.addWidget(self.bindFunctionToButton("Add mesh", self.addMesh))

        self.update()

    def update(self):
        self.groupNodes.clear()
        for n in self.client.gui.getSceneList():
            self.groupNodes.addItem (n)

    def addWidgetsInHBox(self, widgets):
        nameParentW = QtGui.QWidget(self)
        hboxName = QtGui.QHBoxLayout(nameParentW)
        for w in widgets:
            hboxName.addWidget(w)
        return nameParentW

    def bindFunctionToButton (self, buttonLabel, func):
        button = QtGui.QPushButton(self)
        button.text = buttonLabel
        button.connect ('clicked()', func)
        return button

    def addMesh (self):
        filename = QtGui.QFileDialog.getOpenFileName (self, "Choose a mesh")
        self.client.gui.addMesh(str(self.nodeName.text), str(filename))

    def createGroup (self):
        self.client.gui.createGroup(str(self.nodeName.text))

    def addToGroup (self):
        self.client.gui.addToGroup(str(self.nodeName.text), str(self.groupNodes.currentText))

class Plugin(QtGui.QDockWidget):
    """" Extra functionalities of the Gepetto Viewer GUI """
    def __init__ (self, mainWindow, flags = None):
        if flags is None:
            super(Plugin, self).__init__ ("Gepetto Viewer plugin", mainWindow)
        else:
            super(Plugin, self).__init__ ("Gepetto Viewer plugin", mainWindow, flags)
        self.client = Client()
        # Initialize the widget
        self.tabWidget = QtGui.QTabWidget(self)
        self.setWidget (self.tabWidget)
        self.nodeCreator = _NodeCreator(self, self.client)
        self.tabWidget.addTab (self.nodeCreator, "Node Creator")
        mainWindow.connect('refresh()', self.refresh)

    ### If present, this function is called when a new OSG Widget is created.
    def osgWidget(self, osgWindow):
        osgWindow.connect('selected(QString)', self.selected)

    def refresh(self):
        self.nodeCreator.update()

    def selected(self, name):
        QtGui.QMessageBox.information(self, "Selected object", name)
