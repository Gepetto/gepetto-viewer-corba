#ifndef GEPETTO_GUI_ACTION_SEARCH_BAR_HH
#define GEPETTO_GUI_ACTION_SEARCH_BAR_HH

#include <QMap>
#include <QString>
#include <QLineEdit>
#include <QStringListModel>

#include <gepetto/gui/fwd.hh>

namespace gepetto {
  namespace gui {
    class ActionSearchBar : public QLineEdit
    {
      Q_OBJECT

    public:
      ActionSearchBar (QWidget* parent);

      void addAction (QAction* action);

      QAction* showAction() const { return showAction_; }

    protected:
      virtual void keyPressEvent ( QKeyEvent* event );
      virtual void showEvent ( QShowEvent* event );

    private slots:
      bool trigger(const QString& s);
      void handleReturnPressed();

    private:
      QStringListModel* model_;
      QCompleter* completer_;
      QMap<QString, QAction*> actions_;
      QAction* showAction_;
    };
  }
}

#endif // GEPETTO_GUI_ACTION_SEARCH_BAR_HH
