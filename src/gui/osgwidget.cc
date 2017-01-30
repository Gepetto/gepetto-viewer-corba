#include "gepetto/gui/osgwidget.hh"
#include "gepetto/gui/mainwindow.hh"
#include <gepetto/gui/pick-handler.hh>

#include <boost/regex.hpp>

#include <osg/Camera>

#include <osg/DisplaySettings>
#include <osg/Geode>
#include <osg/Material>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/StateSet>

#include <osgGA/EventQueue>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/TrackballManipulator>

#include <osgUtil/IntersectionVisitor>
#include <osgUtil/PolytopeIntersector>

#include <osgViewer/View>
#include <osgViewer/ViewerEventHandlers>

#include <cassert>

#include <stdexcept>
#include <vector>

#include <QDebug>
#include <QKeyEvent>
#include <QWheelEvent>

#include <gepetto/viewer/urdf-parser.h>
#include <gepetto/viewer/OSGManipulator/keyboard-manipulator.h>

#include <gepetto/gui/windows-manager.hh>
#include <gepetto/gui/selection-event.hh>

namespace gepetto {
  namespace gui {
    namespace
    {

      QRect makeRectangle( const QPoint& first, const QPoint& second )
      {
        // Relative to the first point, the second point may be in either one of the
        // four quadrants of an Euclidean coordinate system.
        //
        // We enumerate them in counter-clockwise order, starting from the lower-right
        // quadrant that corresponds to the default case:
        //
        //            |
        //       (3)  |  (4)
        //            |
        //     -------|-------
        //            |
        //       (2)  |  (1)
        //            |

        if( second.x() >= first.x() && second.y() >= first.y() )
          return QRect( first, second );
        else if( second.x() < first.x() && second.y() >= first.y() )
          return QRect( QPoint( second.x(), first.y() ), QPoint( first.x(), second.y() ) );
        else if( second.x() < first.x() && second.y() < first.y() )
          return QRect( second, first );
        else if( second.x() >= first.x() && second.y() < first.y() )
          return QRect( QPoint( first.x(), second.y() ), QPoint( second.x(), first.y() ) );

        // Should never reach that point...
        return QRect();
      }

    }

    OSGWidget::OSGWidget(WindowsManagerPtr_t wm,
                         const std::string & name,
                         MainWindow *parent, Qt::WindowFlags f ,
                         osgViewer::ViewerBase::ThreadingModel threadingModel)
    : QWidget( parent, f )
    , graphicsWindow_()
    , wsm_ (wm)
    , pickHandler_ (new PickHandler (this, wsm_))
    , wid_ (-1)
    , wm_ ()
    , viewer_ (new osgViewer::Viewer)
    , screenCapture_ ()
    {
      osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
      osg::ref_ptr <osg::GraphicsContext::Traits> traits_ptr (new osg::GraphicsContext::Traits(ds));
      traits_ptr->windowName = "Gepetto Viewer";
      traits_ptr->x = this->x();
      traits_ptr->y = this->y();
      traits_ptr->width = this->width();
      traits_ptr->height = this->height();
      traits_ptr->windowDecoration = false;
      traits_ptr->doubleBuffer = true;
      traits_ptr->vsync = true;
      //  traits_ptr->sharedContext = 0;

      graphicsWindow_ = new osgQt::GraphicsWindowQt ( traits_ptr );

      osg::Camera* camera = viewer_->getCamera();
      camera->setGraphicsContext( graphicsWindow_ );

      camera->setClearColor( osg::Vec4(0.2f, 0.2f, 0.6f, 1.0f) );
      camera->setViewport( new osg::Viewport(0, 0, traits_ptr->width, traits_ptr->height) );
      camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits_ptr->width)/static_cast<double>(traits_ptr->height), 1.0f, 10000.0f );

      viewer_->setKeyEventSetsDone(0);


      osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;
      keyswitchManipulator->addMatrixManipulator( '1', "Trackball", new osgGA::TrackballManipulator() );
      keyswitchManipulator->addMatrixManipulator( '2', "First person", new ::osgGA::KeyboardManipulator(graphicsWindow_));
      keyswitchManipulator->selectMatrixManipulator (0);
      viewer_->setCameraManipulator( keyswitchManipulator.get() );

      screenCapture_ = new osgViewer::ScreenCaptureHandler (
            new osgViewer::ScreenCaptureHandler::WriteToFile (
              parent->settings_->captureDirectory + "/" + parent->settings_->captureFilename,
              parent->settings_->captureExtension),
            1);
      viewer_->addEventHandler(screenCapture_);
      viewer_->addEventHandler(new osgViewer::HelpHandler);
      viewer_->addEventHandler(pickHandler_);

      wid_ = wm->createWindow (name, viewer_, graphicsWindow_.get());
      wm_ = wsm_->getWindowManager (wid_);

      viewer_->setThreadingModel(threadingModel);

      osgQt::GLWidget* glWidget = graphicsWindow_->getGLWidget();
      //glWidget->setForwardKeyEvents(true);
      QVBoxLayout* hblayout = new QVBoxLayout (this);
      hblayout->setContentsMargins(1,1,1,1);
      setLayout (hblayout);
      hblayout->addWidget(glWidget);
      setMinimumSize(10,10);

      connect( &timer_, SIGNAL(timeout()), this, SLOT(update()) );
      timer_.start(parent->settings_->refreshRate);
    }

    OSGWidget::~OSGWidget()
    {
      viewer_->setDone(true);
      viewer_->removeEventHandler(pickHandler_);
      pickHandler_ = NULL;
      wm_.reset();
      wsm_.reset();
    }

    void OSGWidget::paintEvent(QPaintEvent*)
    {
      wsm_->lock().lock();
      viewer_->frame();
      wsm_->lock().unlock();
    }

    graphics::WindowsManager::WindowID OSGWidget::windowID() const
    {
      return wid_;
    }

    graphics::WindowManagerPtr_t OSGWidget::window() const
    {
      return wm_;
    }

    WindowsManagerPtr_t OSGWidget::osg() const
    {
      return wsm_;
    }

    void OSGWidget::loadURDF(const QString robotName,
        const QString urdf_file_path)
    {
      QByteArray rn = robotName.toLocal8Bit();
      QByteArray uf = urdf_file_path.toLocal8Bit();
      wsm_->addURDF(rn.constData(),
          uf.constData(),
          "");
      wsm_->addSceneToWindow(rn.constData(), wid_);
    }

    void OSGWidget::onHome()
    {
      viewer_->home ();
    }

    void OSGWidget::addFloor()
    {
      wsm_->addFloor("hpp-gui/floor");
    }

    void OSGWidget::attachToWindow(const std::string nodeName)
    {
      wsm_->addSceneToWindow(nodeName, wid_);
    }
  } // namespace gui
} // namespace gepetto
