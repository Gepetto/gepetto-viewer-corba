#ifndef GEPETTO_GUI_PYTHON_DECORATOR_HH
#define GEPETTO_GUI_PYTHON_DECORATOR_HH

#include <PythonQt/PythonQt.h>

#include <gepetto/gui/mainwindow.hh>
#include <gepetto/gui/osgwidget.hh>

namespace gepetto {
  namespace gui {
    class PythonDecorator : public QObject
    {
      Q_OBJECT

      public slots:
      MainWindow* static_MainWindow_instance () const { return MainWindow::instance (); }
    };
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_PYTHON_DECORATOR_HH
