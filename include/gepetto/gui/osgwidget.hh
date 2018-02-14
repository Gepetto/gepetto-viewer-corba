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
    /// Widget that displays scenes.
    class OSGWidget : public QWidget
    {
      Q_OBJECT
      public:
        OSGWidget( WindowsManagerPtr_t wm,
                  const std::string & name,
                  MainWindow* parent,
                  Qt::WindowFlags f = 0,
                  osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::Viewer::ThreadPerContext );

        virtual ~OSGWidget();

        WindowsManager::WindowID windowID () const;

        graphics::WindowManagerPtr_t window () const;

        WindowsManagerPtr_t osg () const;

        public slots:
        /// Replace the camera at her home position.
        virtual void onHome();

        int wid () const;

        void addFloor();
        void attachToWindow (const std::string nodeName);

      protected:
        virtual void paintEvent(QPaintEvent* event);

      private:
        osg::ref_ptr<osgQt::GraphicsWindowQt> graphicsWindow_;
        WindowsManagerPtr_t wsm_;
        osg::ref_ptr<PickHandler> pickHandler_;
        WindowsManager::WindowID wid_;
        graphics::WindowManagerPtr_t wm_;
        QTimer timer_;
        osgViewer::ViewerRefPtr viewer_;
        osg::ref_ptr <osgViewer::ScreenCaptureHandler> screenCapture_;

        friend class PickHandler;
    };
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_OSGWIDGET_HH
