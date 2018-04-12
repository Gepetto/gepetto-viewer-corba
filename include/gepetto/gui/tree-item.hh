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

#ifndef GEPETTO_GUI_TREEITEM_HH
#define GEPETTO_GUI_TREEITEM_HH

#include <QStandardItem>
#include <QItemDelegate>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QMenu>
#include <QPushButton>
#include <QSignalMapper>

#include <gepetto/gui/fwd.hh>
#include <gepetto/viewer/node.h>

namespace gepetto {
  namespace gui {
    class BodyTreeItem : public QObject, public QStandardItem
    {
      Q_OBJECT

      public:
        BodyTreeItem (QObject* parent, graphics::NodePtr_t node);

        virtual QStandardItem* clone () const;

        virtual int type() {
          return QStandardItem::UserType+1;
        }

        graphics::NodePtr_t node () const;

        void populateContextMenu (QMenu* menu);

        void setParentGroup (const std::string& parent);

        virtual ~BodyTreeItem() {};

      public:
        void attachToWindow (unsigned int windowID);

        public slots:
          void setViewingMode (QString mode);
        void setVisibilityMode (QString mode);
        void removeFromGroup ();
        void remove ();
        void removeAll ();
        void addLandmark ();
        void deleteLandmark ();
        QString text () const { return QStandardItem::text(); }

      private:
        graphics::NodePtr_t node_;
        std::string parentGroup_;

        QSignalMapper vmMapper_;
        QSignalMapper vizMapper_;

        friend class VisibilityItem;
    };
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_TREEITEM_HH
