from matplotlib.figure import Figure
import matplotlib.pyplot as plt
from pythonqt.matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg, NavigationToolbar2QT

from PythonQt import QtGui, Qt
from gepetto.corbaserver import Client

import numpy as np

### This class represents one special tab of the new QDockWidget
class MatplotlibWidget (QtGui.QWidget):
    def __init__(self, parent, withToolbar = False):
        super(MatplotlibWidget, self).__init__ (parent)

        box = QtGui.QVBoxLayout(self)
        # Create a figure
        self.figure = Figure(figsize=(5,5), dpi=96)
        # self.figure = Figure()
        self.canvas = FigureCanvasQTAgg (self.figure)
        box.addWidget(self.canvas)

        if withToolbar:
            self.toolbar = NavigationToolbar2QT(self.canvas, parent, False)
            box.addWidget(self.toolbar)    
