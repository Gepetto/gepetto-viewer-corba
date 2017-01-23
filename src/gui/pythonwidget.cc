#include "gepetto/gui/pythonwidget.hh"

#include <QFileDialog>
#include <PythonQt/PythonQtClassInfo.h>
#include <PythonQt/PythonQt_QtBindings.h>

#include "gepetto/gui/osgwidget.hh"
#include "gepetto/gui/mainwindow.hh"

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
            mainContext_ = PythonQt::self()->getMainModule();
            PythonQtObjectPtr sys = PythonQt::self()->importModule ("sys");
            sys.evalScript ("argv = ['gepetto-gui']");
            console_ = new PythonQtScriptingConsole(NULL, mainContext_);
            PythonQt::self()->registerQObjectClassNames(QStringList()
                << "BodyTreeWidget"
                << "BodyTreeItem"
                << "SelectionEvent"
                << "WindowsManager");
            mainContext_.addObject("mainWindow", MainWindow::instance());
            mainContext_.addObject("windowsManager", MainWindow::instance()->osg().get());
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

                mainContext_.evalFile(file.at(0));
            }
            fd->close();
            fd->deleteLater();
        }

        void PythonWidget::loadModulePlugin(QString moduleName)
        {
          MainWindow* main = MainWindow::instance();
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

        void PythonWidget::addToContext(QString const& name, QObject* obj) {
            mainContext_.addObject(name, obj);
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
    }
}
