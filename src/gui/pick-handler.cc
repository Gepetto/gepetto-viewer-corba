#include "gepetto/gui/pick-handler.hh"

#include <QDebug>

#include <osg/io_utils>

#include <osgUtil/IntersectionVisitor>
#include <osgUtil/LineSegmentIntersector>

#include <osgViewer/Viewer>

#include <iostream>
#include <boost/regex.hpp>

#include <gepetto/gui/windows-manager.hh>
#include <gepetto/gui/osgwidget.hh>
#include <gepetto/gui/mainwindow.hh>
#include <gepetto/gui/bodytreewidget.hh>
#include <gepetto/gui/tree-item.hh>

namespace gepetto {
  namespace gui {
    PickHandler::PickHandler(OSGWidget *parent, WindowsManagerPtr_t wsm)
      : QObject (parent)
      , wsm_ (wsm)
      , parent_ (parent)
      , last_ ()
      , pushed_ (false)
      , lastX_ (0)
      , lastY_ (0)
    {
      MainWindow* main = MainWindow::instance ();
      connect (main->bodyTree ()->view ()->selectionModel(),
          SIGNAL (currentChanged(QModelIndex,QModelIndex)),
          SLOT (bodyTreeCurrentChanged(QModelIndex,QModelIndex)));
    }

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
                computeIntersection(aa, ea.getX(), ea.getY());
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

    void PickHandler::select (graphics::NodePtr_t node)
    {
      if (last_ == node) return;
      if (last_) last_->setHighlightState (0);
      last_ = node;
      if (last_) last_->setHighlightState (8);
    }

    void PickHandler::getUsage (osg::ApplicationUsage& usage)
    {
      usage.addKeyboardMouseBinding ("Right click", "Select node");
      usage.addKeyboardMouseBinding ('z', "Move camera on selected node");
      usage.addKeyboardMouseBinding ('Z', "Move and zoom on selected node");
    }

    std::list<graphics::NodePtr_t> PickHandler::computeIntersection(osgGA::GUIActionAdapter &aa,
                                                                    const float &x, const float &y)
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
          intersector->setIntersectionLimit( osgUtil::Intersector::LIMIT_ONE_PER_DRAWABLE );

          osgUtil::IntersectionVisitor iv( intersector );

          osg::Camera* camera = viewer->getCamera();

          camera->accept( iv );

          if( !intersector->containsIntersections() ) {
            select (graphics::NodePtr_t());
            return nodes;
          }

          osgUtil::LineSegmentIntersector::Intersections intersections = intersector->getIntersections();

          for(osgUtil::LineSegmentIntersector::Intersections::iterator it = intersections.begin();
              it != intersections.end(); ++it) {
              for (int i = (int) it->nodePath.size()-1; i >= 0 ; --i) {
                  graphics::NodePtr_t n = wsm_->getNode(it->nodePath[i]->getName ());
                  if (n) {
                      if (boost::regex_match (n->getID(), boost::regex ("^.*_[0-9]+$")))
                        continue;
                      osg::Vec3d p = it->getWorldIntersectPoint();
                      QVector3D pWF (p[0],p[1],p[2]);
                      parent_->emitClicked(QString::fromStdString(n->getID ()), pWF);
                      return nodes;
                      // nodes.push_back(n);
                      // break;
                    }
                }
            }
          // emit parent_->selected (nodes.front()->getID ());
        }
      select (graphics::NodePtr_t());
      return nodes;
    }

    void PickHandler::setCameraToSelected (osgGA::GUIActionAdapter &aa,
        bool zoom)
    {
      if (!last_) return;
      osgViewer::View* viewer = dynamic_cast<osgViewer::View*>( &aa );
      if(!viewer) return;

      const osg::BoundingSphere& bs = last_->asGroup()->getBound ();
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

    void PickHandler::bodyTreeCurrentChanged (const QModelIndex &current,
        const QModelIndex &/*previous*/)
    {
      BodyTreeItem *item = dynamic_cast <BodyTreeItem*> (
          qobject_cast <const QStandardItemModel*>
          (current.model())->itemFromIndex(current)
         );
      if (item) {
        wsm_->lock ().lock();
        select (item->node ());
        wsm_->lock ().unlock();
      }
    }
  }
}
