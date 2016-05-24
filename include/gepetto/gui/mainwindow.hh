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

        static MainWindow* instance ();

        void insertDockWidget (QDockWidget* dock,
            Qt::DockWidgetArea area = Qt::AllDockWidgetAreas,
            Qt::Orientation orientation = Qt::Horizontal);

        void removeDockWidget (QDockWidget* dock);

        BackgroundQueue &worker();

        WindowsManagerPtr_t osg () const;

        OSGWidget* centralWidget() const;

        QList <OSGWidget*> osgWindows () const;

        BodyTreeWidget* bodyTree () const;

        PluginManager* pluginManager ();

        void emitSendToBackground (WorkItem* item);
#if GEPETTO_GUI_HAS_PYTHONQT
        PythonWidget* pythonWidget()
        {
          return pythonWidget_;
        }
#endif

signals:
        void sendToBackground (WorkItem* item);
        void createView (QString name);
        /// Triggered when an OSGWidget is created.
        void viewCreated (OSGWidget* widget);
        void refresh ();
        void applyCurrentConfiguration();
        void configurationValidation();
        void selectJointFromBodyName(const QString bodyName);

        public slots:
        void log (const QString& text);
        void logError (const QString& text);

          void logJobStarted (int id, const QString& text);
        void logJobDone    (int id, const QString& text);
        void logJobFailed  (int id, const QString& text);

        OSGWidget* delayedCreateView (QString name = "");
        /// Request a refresh of the interface.
        /// \param refreshType tells what to refresh. See RefreshType
        void requestRefresh (int refreshType = RefreshAll);
        void requestApplyCurrentConfiguration ();
        void requestConfigurationValidation ();
        void configurationValidationStatusChanged (bool valid);
        void configurationValidationStatusChanged (QStringList bodiesInCollision);
        void requestSelectJointFromBodyName (const QString bodyName);
        void onOpenPluginManager ();

        private slots:
          OSGWidget* onCreateView(QString name);
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
    };
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_MAINWINDOW_HH
