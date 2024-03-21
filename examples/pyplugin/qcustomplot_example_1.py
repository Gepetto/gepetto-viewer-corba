from PythonQt import QtGui
from PythonQt.QCustomPlot import QCP, QCPItemRichText, QCPItemTracer, QCustomPlot


class Plugin(QtGui.QDockWidget):
    def __init__(self, mainWindow, flags=None):
        if flags is None:
            super().__init__("QCP example plugin", mainWindow)
        else:
            super().__init__("QCP example plugin", mainWindow, flags)
        self.setObjectName("QCP example plugin")

        self.main = mainWindow
        self.qcp = QCustomPlot()
        self.qcp.setInteraction(QCP.iRangeDrag, True)
        self.qcp.setInteraction(QCP.iRangeZoom, True)
        self.setWidget(self.qcp)

        self.graph = self.qcp.addGraph()
        self.graph.setData([0, 1, 2, 3], [0.1, 0.3, 0.2, 0])
        self.qcp.replot()

        self.tracer = QCPItemTracer(self.qcp)
        self.tracer.graph = self.graph
        self.tracer.graphKey = 0
        self.tracer.style = 3

        self.text = QCPItemRichText(self.qcp)
        self.text.text = "q<sub>0,0</sub>"
        self.text.position("position").setParentAnchor(self.tracer.anchor("position"))
        self.text.position("position").setCoords(0, -20)

        self.graph.setData([0, 1, 2, 3], [1, 0.3, 0.2, 0])
