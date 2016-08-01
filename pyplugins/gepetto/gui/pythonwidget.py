from PythonQt import QtGui, Qt
from gepetto.corbaserver import Client

### This class represents one special tab of the new QDockWidget
class _NodeCreator (QtGui.QWidget):
    def __init__(self, parent, plugin):
        super(_NodeCreator, self).__init__ (parent)
        self.plugin = plugin
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

        # Add box
        box.addWidget(self.bindFunctionToButton("Add box", self.addBox))

        self.update()

    def update(self):
        self.groupNodes.clear()
        for n in self.plugin.client.gui.getSceneList():
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
        self.plugin.client.gui.addMesh(str(self.nodeName.text), str(filename))
        self.refreshBodyTree()

    def addBox (self):
        self.plugin.client.gui.addBox(str(self.nodeName.text), 1, 1, 1, [1, 0, 0, 1])
        self.refreshBodyTree()

    def createGroup (self):
        self.plugin.client.gui.createGroup(str(self.nodeName.text))
        self.groupNodes.addItem(self.nodeName.text)
        self.refreshBodyTree()

    def addToGroup (self):
        self.plugin.client.gui.addToGroup(str(self.nodeName.text), str(self.groupNodes.currentText))
        self.refreshBodyTree()

    ## See gepetto::gui::MainWindow::requestRefresh for more information
    def refreshBodyTree(self):
        self.plugin.main.requestRefresh()

class Plugin(QtGui.QDockWidget):
    """
    Example of plugin of the Gepetto Viewer GUI. This can interact with
    PythonWidget C++ class.
    """
    def __init__ (self, mainWindow, flags = None):
        if flags is None:
            super(Plugin, self).__init__ ("Gepetto Viewer plugin", mainWindow)
        else:
            super(Plugin, self).__init__ ("Gepetto Viewer plugin", mainWindow, flags)
        self.client = Client()
        # Initialize the widget
        self.tabWidget = QtGui.QTabWidget(self)
        self.setWidget (self.tabWidget)
        self.nodeCreator = _NodeCreator(self, self)
        self.tabWidget.addTab (self.nodeCreator, "Node Creator")
        self.main = mainWindow
        mainWindow.connect('refresh()', self.refresh)

    ### If present, this function is called when a new OSG Widget is created.
    def osgWidget(self, osgWindow):
        osgWindow.connect('clicked(QString,QVector3D)', self.selected)

    def resetConnection(self):
        self.client = Client()

    def refresh(self):
        self.nodeCreator.update()

    def selected(self, name, posInWorldFrame):
        QtGui.QMessageBox.information(self, "Selected object", name + " " + str(posInWorldFrame))
