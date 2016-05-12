#include "gepetto/gui/tree-item.hh"

#include <QDebug>

#include <gepetto/viewer/group-node.h>

#include "gepetto/gui/mainwindow.hh"
#include "gepetto/gui/windows-manager.hh"
#include <gepetto/gui/bodytreewidget.hh>

namespace gepetto {
  namespace gui {
    BodyTreeItem::BodyTreeItem(QObject *parent, graphics::NodePtr_t node) :
      QObject (parent),
      QStandardItem (QString (node->getID().c_str())),
      node_ (node),
      vmMapper_ (),
      vizMapper_ ()
    {
      init();
      setEditable(false);
      connect (&vmMapper_, SIGNAL (mapped (QString)), SLOT(setViewingMode(QString)));
      connect (&vizMapper_, SIGNAL (mapped (QString)), SLOT(setVisibilityMode(QString)));
    }

    QStandardItem* BodyTreeItem::clone() const
    {
      return new BodyTreeItem (QObject::parent(), node_);
    }

    graphics::NodePtr_t BodyTreeItem::node() const
    {
      return node_;
    }

    void BodyTreeItem::populateContextMenu(QMenu *contextMenu)
    {
      /// Landmark
      QMenu* lm = contextMenu->addMenu (tr("Landmark"));
      QAction* alm = lm->addAction (tr("Add"));
      QAction* dlm = lm->addAction (tr("Remove"));
      connect (alm, SIGNAL (triggered()), SLOT (addLandmark()));
      connect (dlm, SIGNAL (triggered()), SLOT (deleteLandmark()));
      /// Remove
      QAction* remove = contextMenu->addAction (tr("Remove"));
      connect (remove, SIGNAL (triggered()), SLOT (remove()));
      QAction* removeAll = contextMenu->addAction (tr("Remove all"));
      connect (removeAll, SIGNAL (triggered()), SLOT (removeAll()));
      if (!parentGroup_.empty()) {
        QAction* rfg = contextMenu->addAction (tr("Remove from group"));
        connect (rfg, SIGNAL (triggered()), SLOT (removeFromGroup ()));
      }
      /// Viewing mode
      QMenu* viewmode = contextMenu->addMenu(tr("Viewing mode"));
      QAction* f  = viewmode->addAction ("Fill");
      QAction* w  = viewmode->addAction ("Wireframe");
      QAction* fw = viewmode->addAction ("Fill and Wireframe");
      vmMapper_.setMapping (f , QString ("FILL"));
      vmMapper_.setMapping (w , QString ("WIREFRAME"));
      vmMapper_.setMapping (fw, QString ("FILL_AND_WIREFRAME"));
      connect (f , SIGNAL(triggered()), &vmMapper_, SLOT (map()));
      connect (w , SIGNAL(triggered()), &vmMapper_, SLOT (map()));
      connect (fw, SIGNAL(triggered()), &vmMapper_, SLOT (map()));
      /// Visibility mode
      QMenu* vizmode = contextMenu->addMenu(tr("Visibility mode"));
      QAction* on  = vizmode->addAction ("On");
      QAction* aot = vizmode->addAction ("Always on top");
      QAction* off = vizmode->addAction ("Off");
      vizMapper_.setMapping (on , QString ("ON"));
      vizMapper_.setMapping (aot, QString ("ALWAYS_ON_TOP"));
      vizMapper_.setMapping (off, QString ("OFF"));
      connect (on , SIGNAL(triggered()), &vizMapper_, SLOT (map()));
      connect (aot, SIGNAL(triggered()), &vizMapper_, SLOT (map()));
      connect (off, SIGNAL(triggered()), &vizMapper_, SLOT (map()));
    }

    void BodyTreeItem::setParentGroup(const std::string &parent)
    {
      parentGroup_ = parent;
    }

    void BodyTreeItem::init ()
    {
      graphics::GroupNodePtr_t gn = boost::dynamic_pointer_cast <graphics::GroupNode> (node_);
      if (gn) {
        for (size_t i = 0; i < gn->getNumOfChildren(); ++i) {
          BodyTreeItem* item = new BodyTreeItem (this, gn->getChild(i));
          item->setParentGroup (gn->getID());
          appendRow(item);
        }
      }
    }

    void BodyTreeItem::setViewingMode(QString mode)
    {
      MainWindow::instance()->osg()->setWireFrameMode (node_->getID().c_str(),
          mode.toLocal8Bit().data());
    }

    void BodyTreeItem::setVisibilityMode(QString mode)
    {
      MainWindow::instance()->osg()->setVisibility (node_->getID().c_str(),
          mode.toLocal8Bit().data());
    }

    void BodyTreeItem::attachToWindow(unsigned int windowID)
    {
      MainWindow::instance()->osg()->addSceneToWindow (node_->getID().c_str(), windowID);
    }

    void BodyTreeItem::removeFromGroup()
    {
      if (parentGroup_.empty()) return;
      MainWindow::instance()->osg()->removeFromGroup (node_->getID().c_str(),
          parentGroup_.c_str());
      QStandardItem::parent()->removeRow(row());
    }

    void BodyTreeItem::removeAll()
    {
      MainWindow* main = MainWindow::instance();
      main->osg()->deleteNode(node_->getID().c_str(), true);
      main->bodyTree()->reloadBodyTree();
    }

    void BodyTreeItem::remove()
    {
      MainWindow* main = MainWindow::instance();
      main->osg()->deleteNode(node_->getID().c_str(), false);
      main->bodyTree()->reloadBodyTree();
    }

    void BodyTreeItem::addLandmark()
    {
      MainWindow::instance()->osg()->addLandmark(node_->getID().c_str(), 0.05f);
    }

    void BodyTreeItem::deleteLandmark()
    {
      MainWindow::instance()->osg()->deleteLandmark(node_->getID().c_str());
    }
  } // namespace gui
} // namespace gepetto
