#include <qcustomplot.h>

#include <QObject>

void registerQCustomPlot ();

class QCustomPlotDecorator : public QObject
{
  Q_OBJECT
  public Q_SLOTS:
    // add a constructor
    QCustomPlot* new_QCustomPlot(QWidget* parent = 0)
    {
      return new QCustomPlot(parent);
    }
    void delete_QCustomPlot(QCustomPlot* o) //delete QCustomPlot object
    {
      delete o;
    }
    /// \name QCustomPlot
    /// \{
    void clearGraphs(QCustomPlot* o) //add a graph inside the QCustomPlot object
    {
      o->clearGraphs();
    }
    QCPGraph* addGraph(QCustomPlot* o) //add a graph inside the QCustomPlot object
    {
      return o->addGraph();
    }
    QCPGraph* graph(QCustomPlot* o, int graphnum)
    {
      return o->graph(graphnum);
    }
    void setData (QCustomPlot* o,int graphnum, const QVector<double> &keys, const QVector<double> &values) //set data to a certain graph. Deletes existing data inside the graph
    {
      o->graph(graphnum)->setData(keys,values);
    }
    void addData (QCustomPlot* o,int graphnum, const QVector<double> &keys, const QVector<double> &values) //set data to a certain graph. Deletes existing data inside the graph
    {
      o->graph(graphnum)->addData(keys,values);
    }
    void replot (QCustomPlot* o) //replot object to visualise new data
    {
      o->replot();
    }
    void show (QCustomPlot* o) //open new window with graph
    {
      o->show();
    }
    void setWindowTitle(QCustomPlot* o,QString title) //set title of window of graph
    {
      o->setWindowTitle(title);
    }
    void rescaleAxes(QCustomPlot* o, bool v = true) //rescale axis automatically if data does not fit
    {
      o->rescaleAxes(v);
    }
    void plotLayout_insertRow(QCustomPlot* o, int row) //insert row above graph
    {
      o->plotLayout()->insertRow(row);
    }
    void plotLayout_insertColumn(QCustomPlot* o, int column) //insert column above graph
    {
      o->plotLayout()->insertColumn(column);
    }
    void plotLayout_addElement(QCustomPlot* o, int row, int column, QCPLayoutElement *element) //add text to graph at row,column
    {
      o->plotLayout()->addElement(row,column,element);
    }
    void setAutoAddPlottableToLegend (QCustomPlot* o, bool v)
    {
      o->setAutoAddPlottableToLegend (v);
    }
    QCPLegend* legend (QCustomPlot* o)
    {
      return o->legend;
    }
    void setInteraction(QCustomPlot* o, int interaction, bool enabled = true)
    {
      o->setInteraction((QCP::Interaction)interaction, enabled);
    }
    QCPAxis* xAxis  (QCustomPlot* o) { return o->xAxis ; }
    QCPAxis* xAxis2 (QCustomPlot* o) { return o->xAxis2; }
    QCPAxis* yAxis  (QCustomPlot* o) { return o->yAxis ; }
    QCPAxis* yAxis2 (QCustomPlot* o) { return o->yAxis2; }
    /// \}

    /// \name QCPAxis
    /// \{
    void setLabel(QCPAxis* a, const QString text)
    {
      a->setLabel(text);
    }
    void setRange(QCPAxis* a, double position, double size)
    {
      a->setRange(position, size);
    }
    void setAutoTicks(QCPAxis* a, bool on)
    {
      a->setAutoSubTicks(on);
    }
    void setAutoTickLabels(QCPAxis* a, bool on)
    {
      a->setAutoTickLabels(on);
    }
    void setTickVector(QCPAxis* a, const QVector<double> &ticks)
    {
      a->setTickVector(ticks);
    }
    void setTickVectorLabels(QCPAxis* a, const QVector<QString> &labels)
    {
      a->setTickVectorLabels(labels);
    }
    void setTickLength(QCPAxis* a, int inside, int outside)
    {
      a->setTickLength(inside,outside);
    }
    void setSubTickLength(QCPAxis* a, int inside, int outside)
    {
      a->setSubTickLength(inside, outside);
    }
    double pixelToCoord(QCPAxis* a, double pixel)
    {
      return a->pixelToCoord(pixel);
    }
    /// \}

    /// \name QCPGraph
    /// \{
    void rescaleAxes(QCPGraph* g, bool v = true)
    {
      g->rescaleAxes(v);
    }
    void setData (QCPGraph* g, const QVector<double> &keys, const QVector<double> &values)
    {
      g->setData(keys,values);
    }
    void addData (QCPGraph* g, const QVector<double> &keys, const QVector<double> &values)
    {
      g->addData(keys,values);
    }
    void setPen (QCPGraph* g, const QPen &pen)
    {
      g->setPen(pen);
    }
    /// \}

    /// \name QCPAbstractPlottable
    /// \{
    void setName (QCPAbstractPlottable* ap, const QString &n)
    {
      ap->setName(n);
    }
    /// \}

    /// \name QCPLayerable
    /// \{
    void setVisible (QCPLayerable* l, const bool &v)
    {
      l->setVisible(v);
    }
    /// \}
};
