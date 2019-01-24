from PythonQt import QtGui, Qt
from gepetto.corbaserver import Client
import numpy as np

from gepetto.gui.matplotlibwidget import MatplotlibWidget

class Plugin(QtGui.QDockWidget):
    def __init__ (self, mainWindow, flags = None):
        if flags is None:
            super(Plugin, self).__init__ ("Matplotlib example plugin", mainWindow)
        else:
            super(Plugin, self).__init__ ("Matplotlib example plugin", mainWindow, flags)
        self.setObjectName("Matplotlib example plugin")
        self.client = Client()

        # This avoids having a widget bigger than what it needs. It avoids having
        # a big dock widget and a small osg widget when creating the main osg widget.
        p = Qt.QSizePolicy.Ignored
        self.testWidget = MatplotlibWidget(self, True)
        self.testWidget.setSizePolicy(Qt.QSizePolicy(p,p))
        self.setWidget (self.testWidget)

        # Plot something
        x = np.linspace (0, 10, num=100)
        y = np.sin(x)
        self.testWidget.figure.gca().plot (x, y)

    ### If present, this function is called when a new OSG Widget is created.
    def osgWidget(self, osgWindow):
        pass

    def resetConnection(self):
        self.client = Client()
