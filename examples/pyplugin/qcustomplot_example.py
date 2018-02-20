from PythonQt import QtGui, Qt
from PythonQt.QCustomPlot import QCustomPlot

pens = (
    Qt.QPen (Qt.Qt.black),
    Qt.QPen (Qt.Qt.red),
    Qt.QPen (Qt.Qt.green),
    Qt.QPen (Qt.Qt.blue),
    Qt.QPen (Qt.Qt.magenta),
    Qt.QPen (Qt.Qt.yellow),
    Qt.QPen (Qt.Qt.cyan),
    Qt.QPen (Qt.Qt.gray),
    )

class Plot (QtGui.QWidget):
  def __init__ (self, plugin):
    super(Plot, self).__init__ (self)
    self.plugin = plugin

    self.qcp = QCustomPlot ()
    layout = QtGui.QVBoxLayout (self)
    layout.addWidget (self.qcp)
    self.qcp.setInteraction(1, True) # iRangeDrag
    self.qcp.setInteraction(2, True) # iRangeZoom
    self.qcp.setAutoAddPlottableToLegend(True)
    self.qcp.legend().setVisible(True)

    # The timer user refresh the interface
    self.timer = Qt.QTimer(self)
    self.timer.setSingleShot(False)
    self.timer.setInterval(100)
    self.timer.connect(Qt.SIGNAL("timeout()"), self._step)

  def makeCurves(self):
    self.qcp.clearGraphs()
    self.legends = [ "sin", "cos" ]
    self.times = []
    for i,l in enumerate(self.legends):
      graph = self.qcp.addGraph()
      graph.setName(l)
      graph.setPen(pens[i])

  def start(self):
    self.makeCurves()
    self.timer.start()

  def stop (self):
    self.timer.stop()

  def _step(self):
    t = (self.times[-1] + 1) if len(self.times) > 0 else 0
    from math import sin, cos
    x = sin(0.1 * t)
    y = cos(0.1 * t)
    self.qcp.addData(0,(t,),(x,))
    self.qcp.addData(1,(t,),(y,))
    self.times.append(t)
    self.zoomFitBest()

  def zoomFitBest (self):
    self.qcp.rescaleAxes()
    self.qcp.replot()

class Plugin(QtGui.QDockWidget):
  def __init__ (self, mainWindow):
    super(Plugin, self).__init__ ("Gepetto Viewer plugin", mainWindow)
    self.setObjectName("qcustomplot_widget_example")
    self.plot = Plot(self)
    self.setWidget (self.plot)

    toolBar = QtGui.QToolBar ("QCustomPlot buttons")
    toolBar.addAction(QtGui.QIcon.fromTheme("zoom-fit-best"), "Zoom fit best", self.plot.zoomFitBest)
    toolBar.addAction(QtGui.QIcon.fromTheme("media-playback-start"), "Start", self.plot.start)
    toolBar.addAction(QtGui.QIcon.fromTheme("media-playback-stop"), "Stop", self.plot.stop)
    mainWindow.addToolBar (toolBar)
