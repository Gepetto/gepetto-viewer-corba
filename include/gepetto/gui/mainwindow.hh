#ifndef GEPETTO_GUI_MAINWINDOW_HH
#define GEPETTO_GUI_MAINWINDOW_HH

#include <QList>
#include <QStandardItemModel>
#include <QMainWindow>

#include <gepetto/viewer/group-node.h>

#include <gepetto/gui/fwd.hh>

#include <gepetto/gui/ledindicator.hh>

#include <gepetto/gui/omniorb/omniorbthread.hh>
#include <gepetto/gui/dialog/dialogloadrobot.hh>
#include <gepetto/gui/dialog/dialogloadenvironment.hh>
#include <gepetto/gui/dialog/pluginmanagerdialog.hh>

#include <gepetto/gui/settings.hh>

namespace Ui {
  class MainWindow;
}

namespace gepetto {
  namespace gui {
    /// Class
    class MainWindow : public QMainWindow
    {
      Q_OBJECT

      public:
        enum RefreshType {
          RefreshBodyTree = 1,
          RefreshPlugins  = 2,
          RefreshAll = RefreshBodyTree + RefreshPlugins
        };

        Settings* settings_;

        explicit MainWindow(Settings* settings, QWidget *parent = 0);
        ~MainWindow();

        /// Return the instance of MainWindow stored
        static MainWindow* instance ();

        /// Insert a dockwidget in the dock area of the window.
        /// \param dock widget to add
        /// \param area where the widget will be add
        /// \param orientation of the widget
        void insertDockWidget (QDockWidget* dock,
            Qt::DockWidgetArea area = Qt::AllDockWidgetAreas,
            Qt::Orientation orientation = Qt::Horizontal);

        /// Remove a dock widget.
        /// \param dock widget to remove
        void removeDockWidget (QDockWidget* dock);

        /// Get the worker.
        BackgroundQueue &worker();

        /// Get
        WindowsManagerPtr_t osg () const;

        /// Get the central widget.
        OSGWidget* centralWidget() const;

        /// Get the list of windows.
        QList <OSGWidget*> osgWindows () const;

        /// Get the plugin manager.
        PluginManager* pluginManager ();

        /// Emit a signal to indicate that a job has been put in background.
        void emitSendToBackground (WorkItem* item);

        QMenu* pluginMenu () const;

#if GEPETTO_GUI_HAS_PYTHONQT
        /// Get the python widget.
        PythonWidget* pythonWidget()
        {
          return pythonWidget_;
        }
#endif

signals:
        void sendToBackground (WorkItem* item);
        void createView (const std::string& name);
        /// Triggered when an OSGWidget is created.
        void viewCreated (OSGWidget* widget);
        void refresh ();
        void applyCurrentConfiguration();
        void configurationValidation();
        void selectJointFromBodyName(const QString bodyName);

        public slots:
          /// \addtogroup available_in_python Python API
          /// These slots are available for Python scripting in plugins
          /// \{

        /// Get the body tree widget.
        BodyTreeWidget* bodyTree () const;

        /// Add the text to logs.
        /// \param text text to log
        void log (const QString& text);
        /// Add the text to logs and colors it in red.
        /// \param text text to log
        void logError (const QString& text);

        /// Log that a job has started.
        /// \param id id of the job
        /// \param text text to log
          void logJobStarted (int id, const QString& text);

          /// Log that a job has successfuly finished.
          /// \param id id of the job
          /// \param text text to log
          void logJobDone    (int id, const QString& text);

          /// Log that a job has failed.
          /// \param id id of the job
          /// \param text text to log
          void logJobFailed  (int id, const QString& text);

        OSGWidget* delayedCreateView (const std::string& name = "");
        /// Request a refresh of the interface.
        /// \param refreshType tells what to refresh. See RefreshType
        void requestRefresh ();
        /// Emit a signal to display the current configuration in the viewer.
        void requestApplyCurrentConfiguration ();

        /// Emit a signal to check if the the current configuration is valid.
        void requestConfigurationValidation ();

        /// Display if a configuration is valid or not.
        /// \param valid configuration is valid
        void configurationValidationStatusChanged (bool valid);

        /// Display if a configuration is valid or not.
        /// \param bodiesInCollision list of bodies in collision
        void configurationValidationStatusChanged (QStringList bodiesInCollision);

        /// Emit a signal to tell that a body has been selected.
        /// \param bodyName name of the body selected
        void requestSelectJointFromBodyName (const QString bodyName);

        /// Open the plugin manager dialog.
        void onOpenPluginManager ();

        /// Register an object slot that can be accessible without knowing the class definition.
        /// \param slot slot's name
        /// \param obj object's instance
        void registerSlot(const char *slot, QObject* obj);

        /// Get the instance of object which holds the slot.
        /// \param slot slot's name
        /// \return object's instance
        QObject* getFromSlot(const char *slot);

        /// Connect an object's signal to a registered slot.
        /// \param slot slot's name
        /// \param signal signal's name
        /// \param obj object's instance
        void connectSlot(const char *slot, const char *signal, QObject* obj);

        /// Register an action on which users can change the shortcut to trigger it.
        /// \param widgetName widget's name that handle the action
        /// \param actionName action's name
        /// \param action action to modify
        void registerShortcut(QString widgetName, QString actionName, QAction* action);


        /// Register an action on which users can change the shortcut to trigger it.
        /// User action->text() as action name.
        /// \param widgetName widget's name that handle the action
        /// \param action action to modify
        void registerShortcut(QString widgetName, QAction* action);

        /// \}


        private slots:
          OSGWidget* onCreateView(const std::string& name);
          OSGWidget* onCreateView();
        void openLoadRobotDialog ();
        void openLoadEnvironmentDialog ();
        void activateCollision(bool activate);

        void handleWorkerDone (int id);

        void resetConnection ();
        void about ();

      private:
        void setupInterface ();
        void createCentralWidget ();

        static MainWindow* instance_;

        ::Ui::MainWindow* ui_;
        OSGWidget* centralWidget_;
        QList <OSGWidget*> osgWindows_;
#if GEPETTO_GUI_HAS_PYTHONQT
        PythonWidget* pythonWidget_;
#endif
      ShortcutFactory* shortcutFactory_;
      SelectionHandler* selectionHandler_;

        WindowsManagerPtr_t osgViewerManagers_;
        CorbaServer* osgServer_;
        BackgroundQueue backgroundQueue_;
        QThread worker_;

        QCheckBox* collisionValidationActivated_;
        LedIndicator* collisionIndicator_;
        QLabel* collisionLabel_;

        QStandardItemModel *bodyTreeModel_;

        QMutex delayedCreateView_;
        QStringList robotNames_;
        QStringList lastBodiesInCollision_;

        std::map<std::string, QObject *> registeredSlots_;
    };
  } // namespace gui
} // namespace gepetto

Q_DECLARE_METATYPE (std::string)

#endif // GEPETTO_GUI_MAINWINDOW_HH
