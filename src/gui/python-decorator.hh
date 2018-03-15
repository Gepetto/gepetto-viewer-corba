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
