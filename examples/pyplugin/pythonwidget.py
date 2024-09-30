from PythonQt import Qt, QtGui

from gepetto.corbaserver import Client


class _NodeCreator(QtGui.QWidget):
    """This class represents one special tab of the new QDockWidget."""

    def __init__(self, parent, plugin):
        super().__init__(parent)
        self.plugin = plugin
        box = QtGui.QVBoxLayout(self)

        # Name line edit
        self.nodeName = QtGui.QLineEdit("nodeName")
        box.addWidget(
            self.addWidgetsInHBox([QtGui.QLabel("Node name:"), self.nodeName])
        )

        # Create group
        box.addWidget(self.bindFunctionToButton("Create group", self.createGroup))

        # Add to group
        self.groupNodes = QtGui.QComboBox(self)
        self.groupNodes.editable = False
        box.addWidget(
            self.addWidgetsInHBox(
                [
                    self.groupNodes,
                    self.bindFunctionToButton("Add to group", self.addToGroup),
                ]
            )
        )

        # Add mesh
        box.addWidget(self.bindFunctionToButton("Add mesh", self.addMesh))

        # Add box
        box.addWidget(self.bindFunctionToButton("Add box", self.addBox))

        box.addWidget(self.bindFunctionToButton("Create window", self.createWindow))

        self.update()

    def update(self):
        self.groupNodes.clear()
        for n in self.plugin.client.gui.getSceneList():
            self.groupNodes.addItem(n)

    def addWidgetsInHBox(self, widgets):
        nameParentW = QtGui.QWidget(self)
        hboxName = QtGui.QHBoxLayout(nameParentW)
        for w in widgets:
            hboxName.addWidget(w)
        return nameParentW

    def bindFunctionToButton(self, buttonLabel, func):
        button = QtGui.QPushButton(self)
        button.text = buttonLabel
        button.connect("clicked()", func)
        return button

    def addMesh(self):
        filename = QtGui.QFileDialog.getOpenFileName(self, "Choose a mesh")
        self.plugin.client.gui.addMesh(str(self.nodeName.text), str(filename))
        self.refreshBodyTree()

    def addBox(self):
        self.plugin.client.gui.addBox(str(self.nodeName.text), 1, 1, 1, [1, 0, 0, 1])
        self.refreshBodyTree()

    def createWindow(self):
        self.plugin.main.createView(str(self.nodeName.text))

    def createGroup(self):
        self.plugin.client.gui.createGroup(str(self.nodeName.text))
        self.groupNodes.addItem(self.nodeName.text)
        self.refreshBodyTree()

    def addToGroup(self):
        self.plugin.client.gui.addToGroup(
            str(self.nodeName.text), str(self.groupNodes.currentText)
        )
        self.refreshBodyTree()

    # See gepetto::gui::MainWindow::requestRefresh for more information
    def refreshBodyTree(self):
        self.plugin.main.requestRefresh()


class Plugin(QtGui.QDockWidget):
    """
    Example of plugin of the Gepetto Viewer GUI. This can interact with
    PythonWidget C++ class.
    """

    def __init__(self, mainWindow):
        super().__init__("Gepetto Viewer plugin", mainWindow)
        self.setObjectName("Gepetto Viewer plugin")
        self.client = Client()
        # Initialize the widget
        self.tabWidget = QtGui.QTabWidget(self)
        # This avoids having a widget bigger than what it needs. It avoids having
        # a big dock widget and a small osg widget when creating the main osg widget.
        p = Qt.QSizePolicy.Maximum
        self.tabWidget.setSizePolicy(Qt.QSizePolicy(p, p))
        self.setWidget(self.tabWidget)
        self.nodeCreator = _NodeCreator(self, self)
        self.tabWidget.addTab(self.nodeCreator, "Node Creator")
        self.main = mainWindow
        mainWindow.connect("refresh()", self.refresh)
        mainWindow.bodyTree().connect("bodySelected(SelectionEvent*)", self.selected)

    # If present, this function is called when a new OSG Widget is created.
    def osgWidget(self, osgWindow):
        pass

    def resetConnection(self):
        self.client = Client()

    def refresh(self):
        self.nodeCreator.update()

    def selected(self, event):
        # QtGui.QMessageBox.information(self, "Selected object", event.nodeName()
        #                               + " " + str(event.point(False)))
        print("name: ", event.nodeName())
        if event.hasIntersection():
            print("point LF: ", event.point(True))
            print("point WF: ", event.point(False))
            print("normal LF: ", event.normal(True))
            print("normal WF: ", event.normal(False))
