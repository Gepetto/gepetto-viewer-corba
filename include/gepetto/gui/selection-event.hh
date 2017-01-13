#ifndef GEPETTO_GUI_SELECTION_EVENT_HH
#define GEPETTO_GUI_SELECTION_EVENT_HH

#include <QString>
#include <QVector3D>
#include <QObject>
#include <QAtomicInt>

#include <gepetto/viewer/node.h>

namespace gepetto {
  namespace gui {
    class SelectionEvent : public QObject {
      Q_OBJECT

      public:
        enum Type {
          FromOsgWindow,
          FromBodyTree
        };

        SelectionEvent (const Type& t, graphics::NodePtr_t node = graphics::NodePtr_t(), Qt::KeyboardModifiers modKey = Qt::NoModifier)
          : type_ (t)
          , node_ (node)
          , modKey_ (modKey)
          , hasIntersection_ (false)
          , c_ (-1)
        {
          if (node)
            nodeName_ = QString::fromStdString(node->getID());
        }

        SelectionEvent (const Type& t, Qt::KeyboardModifiers modKey)
          : type_ (t)
          , modKey_ (modKey)
          , hasIntersection_ (false)
          , c_ (-1)
        {}

        void setupIntersection(const osgUtil::LineSegmentIntersector::Intersection& it);

        const graphics::NodePtr_t& node () const { return node_; }

        void modKey (const Qt::KeyboardModifiers& m) { modKey_ = m; }

        /// Set this to the number of slots that will receive the event.
        /// \warning Not thread-safe
        void setCounter(int c) { c_ = c; }

      public slots:
        Type                  type    () const { return type_; }
        QString               nodeName() const { return nodeName_; }
        Qt::KeyboardModifiers modKey  () const { return modKey_; }

        bool hasIntersection () { return hasIntersection_; }
        QVector3D normal(bool local) const { return (local ? localNormal_ : worldNormal_); }
        QVector3D point (bool local) const { return (local ? localPoint_  : worldPoint_ ); }
        /// User must call this in slots using SelectionEvent.
        /// This decreases the internal counter and destroys the object when it
        /// reaches zero.
        /// This is thread-safe.
        void done ();

    private:
        Type type_;
        QString nodeName_;
        graphics::NodePtr_t node_;
        Qt::KeyboardModifiers modKey_;

        bool hasIntersection_;
        QVector3D localPoint_, localNormal_, worldPoint_, worldNormal_;
        QAtomicInt c_;
    };
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_SELECTION_EVENT_HH
