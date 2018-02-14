#include "gepetto/gui/pythonwidget.hh"

#include <QAction>
#include <QFileDialog>
#include <PythonQt/PythonQt.h>
#include <PythonQt/PythonQt_QtAll.h>
#include <PythonQt/gui/PythonQtScriptingConsole.h>
#include <PythonQt/PythonQtClassInfo.h>
#include <PythonQt/PythonQt_QtBindings.h>

#include "gepetto/gui/osgwidget.hh"
#include "gepetto/gui/mainwindow.hh"
#include "gepetto/gui/plugin-interface.hh"

namespace gepetto {
    namespace gui {
      namespace {
        void addSignalHandler (PythonQtObjectPtr obj, const QString& callable,
            QObject* sender, const char* signal) {
          PythonQt* pqt = PythonQt::self();
          PythonQtObjectPtr call = pqt->lookupCallable(obj, callable);
          if (call.isNull()) {
            qDebug() << "Callable" << callable << "not found.";
            return;
          }
          if (!pqt->addSignalHandler(sender, signal, call)) {
            qDebug() << "Signal" << signal << "not found in object"
              << sender->objectName();
          } else {
            qDebug() << "Connected"
              << signal << "of" << sender->objectName()
              << "to" << callable;
          }
        }
      }

      PythonWidget::PythonWidget(QWidget *parent) :
        QDockWidget("&PythonQt console", parent)
      {
        PythonQt::init(PythonQt::RedirectStdOut);
        PythonQt_init_QtBindings();
        PythonQtObjectPtr mainContext = PythonQt::self()->getMainModule();
        PythonQtObjectPtr sys = PythonQt::self()->importModule ("sys");
        sys.evalScript ("argv = ['gepetto-gui']");
        console_ = new PythonQtScriptingConsole(NULL, mainContext);
        PythonQt::self()->registerQObjectClassNames(QStringList()
            << "BodyTreeWidget"
            << "BodyTreeItem"
            << "SelectionEvent"
            << "WindowsManager");
        mainContext.addObject("mainWindow", MainWindow::instance());
        mainContext.addObject("windowsManager", MainWindow::instance()->osg().get());
        console_->QTextEdit::clear();
        console_->consoleMessage(
            "PythonQt command prompt\n"
            "Use Shift+Enter for multiline code.\n"
            "Variable mainWindow and osg are available.\n"
            );
        console_->appendCommandPrompt();

        QWidget* widget = new QWidget;
        QVBoxLayout* layout = new QVBoxLayout;
        button_ = new QPushButton;

        button_->setText("Choose file");
        layout->addWidget(console_);
        layout->addWidget(button_);
        widget->setLayout(layout);
        this->setWidget(widget);

        toggleViewAction()->setShortcut(gepetto::gui::DockKeyShortcutBase + Qt::Key_A);
        connect(button_, SIGNAL(clicked()), SLOT(browseFile()));
      }

      PythonWidget::~PythonWidget()
      {
        foreach (const PythonQtObjectPtr& m, modules_)
          unloadModulePlugin(m);
	PythonQt::cleanup();
      }

      void PythonWidget::browseFile()
      {
        QFileDialog* fd = new QFileDialog;

        fd->setFileMode(QFileDialog::ExistingFile);
        fd->setNameFilter("All python file (*.py)");
        if (fd->exec() == QDialog::Accepted) {
          QStringList file = fd->selectedFiles();

          PythonQtObjectPtr mainContext = PythonQt::self()->getMainModule();
          mainContext.evalFile(file.at(0));
        }
        fd->close();
        fd->deleteLater();
      }

      void PythonWidget::loadScriptPlugin(QString moduleName, QString fileName)
      {
        PythonQt* pqt = PythonQt::self();
        PythonQtObjectPtr module = pqt->createModuleFromFile (moduleName, fileName);
        if (pqt->handleError()) {
          return;
        }
        if (module.isNull()) {
          pqt->handleError();
          qDebug() << "Enable to load module" << moduleName << "from script"
            << fileName;
          return;
        }
        loadPlugin (moduleName, module);
      }

      void PythonWidget::loadModulePlugin(QString moduleName)
      {
        PythonQt* pqt = PythonQt::self();
        PythonQtObjectPtr module = pqt->importModule (moduleName);
        if (pqt->handleError()) {
          return;
        }
        if (module.isNull()) {
          pqt->handleError();
          qDebug() << "Enable to load module" << moduleName;
          return;
        }
        loadPlugin (moduleName, module);
      }

      void PythonWidget::loadPlugin(QString moduleName, PythonQtObjectPtr module)
      {
        PythonQt* pqt = PythonQt::self();
        MainWindow* main = MainWindow::instance();
        module.addObject("windowsManager", main->osg().get());

        QString var = "pluginInstance";
        QVariantList args; args << QVariant::fromValue((QObject*)main);
        QVariant instance = module.call("Plugin", args);
        module.addVariable(var, instance);

        QDockWidget* dw = qobject_cast<QDockWidget*>(instance.value<QObject*>());
        if (dw) main->insertDockWidget(dw, Qt::RightDockWidgetArea);
        // PythonQtObjectPtr dockPyObj (instance);
        PythonQtObjectPtr dockPyObj = pqt->lookupObject(module,var);
        addSignalHandlersToPlugin(dockPyObj);
        modules_[moduleName] = module;
      }

      void PythonWidget::unloadModulePlugin(QString moduleName)
      {
        if (modules_.contains(moduleName)) {
          PythonQtObjectPtr module = modules_.value(moduleName);
          unloadModulePlugin(module);
          modules_.remove(moduleName);
        }
      }

      void PythonWidget::unloadModulePlugin(PythonQtObjectPtr module )
      {
        PythonQt* pqt = PythonQt::self();
        QString var = "pluginInstance";
        QVariant instance = pqt->getVariable(module, var);
        QDockWidget* dw = qobject_cast<QDockWidget*>(instance.value<QObject*>());
        if (dw) MainWindow::instance()->removeDockWidget(dw);
        module.removeVariable (var);
      }

      void PythonWidget::addToContext(QString const& name, QObject* obj)
      {
        PythonQtObjectPtr mainContext = PythonQt::self()->getMainModule();
        mainContext.addObject(name, obj);
      }

      void PythonWidget::addSignalHandlersToPlugin(PythonQtObjectPtr plugin)
      {
        MainWindow* main = MainWindow::instance();
        addSignalHandler(plugin, "osgWidget",
            main, SIGNAL(viewCreated(OSGWidget*)));
        QAction* reconnect = main->findChild<QAction*>("actionReconnect");
        if (reconnect)
          addSignalHandler(plugin, "resetConnection",
              reconnect, SIGNAL(triggered()));
        else
          qDebug() << "Could not find actionReconnect button. The plugin will"
            << "not be able to reset CORBA connections";
        QAction* refresh = main->findChild<QAction*>("actionRefresh");
        if (refresh)
          addSignalHandler(plugin, "refreshInterface",
              refresh, SIGNAL(triggered()));
        else
          qDebug() << "Could not find actionRefresh button. The plugin will"
            << "not be able to refresh interface.";
      }

      QVariantList PythonWidget::callPluginMethod (const QString& method,
          const QVariantList& args,
          const QVariantMap& kwargs) const
      {
        PythonQt* pqt = PythonQt::self();
        QString var = "pluginInstance";
        QVariantList ret;
        foreach (const PythonQtObjectPtr& m, modules_)
        {
          PythonQtObjectPtr dockPyObj = pqt->lookupObject(m,var);
          PythonQtObjectPtr call = pqt->lookupCallable(dockPyObj, method);
          if (!call.isNull()) {
            ret << call.call(args, kwargs);
          }
        }
        return ret;
      }
    }
}
