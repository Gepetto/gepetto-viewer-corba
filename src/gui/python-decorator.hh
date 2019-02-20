// Copyright (c) 2015-2018, LAAS-CNRS
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
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Lesser Public License for more details. You should have
// received a copy of the GNU Lesser General Public License along with
// gepetto-viewer-corba. If not, see <http://www.gnu.org/licenses/>.

#ifndef GEPETTO_GUI_PYTHON_DECORATOR_HH
#define GEPETTO_GUI_PYTHON_DECORATOR_HH

#include <PythonQt.h>

#include <gepetto/gui/mainwindow.hh>
#include <gepetto/gui/osgwidget.hh>

namespace gepetto {
  namespace gui {
    class PythonDecorator : public QObject
    {
      Q_OBJECT

      public slots:
      /// \name MainWindow
      /// \{

      MainWindow* static_MainWindow_instance () const { return MainWindow::instance (); }

      QList <OSGWidget*> osgWindows (MainWindow* w) const { return w->osgWindows(); }

      WindowsManager* osg (MainWindow* w) const { return w->osg().get(); }

      QObject* getFromSlot (MainWindow* w, const char* slot) const { return w->getFromSlot(slot); }

      OSGWidget* createView (MainWindow* w, const QString& name) const { return w->createView(name.toStdString()); }

      /// \}

      /// \name OSGWidget
      /// \{

      int windowID (OSGWidget* o) const { return o->windowID(); }

      /// \}
    };
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_PYTHON_DECORATOR_HH
