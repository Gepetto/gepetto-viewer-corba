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
    QWidget* boolPropertyEditor (BodyTreeItem* bti, const graphics::PropertyPtr_t prop)
    {
      QCheckBox* cb = new QCheckBox;
      bool value;
      /* bool success = */ prop->get(value);
      cb->setChecked(value);
      if (prop->hasWriteAccess())
        bti->connect(cb, SIGNAL(toggled(bool)), SLOT(setBoolProperty(bool)));
      else
        cb->setEnabled(false);
      return cb;
    }

    QWidget* enumPropertyEditor (BodyTreeItem* bti, const graphics::PropertyPtr_t prop)
    {
      const graphics::EnumProperty::Ptr_t enumProp = boost::dynamic_pointer_cast<graphics::EnumProperty> (prop);
      const graphics::MetaEnum* enumMeta = enumProp->metaEnum();

      QComboBox* cb = new QComboBox;
      int value;
      /* bool success = */ enumProp->get(value);
      int indexSelected = 0;
      for (std::size_t i = 0; i < enumMeta->values.size(); ++i)
      {
        cb->addItem(enumMeta->names[i].c_str(), enumMeta->values[i]);
        if (value == enumMeta->values[i]) indexSelected = i;
      }
      cb->setCurrentIndex(indexSelected);
      if (prop->hasWriteAccess())
        bti->connect(cb, SIGNAL(currentIndexChanged(int)), SLOT(setIntProperty(int)));
      else
        cb->setEnabled(false);
      return cb;
    }

    QWidget* stringPropertyEditor (BodyTreeItem* bti, const graphics::PropertyPtr_t prop)
    {
      QLineEdit* le = new QLineEdit;
      std::string value;
      /* bool success = */ prop->get(value);
      le->setText(QString::fromStdString(value));
      if (prop->hasWriteAccess())
        bti->connect(le, SIGNAL(textChanged(QString)), SLOT(setStringProperty(QString)));
      else
        le->setReadOnly(true);
      return le;
    }

    QWidget* floatPropertyEditor (BodyTreeItem* bti, const graphics::PropertyPtr_t prop)
    {
      QDoubleSpinBox* dsb = new QDoubleSpinBox;
      float value;
      /* bool success = */ prop->get(value);
      dsb->setValue(value);
      if (prop->hasWriteAccess())
        bti->connect(dsb, SIGNAL(valueChanged(double)), SLOT(setFloatProperty(double)));
      else
        dsb->setEnabled(false);
      return dsb;
    }

    BodyTreeItem::BodyTreeItem(QObject *parent, graphics::NodePtr_t node) :
      QObject (parent),
      QStandardItem (QString (node->getID().c_str())),
      node_ (node)
    {
      setEditable(false);

      const std::string & name = node->getID();
      QStandardItem::setText(name.substr(name.find_last_of("/") + 1).c_str());
    }

    void BodyTreeItem::initialize ()
    {
      connect(this, SIGNAL(requestInitialize()), SLOT(doInitialize()));
      emit requestInitialize();
    }

    void BodyTreeItem::doInitialize ()
    {
      propertyEditors_ = new QWidget();
      BodyTreeWidget* bt = MainWindow::instance()->bodyTree();
      if (propertyEditors_->thread() != bt->thread())
        propertyEditors_->moveToThread(bt->thread());
      QFormLayout* l = new QFormLayout(propertyEditors_);

      l->addRow("Node name:", new QLabel (node_->getID().c_str()));

      const graphics::PropertyMap_t& props = node_->properties();
      for (graphics::PropertyMap_t::const_iterator _prop = props.begin();
           _prop != props.end(); ++_prop)
      {
        const graphics::PropertyPtr_t prop = _prop->second;
        if (!prop->hasReadAccess()) continue;

        QString name = _prop->first.c_str();
        QWidget* field = NULL;
        if (prop->type() == "enum") {
          field = enumPropertyEditor(this, prop);
        } else if (prop->type() == "bool") {
          field = boolPropertyEditor(this, prop);
        } else if (prop->type() == "string") {
          field = stringPropertyEditor(this, prop);
        } else if (prop->type() == "float") {
          field = floatPropertyEditor(this, prop);
        } else {
          qDebug() << "Unhandled property" << name << "of type" << prop->type().c_str() << ".";
        }
        if (field != NULL) {
          field->setProperty("propertyName", name);
          l->addRow(name + ':', field);
        }
      }
      disconnect(SIGNAL(requestInitialize()));
    }

    template <typename T>
    void BodyTreeItem::setProperty (const QObject* sender, const T& value) const
    {
      if (sender != NULL) {
        QVariant nameVariant = sender->property("propertyName");
        if (nameVariant.isValid()) {
          std::string name = nameVariant.toString().toStdString();
          boost::mutex::scoped_lock lock (MainWindow::instance()->osg()->osgFrameMutex());
          node_->setProperty<T>(name, value);
        }
      }
    }

    void BodyTreeItem::setBoolProperty (bool value) const
    {
      setProperty (QObject::sender(), value);
    }

    void BodyTreeItem::setIntProperty (int value) const
    {
      setProperty (QObject::sender(), value);
    }

    void BodyTreeItem::setStringProperty (const QString& value) const
    {
      setProperty (QObject::sender(), value.toStdString());
    }

    void BodyTreeItem::setFloatProperty (const double& value) const
    {
      setProperty (QObject::sender(), float(value));
    }

    BodyTreeItem::~BodyTreeItem()
    {
      if (propertyEditors_->parent() != NULL)
        delete propertyEditors_;
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
