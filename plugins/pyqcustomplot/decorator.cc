// Copyright (c) 2018, Joseph Mirabel
// Authors: Joseph Mirabel (joseph.mirabel@laas.fr)
//
// This file is part of gepetto-viewer-corba.
// gepetto-viewer-corba is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// gepetto-viewer-corba is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// gepetto-viewer-corba. If not, see <http://www.gnu.org/licenses/>.

#include <decorator.hh>

#include <PythonQt.h>

void registerQCustomPlot ()
{
  PythonQt::self()->addDecorators (new QCustomPlotDecorator ());
  PythonQt::self()->registerCPPClass ("QCPPainter"           , "QPainter", "QCustomPlot");
  PythonQt::self()->registerCPPClass ("QCustomPlot"          , "QWidget" , "QCustomPlot");
  // PythonQt::self()->registerCPPClass ("QCPLayoutElement"     , "QGraphicsScene", "QCustomPlot");
  // PythonQt::self()->registerCPPClass ("QCPLayout"            , "QGraphicsItem" , "QCustomPlot");
  // PythonQt::self()->registerCPPClass ("QCPAxis"              , "QGraphicsItem" , "QCustomPlot");
  // PythonQt::self()->registerCPPClass ("QCPAxisRect"          , "QGraphicsItem" , "QCustomPlot");
  // PythonQt::self()->registerCPPClass ("QCPAxisPainterPrivate", "QGraphicsItem" , "QCustomPlot");
  PythonQt::self()->registerCPPClass ("QCPGraph"             , "QCPAbstractPlottable" , "QCustomPlot");
  // PythonQt::self()->registerCPPClass ("QCPItemPosition"      , "QGraphicsItem" , "QCustomPlot");
  PythonQt::self()->registerCPPClass ("QCPLayer"             , "QObject" , "QCustomPlot");
  // PythonQt::self()->registerCPPClass ("QCPPlotTitle"         , "QGraphicsItem" , "QCustomPlot");
  // PythonQt::self()->registerCPPClass ("QCPAbstractLegendItem", "QGraphicsItem" , "QCustomPlot");
  // PythonQt::self()->registerCPPClass ("QCPColorMap"          , "QGraphicsItem" , "QCustomPlot");
  // PythonQt::self()->registerCPPClass ("QCPColorScale"        , "QGraphicsItem" , "QCustomPlot");
  // PythonQt::self()->registerCPPClass ("QCPBars"              , "QGraphicsItem" , "QCustomPlot");

  // Children of QCPLayerable
  PythonQt::self()->registerCPPClass ("QCPLayerable"         , "QObject" , "QCustomPlot");

  PythonQt::self()->registerCPPClass ("QCPAbstractItem"      , "QCPLayerable" , "QCustomPlot");
  PythonQt::self()->registerCPPClass ("QCPItemEllipse"       , "QCPAbstractItem" , "QCustomPlot");

  PythonQt::self()->registerCPPClass ("QCPAbstractPlottable" , "QCPLayerable" , "QCustomPlot");

  PythonQt::self()->registerCPPClass ("QCPLayoutElement"     , "QCPLayerable" , "QCustomPlot");
  PythonQt::self()->registerCPPClass ("QCPLayout"            , "QCPLayoutElement" , "QCustomPlot");
  PythonQt::self()->registerCPPClass ("QCPLayoutGrid"        , "QCPLayout" , "QCustomPlot");
  PythonQt::self()->registerCPPClass ("QCPLegend"            , "QCPLayoutGrid" , "QCustomPlot");
}
