#include <gepetto/gui/action-search-bar.hh>

#include <QDebug>

#include <QAction>
#include <QKeyEvent>
#include <QCompleter>

namespace gepetto {
  namespace gui {
    ActionSearchBar::ActionSearchBar (QWidget* parent)
      : QLineEdit(parent)
      , model_ (new QStringListModel(this))
      , completer_ (new QCompleter(model_, this))
      , showAction_ (new QAction("Open action search bar popup", this))
    {
      setPlaceholderText("Type here");
      setCompleter(completer_);
      completer_->setCaseSensitivity(Qt::CaseInsensitive);
      // completer_->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
      showAction_->setShortcut(Qt::Key_M);
      showAction_->setShortcutContext(Qt::WidgetWithChildrenShortcut);

      connect(this, SIGNAL(returnPressed()), SLOT(handleReturnPressed()));
      connect(completer_, SIGNAL(activated(QString)), SLOT(trigger(QString)));
      connect(showAction_, SIGNAL(triggered()), SLOT(show()));
    }

    void ActionSearchBar::addAction (QAction* action)
    {
      actions_[action->text()] = action;
      model_->setStringList(
          model_->stringList() << action->text()
          );
    }

    void ActionSearchBar::keyPressEvent ( QKeyEvent* event )
    {
      switch (event->key()) {
        case Qt::Key_Escape:
          close();
          return;
        case Qt::Key_Enter:
          handleReturnPressed();
          return;
        default:
          QLineEdit::keyPressEvent(event);
          return;
      }
    }

    void ActionSearchBar::showEvent ( QShowEvent* )
    {
      move(QCursor::pos());
      selectAll();
      // completer_->complete();
      // setFocus(Qt::OtherFocusReason);
    }

    bool ActionSearchBar::trigger ( const QString& s )
    {
      // qDebug() << "Try action " << s;
      if (!actions_.contains(s)) return false;
      actions_[s]->trigger();
      // qDebug() << "Action " << s;
      close();
      return true;
    }

    void ActionSearchBar::handleReturnPressed ()
    {
      if (!trigger(text())) {
        if (completer_->completionCount() > 0) {
          if (trigger(completer_->currentCompletion()))
            return;
        }
      }
      close();
    }
  }
}
