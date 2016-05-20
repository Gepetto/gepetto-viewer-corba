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
      /// \code[py]
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
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_PYTHONWIDGET_HH
