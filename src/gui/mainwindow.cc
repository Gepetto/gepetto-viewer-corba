#include "gepetto/gui/mainwindow.hh"
#include "ui_mainwindow.h"

#include <QtGlobal>
#include <QScrollBar>
#include <QMessageBox>

#include <gepetto/viewer/corba/server.hh>

#include "gepetto/gui/windows-manager.hh"
#include "gepetto/gui/osgwidget.hh"
#include "gepetto/gui/tree-item.hh"
#include "gepetto/gui/shortcut-factory.hh"
#include "gepetto/gui/dialog/dialogloadrobot.hh"
#include "gepetto/gui/dialog/dialogloadenvironment.hh"
#include "gepetto/gui/plugin-interface.hh"
#include "gepetto/gui/selection-handler.hh"
#include "gepetto/gui/action-search-bar.hh"
#include "gepetto/gui/node-action.hh"

#include <gepetto/gui/meta.hh>
#include <gepetto/gui/config-dep.hh>

#if GEPETTO_GUI_HAS_PYTHONQT
# include <gepetto/gui/pythonwidget.hh>
#endif

namespace gepetto {
  namespace gui {
    MainWindow* MainWindow::instance_ = NULL;

    MainWindow::MainWindow(Settings* settings, QWidget *parent) :
      QMainWindow(parent),
      settings_ (settings),
      ui_(new ::Ui::MainWindow),
      centralWidget_ (),
      osgViewerManagers_ (),
      osgServer_ (NULL),
      backgroundQueue_(),
      worker_ (),
      actionSearchBar_ (new ActionSearchBar(this))
    {
      MainWindow::instance_ = this;
      ui_->setupUi(this);

      // Setup the body tree view
      osgViewerManagers_ = WindowsManager::create(ui_->bodyTreeContent);
      ui_->bodyTreeContent->init(ui_->bodyTree, ui_->toolBox);

      if (settings_->startGepettoCorbaServer) {
        osgServer_ = new CorbaServer (new ViewerServerProcess (
              new graphics::corbaServer::Server (
                osgViewerManagers_, 0, NULL, true)));
        osgServer_->start();
      }
      // This scene contains elements required for User Interaction.
      osg()->createScene("hpp-gui");

      // Setup the main OSG widget
      connect (this, SIGNAL (createViewOnMainThread(std::string)), SLOT (createView(std::string)));

      connect (ui_->actionRefresh, SIGNAL (triggered()), SLOT (requestRefresh()));

      connect (&backgroundQueue_, SIGNAL (done(int)), this, SLOT (handleWorkerDone(int)));
      connect (&backgroundQueue_, SIGNAL (failed(int,const QString&)),
          this, SLOT (logJobFailed(int, const QString&)));
      connect (this, SIGNAL (sendToBackground(WorkItem*)),
          &backgroundQueue_, SLOT (perform(WorkItem*)));
      backgroundQueue_.moveToThread(&worker_);
      worker_.start();

      collisionLabel_ = new QLabel("No collisions.");
      shortcutFactory_ = new ShortcutFactory;
#if GEPETTO_GUI_HAS_PYTHONQT
      pythonWidget_ = new PythonWidget(this);
#endif
      setupInterface();
      connect(ui_->actionChange_shortcut, SIGNAL(triggered()), shortcutFactory_, SLOT(open()));

      selectionHandler_ = new SelectionHandler(osgViewerManagers_);
      selectionHandler_->addMode(new MultiSelection(osgViewerManagers_));
      selectionHandler_->addMode(new UniqueSelection(osgViewerManagers_));

      ui_->osgToolBar->addWidget(selectionHandler_);
    }

    MainWindow::~MainWindow()
    {
      delete shortcutFactory_;
#if GEPETTO_GUI_HAS_PYTHONQT
      removeDockWidget(pythonWidget_);
      delete pythonWidget_;
#endif
      pluginManager()->clearPlugins();
      osgViewerManagers_.reset();
      worker_.quit();
      if (osgServer_ != NULL) {
        osgServer_->wait();
        delete osgServer_;
      }
      worker_.wait();
      delete ui_;
    }

    MainWindow *MainWindow::instance()
    {
      return instance_;
    }

    void MainWindow::insertDockWidget(QDockWidget *dock, Qt::DockWidgetArea area, Qt::Orientation orientation)
    {
      addDockWidget(area, dock, orientation);
      dock->setVisible (false);
      dock->toggleViewAction ()->setIcon(QIcon::fromTheme("window-new"));
      dock->adjustSize();
      ui_->menuWindow->addAction(dock->toggleViewAction ());
      actionSearchBar_->addAction(dock->toggleViewAction ());
    }

    void MainWindow::removeDockWidget(QDockWidget *dock)
    {
      ui_->menuWindow->removeAction(dock->toggleViewAction());
      QMainWindow::removeDockWidget(dock);
    }

    BackgroundQueue& MainWindow::worker()
    {
      return backgroundQueue_;
    }

    WindowsManagerPtr_t MainWindow::osg() const
    {
      return osgViewerManagers_;
    }

    OSGWidget *MainWindow::centralWidget() const
    {
      return centralWidget_;
    }

    BodyTreeWidget *MainWindow::bodyTree() const
    {
      return ui_->bodyTreeContent;
    }

    QList<OSGWidget *> MainWindow::osgWindows() const
    {
      return osgWindows_;
    }

    PluginManager *MainWindow::pluginManager()
    {
      return &(settings_->pluginManager_);
    }

    ActionSearchBar* MainWindow::actionSearchBar () const
    {
      return actionSearchBar_;
    }

    void MainWindow::log(const QString &text)
    {
      if (thread() != QThread::currentThread()) {
        emit logString(text);
        return;
      }
      ui_->logText->insertHtml("<hr/><font color=black>"+text+"</font>");
    }

    void MainWindow::logError(const QString &text)
    {
      if (thread() != QThread::currentThread()) {
        emit logErrorString(text);
        return;
      }
      if (!ui_->dockWidget_log->isVisible()) {
        ui_->dockWidget_log->show();
      }
      QScrollBar* sb = ui_->logText->verticalScrollBar();
      bool SBwasAtBottom = sb->value() == sb->maximum();
      ui_->logText->insertHtml("<hr/><font color=red>"+text+"</font>");
      if (SBwasAtBottom)
        sb->setValue(sb->maximum());
    }

    void MainWindow::emitSendToBackground(WorkItem *item)
    {
      emit sendToBackground(item);
    }

    QMenu *MainWindow::pluginMenu() const
    {
      return ui_->menuWindow;
    }

    void MainWindow::logJobStarted(int id, const QString &text)
    {
      emit logString (QString ("Starting job ") + QString::number (id) + ": " + text);
    }

    void MainWindow::logJobDone(int id, const QString &text)
    {
      emit logString (QString ("Job ") + QString::number (id) + " done: " + text);
    }

    void MainWindow::logJobFailed(int id, const QString &text)
    {
      emit logErrorString (QString ("Job ") + QString::number (id) + " failed: " + text);
    }

    OSGWidget *MainWindow::createView(const std::string& name)
    {
      if (thread() != QThread::currentThread()) {
        delayedCreateView_.lock();
        emit createViewOnMainThread(name);
        delayedCreateView_.lock();
        delayedCreateView_.unlock();
        return osgWindows_.last();
      } else {
        OSGWidget* osgWidget = new OSGWidget (osgViewerManagers_, name, this, 0
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
            , osgViewer::Viewer::SingleThreaded
#endif
            );
        osgWidget->setObjectName(name.c_str());
        addOSGWidget (osgWidget);
        emit viewCreated(osgWidget);
        delayedCreateView_.unlock();
        return osgWidget;
      }
    }

    void MainWindow::requestRefresh()
    {
      emit refresh ();
    }

    void MainWindow::createDefaultView() {
      std::stringstream ss; ss << "hpp_gui_window_" << osgWindows_.size();
      createView (ss.str());
    }

    void MainWindow::addOSGWidget(OSGWidget* osgWidget)
    {
      if (!osgWindows_.empty()) {
        QDockWidget* dockOSG = new QDockWidget (
            tr("OSG Viewer") + " " + QString::number (osgWindows_.size()), this);
        dockOSG->setWidget(osgWidget);
        addDockWidget(Qt::RightDockWidgetArea, dockOSG);
      } else {
        // This OSGWidget should be the central view
        centralWidget_ = osgWidget;
        setCentralWidget(centralWidget_);
#if GEPETTO_GUI_HAS_PYTHONQT
        pythonWidget_->addToContext("osg", centralWidget_);
#endif
        connect(ui_->actionHome, SIGNAL (triggered()), centralWidget_, SLOT (onHome()));
        ui_->osgToolBar->show();

        osg()->addSceneToWindow("hpp-gui", centralWidget_->windowID());
        connect(ui_->actionAdd_floor, SIGNAL (triggered()), centralWidget_, SLOT (addFloor()));

        actionSearchBar_->addAction(new NodeAction("Attach camera to selected node", osgWidget, this));
      }
      osgWidget->addAction(actionSearchBar_->showAction());
      osgWindows_.append(osgWidget);
    }

    void MainWindow::openLoadRobotDialog()
    {
      statusBar()->showMessage("Loading a robot...");
      DialogLoadRobot* d = new DialogLoadRobot (this);
      if (d->exec () == QDialog::Accepted) {
        createCentralWidget();
        DialogLoadRobot::RobotDefinition rd = d->getSelectedRobotDescription();

        QString urdfFile = QString("package://%1/urdf/%2%3.urdf").arg(rd.package_).arg(rd.modelName_).arg(rd.urdfSuf_);
        try {
          osgViewerManagers_->addURDF(rd.robotName_.toStdString(), urdfFile.toStdString());
          osgViewerManagers_->addSceneToWindow(rd.robotName_.toStdString(), centralWidget_->windowID());
        } catch (std::runtime_error& exc) {
          logError (exc.what ());
        }
        robotNames_.append (rd.robotName_);

        QString what = QString ("Loading robot ") + rd.name_;
        WorkItem* item;
        foreach (ModelInterface* loader, pluginManager()->get <ModelInterface> ()) {
          item = new WorkItem_1 <ModelInterface, void,
               DialogLoadRobot::RobotDefinition>
                 (loader, &ModelInterface::loadRobotModel, rd);
          logJobStarted(item->id(), what);
          emit sendToBackground(item);
        }
      }
      d->close();
      statusBar()->clearMessage();
      d->deleteLater();
    }

    void MainWindow::openLoadEnvironmentDialog()
    {
      statusBar()->showMessage("Loading an environment...");
      DialogLoadEnvironment* e = new DialogLoadEnvironment (this);
      if (e->exec() == QDialog::Accepted) {
        createCentralWidget();
        DialogLoadEnvironment::EnvironmentDefinition ed = e->getSelectedDescription();

        QString urdfFile = QString("package://%1/urdf/%2.urdf").arg(ed.package_).arg(ed.urdfFilename_);
        try {
          osgViewerManagers_->addUrdfObjects(ed.envName_.toStdString(),
                                             urdfFile   .toStdString(),
                                             true);
          osgViewerManagers_->addSceneToWindow(ed.envName_.toStdString(),
                                               centralWidget_->windowID());
        } catch (std::runtime_error& exc) {
          log (exc.what ());
        }

        QString what = QString ("Loading environment ") + ed.name_;
        WorkItem* item;
        foreach (ModelInterface* loader, pluginManager()->get <ModelInterface> ()) {
          item = new WorkItem_1 <ModelInterface, void,
               DialogLoadEnvironment::EnvironmentDefinition>
                 (loader, &ModelInterface::loadEnvironmentModel, ed);
          logJobStarted(item->id(), what);
          emit sendToBackground(item);
        }
      }
      statusBar()->clearMessage();
      e->close();
      e->deleteLater();
    }

    void MainWindow::handleWorkerDone(int /*id*/)
    {
    }

    void MainWindow::resetConnection()
    {
      foreach (CorbaInterface* e, pluginManager()->get <CorbaInterface> ()) {
        e->openConnection ();
      }
    }

    void MainWindow::closeConnection()
    {
      foreach (CorbaInterface* e, pluginManager()->get <CorbaInterface> ()) {
        e->closeConnection ();
      }
    }

    void MainWindow::about()
    {
      QString devString;
      devString = trUtf8("<p>Version %1. For more information visit <a href=\"%2\">%2</a></p>"
          "<p><small>Copyright (c) 2015-2016 CNRS<br/>By Joseph Mirabel and others.</small></p>"
          "<p><small>"
          "%3 is free software: you can redistribute it and/or modify it under the "
          "terms of the GNU Lesser General Public License as published by the Free "
          "Software Foundation, either version 3 of the License, or (at your option) "
          "any later version.<br/><br/>"
          "%3 is distributed in the hope that it will be "
          "useful, but WITHOUT ANY WARRANTY; without even the implied warranty "
          "of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU "
          "General Lesser Public License for more details.  You should have "
          "received a copy of the GNU Lesser General Public License along with %3. "
          "If not, see <a href=\"http://www.gnu.org/licenses\">http://www.gnu.org/licenses<a/>."
          "</small></p>"
          )
        .arg(QApplication::applicationVersion())
        .arg(QApplication::organizationDomain())
        .arg(QApplication::applicationName());

      QMessageBox::about(this, QApplication::applicationName(), devString);
    }

    void MainWindow::activateCollision(bool activate)
    {
      if (activate) {
        requestConfigurationValidation();
        foreach (QString b, lastBodiesInCollision_) {
          osg ()->setHighlight(b.toLocal8Bit().data(), 1);
        }
      }
      else {
        foreach (QString b, lastBodiesInCollision_) {
          osg ()->setHighlight(b.toLocal8Bit().data(), 0);
        }
        collisionIndicator_->switchLed(true);
        collisionLabel_->setText("No collisions.");
      }
    }

    void MainWindow::setupInterface()
    {
      // Menu "Window"
      QMenu* toolbar = ui_->menuWindow->addMenu("Tool bar");
      toolbar->setIcon(QIcon::fromTheme("configure-toolbars"));
      ui_->mainToolBar->setVisible(true);
      ui_->osgToolBar->setVisible(false);
      toolbar->addAction (ui_->mainToolBar->toggleViewAction ());
      toolbar->addAction (ui_->osgToolBar->toggleViewAction ());

      ui_->menuWindow->addSeparator();

      ui_->dockWidget_bodyTree->setVisible (false);
      ui_->dockWidget_bodyTree->toggleViewAction ()->setIcon(QIcon::fromTheme("window-new"));
      ui_->dockWidget_bodyTree->toggleViewAction ()->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_B);
      ui_->menuWindow->addAction(ui_->dockWidget_bodyTree->toggleViewAction ());
      ui_->dockWidget_log->setVisible (false);
      ui_->dockWidget_log->toggleViewAction ()->setIcon(QIcon::fromTheme("window-new"));
      ui_->dockWidget_log->toggleViewAction ()->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_L);
      ui_->menuWindow->addAction(ui_->dockWidget_log->toggleViewAction ());
#if GEPETTO_GUI_HAS_PYTHONQT
      insertDockWidget(pythonWidget_, Qt::BottomDockWidgetArea, Qt::Horizontal);
      registerShortcut("Python console", "Toggle view", pythonWidget_->toggleViewAction());
#endif

      registerShortcut("Log widget", "Toggle view", ui_->dockWidget_log->toggleViewAction ());
      registerShortcut("Body tree widget", "Toggle view", ui_->dockWidget_bodyTree->toggleViewAction ());
      registerShortcut("Main window", ui_->actionLoad_robot_from_file);
      registerShortcut("Main window", ui_->actionLoad_environment);
      registerShortcut("Main window", ui_->actionChange_shortcut);
      registerShortcut("Main window", ui_->actionQuit);
      registerShortcut("Main window", ui_->actionReconnect);
      registerShortcut("Main window", ui_->actionRefresh);
      registerShortcut("Main window", ui_->actionPlugins);

      ui_->menuWindow->addSeparator();

      // Setup the status bar
      collisionIndicator_ = new LedIndicator (statusBar());
      collisionValidationActivated_ = new QCheckBox ();
      collisionValidationActivated_->setToolTip (tr("Automatically validate configurations."));
      collisionValidationActivated_->setCheckState (Qt::Checked);
      statusBar()->addPermanentWidget(collisionLabel_);
      statusBar()->addPermanentWidget(collisionValidationActivated_);
      statusBar()->addPermanentWidget(collisionIndicator_);

      connect (collisionValidationActivated_, SIGNAL(clicked(bool)), SLOT(activateCollision(bool)));
      connect (collisionIndicator_, SIGNAL (mouseClickEvent()), SLOT(requestConfigurationValidation()));
      connect (ui_->actionAbout, SIGNAL (triggered ()), SLOT(about()));
      connect (ui_->actionReconnect, SIGNAL (triggered ()), SLOT(resetConnection()));
      connect (ui_->actionClose_connections, SIGNAL (triggered ()), SLOT(closeConnection()));
      connect (ui_->actionFetch_configuration, SIGNAL (triggered ()), SLOT(requestApplyCurrentConfiguration()));

      connect (this, SIGNAL(logString(QString)), SLOT(log(QString)));
      connect (this, SIGNAL(logErrorString(QString)), SLOT(logError(QString)));

      // actionSearchBar_->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
      actionSearchBar_->setWindowFlags(Qt::Popup);
      actionSearchBar_->addAction(new NodeAction(NodeAction::VISIBILITY_ON,  "Show node", this));
      actionSearchBar_->addAction(new NodeAction(NodeAction::VISIBILITY_OFF, "Hide node", this));
      actionSearchBar_->addAction(new NodeAction(NodeAction::ALWAYS_ON_TOP, "Always on top", this));
      actionSearchBar_->addAction(ui_->actionFetch_configuration);
      actionSearchBar_->addAction(ui_->actionClose_connections);
      actionSearchBar_->addAction(ui_->actionReconnect);
      actionSearchBar_->addAction(ui_->actionRefresh);
    }

    void MainWindow::createCentralWidget()
    {
      if (!osgWindows_.empty()) return;
      createDefaultView();
    }

    void MainWindow::requestApplyCurrentConfiguration()
    {
      emit applyCurrentConfiguration();
      if (collisionValidationActivated_->isChecked ())
        requestConfigurationValidation();
    }

    void MainWindow::requestConfigurationValidation()
    {
      emit configurationValidation();
    }

    void MainWindow::onOpenPluginManager()
    {
      PluginManagerDialog d (pluginManager(), this);
      d.exec ();
    }

    void MainWindow::registerSignal(const char *signal, QObject *obj)
    {
      if (registeredSignals_.find(signal) == registeredSignals_.end())
        {
          registeredSignals_[signal] = obj;
          qDebug() << signal << " registered";
        }
      else
        std::cout << "Signal " << signal << "already registered." << std::endl;
    }

    QObject* MainWindow::getFromSignal(const char* signal)
    {
      if (registeredSignals_.find(signal) == registeredSignals_.end())
        {
          std::cout << "signal " << signal << "isn't registered" << std::endl;
          return NULL;
        }
      return registeredSignals_[signal];
    }

    void MainWindow::connectSignal(const char *signal, const char *slot, QObject* obj)
    {
      QObject* obj_sig = getFromSignal(signal);
      if (obj_sig != NULL) {
        QObject::connect(obj_sig, signal, obj, slot);
      }
    }

    void MainWindow::registerSlot(const char *slot, QObject *obj)
    {
      if (registeredSlots_.find(slot) == registeredSlots_.end())
        {
          registeredSlots_[slot] = obj;
          qDebug() << slot << " registered";
        }
      else
        std::cout << "Slot " << slot << "already registered." << std::endl;
    }

    QObject* MainWindow::getFromSlot(const char* slot)
    {
      if (registeredSlots_.find(slot) == registeredSlots_.end())
        {
          std::cout << "slot " << slot << "isn't registered" << std::endl;
          return NULL;
        }
      return registeredSlots_[slot];
    }

    void MainWindow::connectSlot(const char *slot, const char *signal, QObject* obj)
    {
      if (registeredSlots_.find(slot) != registeredSlots_.end()) {
        QObject::connect(obj, signal, registeredSlots_[slot], slot);
      }
    }

    void MainWindow::registerShortcut(QString widgetName, QString actionName, QAction* action)
    {
      shortcutFactory_->addBinding(widgetName, actionName, action);
    }

    void MainWindow::registerShortcut(QString widgetName, QAction* action)
    {
      shortcutFactory_->addBinding(widgetName, action->text(), action);
    }

    void MainWindow::configurationValidationStatusChanged (bool valid)
    {
      collisionIndicator_->switchLed (valid);
      int state = (valid)?0:1;
      foreach(const QString& s, robotNames_) {
        osg ()->setHighlight(s.toLocal8Bit().data(), state);
      }
    }

    void MainWindow::configurationValidationStatusChanged (QStringList bodiesInCollision)
    {
      QStringList lastBodiesInCollision = lastBodiesInCollision_;
      lastBodiesInCollision_.clear();
      collisionIndicator_->switchLed (bodiesInCollision.empty());
      foreach (QString b, lastBodiesInCollision) {
        if (bodiesInCollision.removeAll(b) == 0) {
          /// This body is not in collision
          osg ()->setHighlight(b.toLocal8Bit().data(), 0);
        } else {
          /// This body is still in collision
          lastBodiesInCollision_.append(b);
        }
      }
      QString tooltip ("Collision between ");
      foreach(const QString& b, bodiesInCollision) {
        osg ()->setHighlight(b.toLocal8Bit().data(), 1);
        lastBodiesInCollision_.append(b);
      }
      tooltip += lastBodiesInCollision_.join (", ");
      if (lastBodiesInCollision_.count() > 0)
        collisionLabel_->setText(tooltip);
      else
        collisionLabel_->setText("No collisions.");
    }

    void MainWindow::requestSelectJointFromBodyName(const QString bodyName)
    {
      emit selectJointFromBodyName(bodyName);
    }
  } // namespace gui
} // namespace gepetto
