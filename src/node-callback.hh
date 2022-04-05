#include <QObject>
#include <gepetto/gui/selection-event.hh>
#include <gepetto/viewer/corba/graphical-interface.hh>

namespace gepetto {
namespace viewer {
namespace corba {

class NodeCallback : public QObject {
  Q_OBJECT

 public:
  typedef gui::SelectionEvent SelectionEvent;

  NodeCallback(QObject* parent, corbaserver::NodeCallback_var cb);

 public slots:
  void selected(SelectionEvent* event);

 private:
  corbaserver::NodeCallback_var cb_;
};

}  // namespace corba
}  // namespace viewer
}  // namespace gepetto
