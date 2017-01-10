#include "gepetto/gui/pick-handler.hh"

#include <QDebug>

#include <osg/io_utils>

#include <osgUtil/IntersectionVisitor>
#include <osgUtil/LineSegmentIntersector>

#include <osgViewer/Viewer>

#include <iostream>
#include <boost/regex.hpp>

#include <gepetto/gui/selection-event.hh>
#include <gepetto/gui/windows-manager.hh>
#include <gepetto/gui/osgwidget.hh>
#include <gepetto/gui/mainwindow.hh>
#include <gepetto/gui/bodytreewidget.hh>
#include <gepetto/gui/tree-item.hh>
#include <gepetto/gui/selection-handler.hh>

namespace gepetto {
  namespace gui {
    PickHandler::PickHandler(OSGWidget *parent, WindowsManagerPtr_t wsm)
      : QObject (parent)
      , wsm_ (wsm)
      , parent_ (parent)
      , pushed_ (false)
      , lastX_ (0)
      , lastY_ (0)
    {}

    PickHandler::~PickHandler()
    {
    }

    bool PickHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
    {
      switch (ea.getEventType()) {
        case osgGA::GUIEventAdapter::PUSH:
        case osgGA::GUIEventAdapter::RELEASE:
          if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) {
            if (pushed_
                && ea.getEventType() == osgGA::GUIEventAdapter::RELEASE) {
              pushed_ = false;
              if ((int)floor(lastX_ - ea.getX()+0.05) == 0
                  && (int)floor(lastY_ - ea.getY() + 0.5) == 0) {
                computeIntersection(aa, ea.getX(), ea.getY(), ea.getModKeyMask());
                return true;
              }
            }
            if (ea.getEventType() == osgGA::GUIEventAdapter::PUSH) {
              lastX_ = ea.getX();
              lastY_ = ea.getY();
              pushed_ = true;
            }
          }
          return false;
          break;
        case osgGA::GUIEventAdapter::KEYUP:
          switch (ea.getKey ()) {
            case 'z':
                setCameraToSelected (aa, false);
                return true;
            case 'Z':
                setCameraToSelected (aa, true);
              return true;
            default:
              break;
          }
          break;
        default:
          break;
      }
      return false;
    }

    void PickHandler::getUsage (osg::ApplicationUsage& usage)
    {
      usage.addKeyboardMouseBinding ("Right click", "Select node");
      usage.addKeyboardMouseBinding ('z', "Move camera on selected node");
      usage.addKeyboardMouseBinding ('Z', "Move and zoom on selected node");
    }

    std::list<graphics::NodePtr_t> PickHandler::computeIntersection(osgGA::GUIActionAdapter &aa,
                                                                    const float &x, const float &y,
								    int modKeyMask)
    {
      std::list<graphics::NodePtr_t> nodes;
      osgViewer::View* viewer = dynamic_cast<osgViewer::View*>( &aa );
      if( viewer )
      {
          // There is no need to lock the windows manager mutex
          // as this is treated in the event loop of OSG, and not Qt.
          // On the contrary, locking here creates a deadlock as the lock is
          // already acquired by OSGWidget::paintEvent.
          // wsm_->lock().lock();
          osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
              new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, x, y);
          intersector->setIntersectionLimit( osgUtil::Intersector::LIMIT_NEAREST );

          osgUtil::IntersectionVisitor iv( intersector );

          osg::Camera* camera = viewer->getCamera();
          camera->accept( iv );

          if( !intersector->containsIntersections() ) {
            parent_->emitClicked(new SelectionEvent(SelectionEvent::FromOsgWindow, QApplication::keyboardModifiers()));
            return nodes;
          }

          // Only one intersection. Otherwise, one has to loop on elements of
          // intersector->getIntersections();
          const osgUtil::LineSegmentIntersector::Intersection&
            intersection = intersector->getFirstIntersection();
          for (int i = (int) intersection.nodePath.size()-1; i >= 0 ; --i) {
            graphics::NodePtr_t n = wsm_->getNode(intersection.nodePath[i]->getName ());
            if (n) {
              if (boost::regex_match (n->getID(), boost::regex ("^.*_[0-9]+$")))
                continue;
              SelectionEvent *event = new SelectionEvent(SelectionEvent::FromOsgWindow,
                  n,
                  mapper_.getQtModKey(modKeyMask));
              event->setupIntersection(intersection);
              parent_->emitClicked(event);
              return nodes;
            }
          }
        }
      parent_->emitClicked(new SelectionEvent(SelectionEvent::FromOsgWindow, QApplication::keyboardModifiers()));
      return nodes;
    }

    void PickHandler::setCameraToSelected (osgGA::GUIActionAdapter &aa,
        bool zoom)
    {
      MainWindow* main = MainWindow::instance();
      graphics::NodePtr_t last = main->osg()->getNode(
            main->selectionHandler()->mode()->currentBody().toStdString()
            );
      if (!last) return;
      osgViewer::View* viewer = dynamic_cast<osgViewer::View*>( &aa );
      if(!viewer) return;

      const osg::BoundingSphere& bs = last->asGroup()->getBound ();
      osg::Vec3f eye, center, up;
      viewer->getCameraManipulator()->getInverseMatrix ()
        .getLookAt (eye, center, up);
      if (zoom) {
        eye.normalize();
        eye *= 1 + bs.radius ();
      }
      viewer->getCameraManipulator()->setByInverseMatrix (
          osg::Matrixd::lookAt (eye, bs.center(), up)
          );
    }
  }
}
