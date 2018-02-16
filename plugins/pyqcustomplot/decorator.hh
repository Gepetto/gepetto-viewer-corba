#include <qcustomplot.h>

#include <QObject>

void registerQCustomPlot ();

class QCustomPlotDecorator : public QObject
{
  Q_OBJECT
  public Q_SLOTS:
    // add a constructor
    QCustomPlot* new_QCustomPlot()
    {
      return new QCustomPlot();
    }
    //wrapper functions
    void delete_QCustomPlot(QCustomPlot* o) //delete QCustomPlot object
    {
      delete o;
    }
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
    //------------------------------------------------------- Axis adjustment stuff------------------------------------------------
    //xAxis stuff
    void xAxis_setLabel(QCustomPlot* o, const QString text)
    {
      o->xAxis->setLabel(text);
    }
    void xAxis_setVisible(QCustomPlot* o, bool visible)
    {
      o->setVisible(visible);
    }
    void xAxis_setRange(QCustomPlot* o, double position, double size)
    {
      o->xAxis->setRange(position, size);
    }
    void xAxis_setAutoTicks(QCustomPlot* o, bool on)
    {
      o->xAxis->setAutoSubTicks(on);
    }
    void xAxis_setAutoTickLabels(QCustomPlot* o, bool on)
    {
      o->xAxis->setAutoTickLabels(on);
    }
    void xAxis_setTickVector(QCustomPlot* o, const QVector<double> &ticks)
    {
      o->xAxis->setTickVector(ticks);
    }
    void xAxis_setTickVectorLabels(QCustomPlot* o, const QVector<QString> &labels)
    {
      o->xAxis->setTickVectorLabels(labels);
    }
    void xAxis_setTickLength(QCustomPlot* o, int inside, int outside)
    {
      o->xAxis->setTickLength(inside,outside);
    }
    void xAxis_setSubTickLength(QCustomPlot* o, int inside, int outside)
    {
      o->xAxis->setSubTickLength(inside, outside);
    }
    //xAxis2 stuff
    void xAxis2_setLabel(QCustomPlot* o, const QString text)
    {
      o->xAxis2->setLabel(text);
    }
    void xAxis2_setVisible(QCustomPlot* o, bool visible)
    {
      o->setVisible(visible);
    }
    void xAxis2_setRange(QCustomPlot* o, double position, double size)
    {
      o->xAxis2->setRange(position, size);
    }
    void xAxis2_setAutoTicks(QCustomPlot* o, bool on)
    {
      o->xAxis2->setAutoSubTicks(on);
    }
    void xAxis2_setAutoTickLabels(QCustomPlot* o, bool on)
    {
      o->xAxis2->setAutoTickLabels(on);
    }
    void xAxis2_setTickVector(QCustomPlot* o, const QVector<double> &ticks)
    {
      o->xAxis2->setTickVector(ticks);
    }
    void xAxis2_setTickVectorLabels(QCustomPlot* o, const QVector<QString> &labels)
    {
      o->xAxis2->setTickVectorLabels(labels);
    }
    void xAxis2_setTickLength(QCustomPlot* o, int inside, int outside)
    {
      o->xAxis2->setTickLength(inside,outside);
    }
    void xAxis2_setSubTickLength(QCustomPlot* o, int inside, int outside)
    {
      o->xAxis2->setSubTickLength(inside, outside);
    }
    //yAxis stuff
    void yAxis_setLabel(QCustomPlot* o, const QString text)
    {
      o->yAxis->setLabel(text);
    }
    void yAxis_setVisible(QCustomPlot* o, bool visible)
    {
      o->setVisible(visible);
    }
    void yAxis_setRange(QCustomPlot* o, double position, double size)
    {
      o->yAxis->setRange(position, size);
    }
    void yAxis_setAutoTicks(QCustomPlot* o, bool on)
    {
      o->yAxis->setAutoSubTicks(on);
    }
    void yAxis_setAutoTickLabels(QCustomPlot* o, bool on)
    {
      o->yAxis->setAutoTickLabels(on);
    }
    void yAxis_setTickVector(QCustomPlot* o, const QVector<double> &ticks)
    {
      o->yAxis->setTickVector(ticks);
    }
    void yAxis_setTickVectorLabels(QCustomPlot* o, const QVector<QString> &labels)
    {
      o->yAxis->setTickVectorLabels(labels);
    }
    void yAxis_setTickLength(QCustomPlot* o, int inside, int outside)
    {
      o->yAxis->setTickLength(inside,outside);
    }
    void yAxis_setSubTickLength(QCustomPlot* o, int inside, int outside)
    {
      o->yAxis->setSubTickLength(inside, outside);
    }
    //yAxis2 stuff
    void yAxis2_setLabel(QCustomPlot* o, const QString text)
    {
      o->yAxis2->setLabel(text);
    }
    void yAxis2_setVisible(QCustomPlot* o, bool visible)
    {
      o->setVisible(visible);
    }
    void yAxis2_setRange(QCustomPlot* o, double position, double size)
    {
      o->yAxis2->setRange(position, size);
    }
    void yAxis2_setAutoTicks(QCustomPlot* o, bool on)
    {
      o->yAxis2->setAutoSubTicks(on);
    }
    void yAxis2_setAutoTickLabels(QCustomPlot* o, bool on)
    {
      o->yAxis2->setAutoTickLabels(on);
    }
    void yAxis2_setTickVector(QCustomPlot* o, const QVector<double> &ticks)
    {
      o->yAxis2->setTickVector(ticks);
    }
    void yAxis2_setTickVectorLabels(QCustomPlot* o, const QVector<QString> &labels)
    {
      o->yAxis2->setTickVectorLabels(labels);
    }
    void yAxis2_setTickLength(QCustomPlot* o, int inside, int outside)
    {
      o->yAxis2->setTickLength(inside,outside);
    }
    void yAxis2_setSubTickLength(QCustomPlot* o, int inside, int outside)
    {
      o->yAxis2->setSubTickLength(inside, outside);
    }
    //Interaction
    void setInteraction(QCustomPlot* o, int interaction, bool enabled = true)
    {
      o->setInteraction((QCP::Interaction)interaction, enabled);
    }
    //QCPGraph
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
    //QCPAbstractPlottable
    void setName (QCPAbstractPlottable* ap, const QString &n)
    {
      ap->setName(n);
    }
    //QCPLayerable
    void setVisible (QCPLayerable* l, const bool &v)
    {
      l->setVisible(v);
    }
};
