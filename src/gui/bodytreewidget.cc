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

#include <gepetto/gui/bodytreewidget.hh>

#include <gepetto/gui/tree-item.hh>
#include <gepetto/gui/mainwindow.hh>
#include <gepetto/gui/windows-manager.hh>
#include <gepetto/gui/osgwidget.hh>
#include <gepetto/gui/meta.hh>
#include <gepetto/gui/selection-event.hh>
#include <gepetto/gui/node-action.hh>

#include <QSignalMapper>
#include <QColorDialog>
#include <QHBoxLayout>
#include <QApplication>

/*
static void addColorSelector (QToolBox* tb, QString title, QObject* receiver, const char* slot) {
  QWidget* newW = new QWidget();
  newW->setObjectName(title);
  QHBoxLayout* layout = new QHBoxLayout();
  newW->setLayout(layout);
  layout->setSpacing(6);
  layout->setContentsMargins(11, 11, 11, 11);
  layout->setObjectName(title + "_layout");
  QPushButton* button = new QPushButton("Select color", newW);
  button->setObjectName(title + "_buttonSelect");
  layout->addWidget (button);

  QColorDialog* colorDialog = new QColorDialog(newW);
  colorDialog->setObjectName(title + "_colorDialog");
  colorDialog->setOption(QColorDialog::ShowAlphaChannel, true);

  colorDialog->connect(button, SIGNAL(clicked()), SLOT(open()));
  receiver->connect (colorDialog, SIGNAL(colorSelected(QColor)), slot);
  tb->addItem(newW, title);
}
*/

namespace gepetto {
  namespace gui {
    void BodyTreeWidget::init(QTreeView* view, QWidget *propertyArea)
    {
      MainWindow* main = MainWindow::instance();
      osg_ = main->osg();
      view_ = view;
      propertyArea_ = propertyArea;
      model_  = new QStandardItemModel (this);
      view_->setModel(model_);
      view_->setSelectionMode(QAbstractItemView::ExtendedSelection);

      connect (view_, SIGNAL (customContextMenuRequested(QPoint)), SLOT(customContextMenu(QPoint)));
      connect (view_->selectionModel(),
          SIGNAL (currentChanged(QModelIndex,QModelIndex)),
          SLOT (currentChanged(QModelIndex,QModelIndex)));

/*
      addSlider(toolBox_, "Transparency", this, SLOT(setTransparency(int)));
      addColorSelector(toolBox_, "Color", this, SLOT(setColor(QColor)));
      addSlider(toolBox_, "Scale", this, SLOT(setScale(int)));
*/
      propertyArea_->setLayout (new QVBoxLayout);
    }

    QTreeView* BodyTreeWidget::view ()
    {
      return view_;
    }

    void BodyTreeWidget::selectBodyByName(const QString bodyName)
    {
      QList<QStandardItem*> matches;
      if (!bodyName.isEmpty() && !bodyName.isNull()) {
        matches = model_->findItems(bodyName, Qt::MatchFixedString
            | Qt::MatchCaseSensitive
            | Qt::MatchRecursive);
      }
      if (matches.empty()) {
        qDebug () << "Body" << bodyName << "not found.";
        view_->clearSelection();
      } else {
        view_->setCurrentIndex(matches.first()->index());
      }
    }

    void BodyTreeWidget::selectBodyByName (const std::string& bodyName)
    {
      qDebug () << "Use QString instead of std::string";
      return selectBodyByName (QString::fromStdString (bodyName));
    }

    void BodyTreeWidget::handleSelectionEvent (const SelectionEvent* event)
    {
      disconnect (view_->selectionModel(),
          SIGNAL (currentChanged(QModelIndex,QModelIndex)),
          this, SLOT (currentChanged(QModelIndex,QModelIndex)));
      BodyTreeItem* item = NULL;
      if (event->node()) {
        WindowsManager::BodyTreeItems_t matches =
          osg_->bodyTreeItems(event->node()->getID());

        if (matches.empty())
          view_->clearSelection();
        else {
          item = matches[0];
          if (event->modKey() == Qt::ControlModifier)
            view_->selectionModel()->setCurrentIndex (item->index(),
                 QItemSelectionModel::Toggle);
          else
            view_->selectionModel()->select          (item->index(),
                QItemSelectionModel::ClearAndSelect);
          view_->scrollTo (matches[0]->index());
        }
      } else
        view_->clearSelection();
      updatePropertyArea(item);
      connect (view_->selectionModel(),
          SIGNAL (currentChanged(QModelIndex,QModelIndex)),
          SLOT (currentChanged(QModelIndex,QModelIndex)));
    }

    void BodyTreeWidget::emitBodySelected(SelectionEvent* event)
    {
      event->setCounter(receivers(SIGNAL(bodySelected(SelectionEvent*))) + 1);
      emit bodySelected (event);
      if (event->type() != SelectionEvent::FromBodyTree) {
        MainWindow* main = MainWindow::instance();
        handleSelectionEvent(event);
        main->requestSelectJointFromBodyName(event->nodeName());
      }
      event->done();
    }

    void BodyTreeWidget::updatePropertyArea (BodyTreeItem* item)
    {
      QLayoutItem *child;
      while ((child = propertyArea_->layout()->takeAt(0)) != 0) {
        if (child->widget() != NULL) {
          child->widget()->setParent(NULL);
        }
      }
      if (item != NULL) {
        propertyArea_->layout()->addWidget(item->propertyEditors());
      }
    }

    void BodyTreeWidget::currentChanged (const QModelIndex &current,
        const QModelIndex &/*previous*/)
    {
      // TODO
      // if (!current.isValid()) {
      // deselect
      // }
      BodyTreeItem *item = dynamic_cast <BodyTreeItem*> (
          qobject_cast <const QStandardItemModel*>
          (view_->model())->itemFromIndex(current)
         );
      updatePropertyArea(item);
      if (item) {
        SelectionEvent *event = new SelectionEvent(SelectionEvent::FromBodyTree, item->node(), QApplication::keyboardModifiers());
        emitBodySelected(event);
      }
    }

    QList<BodyTreeItem*> BodyTreeWidget::selectedBodies() const
    {
      QList<BodyTreeItem*> list;
      foreach (const QModelIndex& index, view_->selectionModel ()->selectedIndexes ()) {
        BodyTreeItem *item = dynamic_cast <BodyTreeItem*>
          (model_->itemFromIndex (index));
        if (item) list.push_back(item);
      }
      return list;
    }

    void BodyTreeWidget::customContextMenu(const QPoint &pos)
    {
      QModelIndex index = view_->indexAt(pos);
      if(index.isValid()) {
          BodyTreeItem *item = dynamic_cast <BodyTreeItem*>
              (model_->itemFromIndex(index));
          if (!item) return;
          MainWindow* main = MainWindow::instance ();
          QMenu contextMenu (tr("Node"));
          item->populateContextMenu (&contextMenu);
          QMenu* windows = contextMenu.addMenu(tr("Attach to window"));
          foreach (OSGWidget* w, main->osgWindows ()) {
              NodeAction* ja = new NodeAction (w->objectName(), item->node(), w, windows);
              windows->addAction (ja);
            }
          contextMenu.exec(view_->mapToGlobal(pos));
        }
    }
  }
}
