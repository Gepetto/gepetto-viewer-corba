#ifndef GEPETTO_GUI_PYTHONWIDGET_HH
#define GEPETTO_GUI_PYTHONWIDGET_HH

#include <gepetto/gui/fwd.hh>
#include <gepetto/gui/config-dep.hh>

#if ! GEPETTO_GUI_HAS_PYTHONQT
# error "hpp-gui was not compile with PythonQt dependency."
#endif

#include <QDockWidget>
#include <QLayout>
#include <QPushButton>
#include <PythonQt/PythonQt.h>
#include <PythonQt/PythonQt_QtAll.h>
#include <PythonQt/PythonQtScriptingConsole.h>

namespace gepetto {
  namespace gui {
    /// \defgroup plugin Plugin interfaces


    /// \ingroup plugin
    /// Python plugin interface
    ///
    /// A Python plugin is a Python module containing a class Plugin, with a
    /// constructor taking a pointer to the MainWindow as input.
    ///
    /// For instance, if you have \code gepetto.gui=true \endcode in your
    /// configuration file, then the Plugin class will be accessed via:
    ///
    /// \code{py}
    /// from gepetto.gui import Plugin
    /// pluginInstance = Plugin(mainWindow)
    /// \endcode
    ///
    /// It may interact with the interface in two following ways.
    ///
    /// pyplugin_dockwidget Add a dock widget:
    ///
    /// Your plugin may inherits from class PythonQt.QtGui.QDockWidget.
    /// In this case, an instance of the Plugin  will be added to the MainWindow
    /// as a QDockWidget.
    ///
    /// pyplugin_signals Signals and slots:
    ///
    /// The following method will be automatically connected to Qt signals:
    /// \li MainWindow::viewCreated(OSGWidget*) -> Plugin.osgWidget
    ///
    /// \sa See example \ref pyplugins/gepetto/gui/pythonwidget.py
    class PythonWidget : public QDockWidget
    {
      Q_OBJECT
    public:
      explicit PythonWidget(QWidget *parent = 0);
      void addToContext(QString const& name, QObject *obj);
      virtual ~PythonWidget();

    public slots:
      /// Load a plugin by importing a module
      ///
      /// This is mostly equivalent to running the following code in the Python
      /// console
      /// \code{py}
      /// import modulename
      /// pluginInstance = modulename.Plugin(mainWindow)
      /// mainWindow.addDockWidget (1, pluginInstance)
      /// # Some extra code to add dock widget toggle button to the right place.
      /// \endcode
      ///
      /// \note The module must contain a class Plugin that inherits from
      /// class PythonQt.QtGui.QDockWidget
      void loadModulePlugin(QString moduleName);
      void unloadModulePlugin(QString moduleName);

    private:
      void unloadModulePlugin(PythonQtObjectPtr module);

      void addSignalHandlersToPlugin(PythonQtObjectPtr plugin);

      PythonQtObjectPtr mainContext_;
      PythonQtScriptingConsole* console_;
      QPushButton* button_;
      QMap<QString, PythonQtObjectPtr> modules_;

    signals:

    public slots:
      void browseFile();
    };

    /// \example pyplugins/gepetto/gui/pythonwidget.py
    /// This is an example Python Plugin for \link hpp::gui::PythonWidget \endlink. Two classes are defined:
    /// \b _NodeCreator and \b Plugin. Two signals are used: \a mainWindow.refresh()
    /// and \a osgWidget.
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_PYTHONWIDGET_HH
