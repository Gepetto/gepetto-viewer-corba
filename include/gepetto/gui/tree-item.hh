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
