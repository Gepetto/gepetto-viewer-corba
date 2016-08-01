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
#include <gepetto/gui/bodytreewidget.hh>

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
        std::string name,
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
        , mode_ (CAMERA_MANIPULATION)
        , selectionFinished_( true )
        , infoBox_ (this)
    {
      osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
      osg::ref_ptr <osg::GraphicsContext::Traits> traits_ptr (new osg::GraphicsContext::Traits);
      traits_ptr->windowName = "Gepetto Viewer";
      traits_ptr->x = this->x();
      traits_ptr->y = this->y();
      traits_ptr->width = this->width();
      traits_ptr->height = this->height();
      traits_ptr->windowDecoration = false;
      traits_ptr->doubleBuffer = true;
      traits_ptr->vsync = true;
      //  traits_ptr->sharedContext = 0;

      traits_ptr->alpha = ds->getMinimumNumAlphaBits();
      traits_ptr->stencil = ds->getMinimumNumStencilBits();
      traits_ptr->sampleBuffers = ds->getMultiSamples();
      traits_ptr->samples = ds->getNumMultiSamples();
      traits_ptr->sampleBuffers = 4;
      traits_ptr->samples = 4;

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

      wid_ = wm->createWindow (name.c_str(), viewer_, graphicsWindow_.get());
      wm_ = wsm_->getWindowManager (wid_);

      viewer_->setThreadingModel(threadingModel);

      QGLWidget* glWidget = graphicsWindow_->getGLWidget();
      QVBoxLayout* hblayout = new QVBoxLayout (this);
      hblayout->setContentsMargins(1,1,1,1);
      setLayout (hblayout);
      hblayout->addWidget(glWidget);

      render_.viewerPtr = viewer_;
      render_.wsm_ = wsm_;
      render_.refreshRate = parent->settings_->refreshRate;
      render_.start ();

      parent->bodyTree()->connect(this,
          SIGNAL (clicked(QString,QVector3D,QKeyEvent*)), SLOT (selectBodyByName(QString)));
      parent->connect(this, SIGNAL (clicked(QString,QVector3D,QKeyEvent*)),
		      SLOT (requestSelectJointFromBodyName(QString)));
    }

    OSGWidget::~OSGWidget()
    {
      viewer_->setDone(true);
      viewer_->removeEventHandler(pickHandler_);
      pickHandler_ = NULL;
      wm_.reset();
      wsm_.reset();
    }

    graphics::WindowsManager::WindowID OSGWidget::windowID() const
    {
      return wid_;
    }

    void OSGWidget::loadURDF(const QString robotName,
        const QString urdf_file_path,
        const QString meshDataRootDir)
    {
      QByteArray rn = robotName.toLocal8Bit();
      QByteArray uf = urdf_file_path.toLocal8Bit();
      QByteArray md = meshDataRootDir.toLocal8Bit();
      wsm_->addURDF(rn.constData(),
          uf.constData(),
          md.constData());
      wsm_->addSceneToWindow(rn.constData(), wid_);
      MainWindow* w = dynamic_cast <MainWindow*> (parentWidget());
      if (w) {
        w->bodyTree()->addBodyToTree (wsm_->getScene (robotName.toStdString()));
      }
    }

    void OSGWidget::paintEvent( QPaintEvent* /* paintEvent */ )
    {
//        wsm_->lock().lock();
//        viewer_->frame();
      //        wsm_->lock().unlock();
    }

    void OSGWidget::emitClicked(QString name, QVector3D positionInWorldFrame, QKeyEvent* event)
    {
      emit clicked (name, positionInWorldFrame, event);
    }

    void OSGWidget::onHome()
    {
      viewer_->home ();
    }

    void OSGWidget::changeMode(Mode mode)
    {
      mode_ = mode;
      infoBox_.setMode (mode);
    }

    void OSGWidget::selectionMode()
    {
      mode_ = NODE_SELECTION;
      infoBox_.setMode(NODE_SELECTION);
    }

    void OSGWidget::cameraManipulationMode()
    {
      mode_ = CAMERA_MANIPULATION;
      infoBox_.setMode(CAMERA_MANIPULATION);
    }

    void OSGWidget::addFloor()
    {
      wsm_->addFloor("hpp-gui/floor");
    }

    void OSGWidget::attachToWindow(const std::string nodeName)
    {
      wsm_->addSceneToWindow(nodeName.c_str(), wid_);
    }

    osgGA::EventQueue* OSGWidget::getEventQueue() const
    {
      osgGA::EventQueue* eventQueue = viewer_->getEventQueue();

      if( eventQueue )
        return( eventQueue );
      else
        throw( std::runtime_error( "Unable to obtain valid event queue") );
    }

    std::list <graphics::NodePtr_t> OSGWidget::processSelection()
    {
      NodeList nodes;
      QRect selectionRectangle = makeRectangle( selectionStart_, selectionEnd_ );
      int widgetHeight         = this->height();

      double xMin = selectionRectangle.left();
      double xMax = selectionRectangle.right();
      double yMin = widgetHeight - selectionRectangle.bottom();
      double yMax = widgetHeight - selectionRectangle.top();

      osg::ref_ptr<osgUtil::PolytopeIntersector> polytopeIntersector
        = new osgUtil::PolytopeIntersector( osgUtil::PolytopeIntersector::WINDOW,
            xMin, yMin,
            xMax, yMax );

      // This limits the amount of intersections that are reported by the
      // polytope intersector. Using this setting, a single drawable will
      // appear at most once while calculating intersections. This is the
      // preferred and expected behaviour.
      polytopeIntersector->setIntersectionLimit( osgUtil::Intersector::LIMIT_ONE_PER_DRAWABLE );

      osgUtil::IntersectionVisitor iv( polytopeIntersector );

      osg::ref_ptr<osg::Camera> camera = viewer_->getCamera();

      if( !camera )
        throw std::runtime_error( "Unable to obtain valid camera for selection processing" );

      camera->accept( iv );

      if( !polytopeIntersector->containsIntersections() )
        return nodes;

      osgUtil::PolytopeIntersector::Intersections intersections = polytopeIntersector->getIntersections();

      for(osgUtil::PolytopeIntersector::Intersections::iterator it = intersections.begin();
          it != intersections.end(); ++it) {
        for (int i = (int) it->nodePath.size()-1; i >= 0 ; --i) {
          graphics::NodePtr_t n = wsm_->getNode(it->nodePath[i]->getName ());
          if (n) {
            if (boost::regex_match (n->getID().c_str(), boost::regex ("^.*_[0-9]+$")))
              continue;
            nodes.push_back(n);
            break;
          }
        }
      }
      return nodes;
    }

    OSGWidget::InfoBox::InfoBox(QWidget *parent) :
      size_ (16,16),
      selection_ (QIcon::fromTheme("edit-select").pixmap(size_)),
      record_ (QIcon::fromTheme("media-record").pixmap(size_)),
      label_ (new QLabel (parent))
    {
      label_->setAutoFillBackground(true);
      label_->hide();
      label_->setGeometry(QRect (QPoint(0,0), size_));
    }

    void OSGWidget::InfoBox::normalMode()
    {
      label_->hide();
    }

    void OSGWidget::InfoBox::selectionMode()
    {
      label_->show();
      label_->setPixmap(selection_);
    }

    void OSGWidget::InfoBox::recordMode()
    {
      label_->show();
      label_->setPixmap(record_);
    }

    void OSGWidget::InfoBox::setMode(OSGWidget::Mode mode)
    {
      switch (mode) {
        case CAMERA_MANIPULATION:
          normalMode();
          break;
        case NODE_SELECTION:
          selectionMode();
          break;
        case NODE_MOTION:
          selectionMode();
          break;
        default:
          normalMode();
          break;
      }
    }

    void RenderThread::run()
    {
      if (viewerPtr) {
          while (!viewerPtr->done ()) {
              wsm_->lock ().lock();
              viewerPtr->frame();
              wsm_->lock ().unlock();
              QThread::msleep(30);
            }
        }
    }

    RenderThread::~RenderThread() {
      if (viewerPtr) viewerPtr->setDone(true);
      wait();
    }

  } // namespace gui
} // namespace gepetto
