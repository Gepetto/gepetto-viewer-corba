#ifndef GEPETTO_GUI_OSGWIDGET_HH
#define GEPETTO_GUI_OSGWIDGET_HH

#include <QString>
#include <QThread>
#include <QLabel>
#include <QTimer>

#include <osg/ref_ptr>

#include <osgQt/GraphicsWindowQt>
#include <gepetto/viewer/window-manager.h>

#include <gepetto/gui/fwd.hh>
#include <gepetto/gui/windows-manager.hh>

namespace gepetto {
  namespace gui {
    class RenderThread : public QThread {
    public:
      RenderThread () :
        QThread (), viewerPtr (0), refreshRate (30)
      {}

      virtual ~RenderThread();

      osgViewer::ViewerRefPtr viewerPtr;
      WindowsManagerPtr_t wsm_;
      int refreshRate;
      bool stop;

      // QThread interface
    protected:
      void run();
    };

    /// Widget that displays scenes.
    class OSGWidget : public QWidget
    {
      Q_OBJECT
      public:
        typedef std::list <graphics::NodePtr_t> NodeList;

        enum Mode {
          CAMERA_MANIPULATION,
          NODE_SELECTION,
          NODE_MOTION
        };

        OSGWidget( WindowsManagerPtr_t wm,
            std::string name,
            MainWindow* parent = 0,
            Qt::WindowFlags f = 0,
            osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::Viewer::SingleThreaded );

        virtual ~OSGWidget();

        WindowsManager::WindowID windowID () const;

signals:
        void selected (QString name);
        void requestMotion (graphics::NodePtr_t node, graphics::Node::Arrow direction,
            float speed);

        public slots:
        /// Load an urdf file in the viewer.
        /// \param robotName name of the robot
        /// \param udrf_file_path path to the urdf file
        /// \param meshDataRootDir path to the mesh directory
          void loadURDF (const QString robotName,
              const QString urdf_file_path,
              const QString meshDataRootDir);

        /// Replace the camera at her home position.
        virtual void onHome();

        /// Change
        void changeMode (Mode mode);
        void selectionMode ();
        void cameraManipulationMode ();
        void addFloor();
        void attachToWindow (const std::string nodeName);

      protected:

        virtual void paintEvent( QPaintEvent* paintEvent );

    private slots:
        void transferSelected (QString name);

      private:
        osgGA::EventQueue* getEventQueue() const;

        osg::ref_ptr<osgQt::GraphicsWindowQt> graphicsWindow_;
        WindowsManagerPtr_t wsm_;
        osg::ref_ptr<PickHandler> pickHandler_;
        WindowsManager::WindowID wid_;
        graphics::WindowManagerPtr_t wm_;
        RenderThread render_;
        osgViewer::ViewerRefPtr viewer_;
        osg::ref_ptr <osgViewer::ScreenCaptureHandler> screenCapture_;

        QPoint selectionStart_;
        QPoint selectionEnd_;
        graphics::NodePtr_t selectedNode_;

        Mode mode_;
        bool selectionFinished_;

        std::list <graphics::NodePtr_t> processPoint ();
        std::list <graphics::NodePtr_t> processSelection();

        struct InfoBox {
          QSize size_;
          QPixmap selection_, record_;
          QLabel* label_;

          InfoBox (QWidget* parent);
          void normalMode ();
          void selectionMode ();
          void recordMode ();
          void setMode (Mode mode);
        };
        InfoBox infoBox_;

        friend class PickHandler;
    };
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_OSGWIDGET_HH
