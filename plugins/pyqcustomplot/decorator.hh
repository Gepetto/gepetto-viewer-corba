#include <qcustomplot.h>

#include <QObject>

void registerQCustomPlot ();

class QCustomPlotDecorator : public QObject
{
  Q_OBJECT
  public Q_SLOTS:
    /// \name QCustomPlot
    /// \{
    QCustomPlot* new_QCustomPlot(QWidget* parent = 0)
    {
      return new QCustomPlot(parent);
    }
    void delete_QCustomPlot(QCustomPlot* o) //delete QCustomPlot object
    {
      delete o;
    }
    void clearGraphs(QCustomPlot* o)
    {
      o->clearGraphs();
    }

    QCPGraph* addGraph(QCustomPlot* o)
    {
      return o->addGraph();
    }
    void addPlottable(QCustomPlot* o, QCPAbstractPlottable* ap)
    {
      o->addPlottable(ap);
    }

    QCPGraph* graph(QCustomPlot* o, int graphnum)
    {
      return o->graph(graphnum);
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
    QCPLayoutGrid* plotLayout (QCustomPlot* o) { return o->plotLayout (); }
    void setAutoAddPlottableToLegend (QCustomPlot* o, bool v)
    {
      o->setAutoAddPlottableToLegend (v);
    }
    /// \param interaction See QCP::Interaction
    void setInteraction(QCustomPlot* o, int interaction, bool enabled = true)
    {
      o->setInteraction((QCP::Interaction)interaction, enabled);
    }
    QCPAxis* xAxis  (QCustomPlot* o) { return o->xAxis ; }
    QCPAxis* xAxis2 (QCustomPlot* o) { return o->xAxis2; }
    QCPAxis* yAxis  (QCustomPlot* o) { return o->yAxis ; }
    QCPAxis* yAxis2 (QCustomPlot* o) { return o->yAxis2; }
    QCPLegend* legend (QCustomPlot* o) { return o->legend; }
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
    QCPGraph* new_QCPGraph (QCPAxis* key, QCPAxis* value) { return new QCPGraph (key, value); }
    void delete_QCPGraph (QCPGraph* g) { delete g; }
    void setData (QCPGraph* g, const QVector<double> &keys, const QVector<double> &values)
    {
      g->setData(keys,values);
    }
    void addData (QCPGraph* g, const QVector<double> &keys, const QVector<double> &values)
    {
      g->addData(keys,values);
    }
    void addData (QCPGraph* g, const double &key, const double &value)
    {
      g->addData(key,value);
    }
    void clearData (QCPGraph* o) { o->clearData (); }
    /// \}

    /// \name QCPCurve
    /// \{
    QCPCurve* new_QCPCurve (QCPAxis* key, QCPAxis* value) { return new QCPCurve (key, value); }
    void delete_QCPCurve (QCPCurve* g) { delete g; }
    void setData (QCPCurve* c, const QVector<double> &keys, const QVector<double> &values)
    {
      c->setData(keys,values);
    }
    void addData (QCPCurve* c, const QVector<double> &ts, const QVector<double> &keys, const QVector<double> &values)
    {
      c->addData(ts, keys,values);
    }
    void clearData (QCPCurve* o) { o->clearData (); }
    /// \}

    /// \name QCPBars
    /// \{
    QCPBars* new_QCPBars (QCPAxis* key, QCPAxis* value) { return new QCPBars (key, value); }
    void delete_QCPBars (QCPBars* g) { delete g; }
    void setData (QCPBars* c, const QVector<double> &keys, const QVector<double> &values)
    {
      c->setData(keys,values);
    }
    void addData (QCPBars* c, const QVector<double> &keys, const QVector<double> &values)
    {
      c->addData(keys,values);
    }
    void clearData (QCPBars* o) { o->clearData (); }
    /// \}

    /// \name QCPAbstractPlottable
    /// \{
    void rescaleAxes(QCPAbstractPlottable* ap, bool v = true)
    {
      ap->rescaleAxes(v);
    }
    void setName (QCPAbstractPlottable* ap, const QString &n)
    {
      ap->setName(n);
    }
    void setPen (QCPAbstractPlottable* ap, const QPen &pen)
    {
      ap->setPen(pen);
    }
    /// \}

    /// \name QCPLayerable
    /// \{
    void setVisible (QCPLayerable* l, const bool &v)
    {
      l->setVisible(v);
    }
    /// \}

    /// \name QCPLayoutGrid
    /// \{
    void insertRow (QCPLayoutGrid* lg, int row) //insert row above graph
    {
      lg->insertRow(row);
    }
    void insertColumn (QCPLayoutGrid* lg, int column) //insert column above graph
    {
      lg->insertColumn(column);
    }
    void addElement (QCPLayoutGrid* lg, int row, int column, QCPLayoutElement *element) //add text to graph at row,column
    {
      lg->addElement(row,column,element);
    }
    /// \}
};
