// Copyright (c) 2015-2018, LAAS-CNRS
// Authors: Joseph Mirabel (joseph.mirabel@laas.fr)
//
// This file is part of gepetto-viewer-corba.
// gepetto-viewer-corba is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// gepetto-viewer-corba is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Lesser Public License for more details. You should have
// received a copy of the GNU Lesser General Public License along with
// gepetto-viewer-corba. If not, see <http://www.gnu.org/licenses/>.

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

    void BodyTreeItem::setViewingMode(QString mode)
    {
      MainWindow::instance()->osg()->setWireFrameMode (node_->getID(),
          mode.toLocal8Bit().data());
    }

    void BodyTreeItem::setVisibilityMode(QString mode)
    {
      MainWindow::instance()->osg()->setVisibility (node_->getID(),
          mode.toLocal8Bit().data());
    }

    void BodyTreeItem::removeFromGroup()
    {
      if (parentGroup_.empty()) return;
      MainWindow::instance()->osg()->removeFromGroup (node_->getID(), parentGroup_);
      QStandardItem::parent()->removeRow(row());
    }

    void BodyTreeItem::removeAll()
    {
      MainWindow* main = MainWindow::instance();
      main->osg()->deleteNode(node_->getID(), true);
    }

    void BodyTreeItem::remove()
    {
      MainWindow* main = MainWindow::instance();
      main->osg()->deleteNode(node_->getID(), false);
    }

    void BodyTreeItem::addLandmark()
    {
      MainWindow::instance()->osg()->addLandmark(node_->getID(), 0.05f);
    }

    void BodyTreeItem::deleteLandmark()
    {
      MainWindow::instance()->osg()->deleteLandmark(node_->getID());
    }
  } // namespace gui
} // namespace gepetto
