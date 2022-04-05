#include "node-callback.hh"

#include <gepetto/viewer/node.h>

#include <gepetto/gui/bodytreewidget.hh>
#include <gepetto/gui/mainwindow.hh>

#include "conversions.hh"

namespace gepetto {
namespace viewer {
namespace corba {
using gui::MainWindow;
using gui::SelectionEvent;

NodeCallback::NodeCallback(QObject* parent, corbaserver::NodeCallback_var cb)
    : QObject(), cb_(cb) {
  moveToThread(parent->thread());
  setParent(parent);

  MainWindow* main = MainWindow::instance();
  connect(main->bodyTree(), SIGNAL(bodySelected(SelectionEvent*)),
          SLOT(selected(SelectionEvent*)), Qt::QueuedConnection);
}

void NodeCallback::selected(SelectionEvent* event) {
  NodePtr_t node = event->node();
  try {
    corbaserver::Position_slice* pos = corbaserver::Position_alloc();
    corbaserver::Position_slice* nor = corbaserver::Position_alloc();
    if (event->hasIntersection()) {
      QVector3D v = event->point(false);
      pos[0] = (float)v.x();
      pos[1] = (float)v.y();
      pos[2] = (float)v.z();
      v = event->normal(false);
      nor[0] = (float)v.x();
      nor[1] = (float)v.y();
      nor[2] = (float)v.z();
    } else {
      pos[0] = pos[1] = pos[2] = 0;
      nor[0] = nor[1] = nor[2] = 0;
    }
    if (node)
      cb_->selected(node->getID().c_str(), pos, nor);
    else
      cb_->selected("", pos, nor);
  } catch (const gepetto::Error& e) {
    qDebug() << e.msg;
  } catch (const CORBA::Exception& e) {
    qDebug() << "CORBA Exception in NodeCallback::selected" << e._name() << "-"
             << e._rep_id();
    qDebug() << "disconnecting NodeCallback.";
    deleteLater();
  }
  event->done();
}

}  // namespace corba
}  // namespace viewer
}  // namespace gepetto
