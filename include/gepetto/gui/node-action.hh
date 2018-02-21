#ifndef GEPETTO_GUI_NODE_ACTION_HH
#define GEPETTO_GUI_NODE_ACTION_HH

#include <QAction>

#include <gepetto/viewer/node.h>
#include <gepetto/gui/fwd.hh>

namespace gepetto {
  namespace gui {
    class NodeActionBase : public QAction {
      Q_OBJECT

    public:
        NodeActionBase(const QString& text, graphics::NodePtr_t node, QWidget* parent);

        virtual ~NodeActionBase () {}

        graphics::NodePtr_t node () const;

    protected:
      virtual void act(bool checked) = 0;

    private slots:
      void _act(bool checked);

    private:
      graphics::NodePtr_t node_;
    };

    class NodeAction : public NodeActionBase {
      public:
        enum Type {
          VISIBILITY_ON,
          VISIBILITY_OFF,
          ALWAYS_ON_TOP,
          ATTACH_TO_WINDOW,
          ATTACH_CAMERA_TO_NODE
        };

        NodeAction(const Type& t, const QString& text, graphics::NodePtr_t node, QWidget* parent);

        NodeAction(const Type& t, const QString& text, QWidget* parent);

        /// Attach to window
        NodeAction(const QString& text, graphics::NodePtr_t node, OSGWidget* window, QWidget* parent);

        /// Attach camera to node
        NodeAction(const QString& text, OSGWidget* window, QWidget* parent);

        virtual ~NodeAction () {}

      protected:
        void act(bool checked);

      private:
        const Type type_;
        OSGWidget* window_;
    };
  }
}

#endif // GEPETTO_GUI_NODE_ACTION_HH
