from matplotlib.figure import Figure
from pythonqt.matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg, NavigationToolbar2QT
from PythonQt.QtGui import QWidget, QVBoxLayout

### Python QWidget that allows to draw matplotlib figures within the interface.
###
### Create an instance and populate member MatplotlibWidget.figure.
class MatplotlibWidget (QWidget):
    def __init__(self, parent, withToolbar = False):
        super(MatplotlibWidget, self).__init__ (parent)

        box = QVBoxLayout(self)
        ## The matplotlib figure
        self.figure = Figure(figsize=(5,5), dpi=96)
        ## The figure canvas
        self.canvas = FigureCanvasQTAgg (self.figure)
        box.addWidget(self.canvas)

        if withToolbar:
            self.toolbar = NavigationToolbar2QT(self.canvas, parent, False)
            box.addWidget(self.toolbar)    
