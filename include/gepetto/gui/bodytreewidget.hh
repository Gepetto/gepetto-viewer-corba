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

#ifndef GEPETTO_GUI_BODYTREEWIDGET_HH
#define GEPETTO_GUI_BODYTREEWIDGET_HH

// This does not work because of qt meta-object compiler
#define GEPETTO_GUI_BODYTREE_DECL_FEATURE(func, ArgType) \
  public slots: \
    void func (ArgType arg)
#define GEPETTO_GUI_BODYTREE_IMPL_FEATURE(func, ArgType, OutType, WindowsManagerFunc) \
  void BodyTreeWidget::func (ArgType arg) { \
    WindowsManagerPtr_t wsm = MainWindow::instance()->osg(); \
    foreach (const QModelIndex& index, view_->selectionModel ()->selectedIndexes ()) { \
      const BodyTreeItem *item = dynamic_cast <const BodyTreeItem*> \
        (model_->itemFromIndex (index)); \
      if (item) wsm->WindowsManagerFunc (item->node()->getID(), \
                                         convertTo<OutType>::from(arg)); \
      else \
        qDebug() << model_->itemFromIndex(index)->text() << "is not a BodyTreeItem"; \
    } \
  }

#include <QWidget>
#include <QTreeView>
#include <QToolBox>
#include <QStandardItemModel>
#include <QVector3D>

#include <gepetto/viewer/group-node.h>

#include <gepetto/gui/fwd.hh>

namespace gepetto {
  namespace gui {
    /// Contains the list of all the bodies in the scene.
    class BodyTreeWidget : public QWidget
    {
      Q_OBJECT

    public:
      explicit BodyTreeWidget (QWidget* parent = NULL)
        : QWidget (parent)
      {}

      /// Init the widget.
      /// \param view tree view to display.
      /// \param toolBox menu in the window
      void init(QTreeView *view, QToolBox* toolBox);

      virtual ~BodyTreeWidget () {}

      /// Display the value in the slider.
      /// \param alpha alpha value to convert
      void changeAlphaValue(const float& alpha);

      /// Get the body tree view.
      QTreeView* view ();

      QStandardItemModel* model()
      {
        return model_;
      }

      void emitBodySelected (SelectionEvent* event);

    signals:
      void bodySelected (SelectionEvent* event);

    public slots:
      /// \ingroup plugin_python
      /// \{

      /// Triggered when an item is selected in the body tree view.
      /// \param bodyName name of the body
      void selectBodyByName (const QString bodyName);

      /// Triggered when an item is selected in the body tree view.
      /// \param bodyName name of the body
      void selectBodyByName (const std::string& bodyName);

      /// Get selected bodies
      QList<BodyTreeItem*> selectedBodies() const;

      /// Set the transparency of currently selected body.
      /// \param value value of the slider to convert
      void setTransparency(int value);

      /// Set the visibility mode of currently selected body.
      /// \param arg visibility mode
      void setVisibilityMode (QString arg);

      /// Set the wireframe mode of currently selected body.
      /// \param arg wireframe mode
      void setWireFrameMode (QString arg);

      /// Set the color of currently selected body.
      /// \param color new color of the body
      void setColor (QColor color);

      /// Set the scale of currently selected body.
      /// \param scale new scale of the body
      void setScale (int scale);

      /// \}

    protected slots:
      /// Display the context menu for one item.
      /// \param pos position of the item
      void customContextMenu (const QPoint& pos);

      void currentChanged (const QModelIndex &current,
                           const QModelIndex &previous);

    private:
      /// Handle a selection event
      ///
      /// Does not re-emit a selection event when the body tree selection
      /// is updated.
      void handleSelectionEvent (const SelectionEvent* event);

      QTreeView* view_;
      QStandardItemModel* model_;
      WindowsManagerPtr_t osg_;
      QToolBox* toolBox_;
    };
  }
}

#endif // GEPETTO_GUI_BODYTREEWIDGET_HH
