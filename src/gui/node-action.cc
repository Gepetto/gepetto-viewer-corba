// Copyright (c) 2017, Joseph Mirabel
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
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// gepetto-viewer-corba. If not, see <http://www.gnu.org/licenses/>.

#include <gepetto/gui/node-action.hh>

#include <gepetto/gui/mainwindow.hh>
#include <gepetto/gui/selection-handler.hh>
#include <gepetto/gui/windows-manager.hh>

namespace gepetto {
  namespace gui {
    NodeActionBase::NodeActionBase(const QString& text, graphics::NodePtr_t node, QWidget* parent)
      : QAction (text, parent)
      , node_ (node)
    {
      connect(this, SIGNAL(triggered(bool)), SLOT(_act(bool)));
    }

    graphics::NodePtr_t NodeActionBase::node () const
    {
      if (node_) return node_;
      QString s = MainWindow::instance()->selectionHandler()->mode()->currentBody();
      return MainWindow::instance()->osg()->getNode(s.toStdString(), false);
    }

    void NodeActionBase::_act (bool checked)
    {
      act(checked);
    }

    NodeAction::NodeAction(const NodeAction::Type& t, const QString& text, graphics::NodePtr_t node, QWidget* parent)
      : NodeActionBase (text, node, parent)
      , type_ (t)
    {}

    NodeAction::NodeAction(const NodeAction::Type& t, const QString& text, QWidget* parent)
      : NodeActionBase (text, graphics::NodePtr_t(), parent)
      , type_ (t)
    {}

    void NodeAction::act(bool)
    {
      graphics::NodePtr_t n = node();
      if (!n) return;
      switch (type_) {
        case VISIBILITY_ON:
          n->setVisibilityMode(graphics::VISIBILITY_ON);
          break;
        case VISIBILITY_OFF:
          n->setVisibilityMode(graphics::VISIBILITY_OFF);
          break;
        case ALWAYS_ON_TOP:
          n->setVisibilityMode(graphics::ALWAYS_ON_TOP);
          break;
      }
    }
  }
}
