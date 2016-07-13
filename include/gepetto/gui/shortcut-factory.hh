#ifndef GEPETTO_GUI_SHORTCUT_FACTORY_HH_
# define GEPETTO_GUI_SHORTCUT_FACTORY_HH_

#include <QAction>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPushButton>

#include <map>
#include <utility>

namespace gepetto {
  namespace gui {
    class ShortcutFactory : public QObject {
      Q_OBJECT
    public:
      ShortcutFactory();
      ~ShortcutFactory();

    public:
      /// Add a binding to a given widget.
      /// If widgetName doesn't exists, then it will be add.
      /// If actionName exists, do nothing
      /// \param widgetName name of the corresponding widget
      /// \param actionName name of the action
      /// \param action instance of the action
      void addBinding(QString widgetName, QString actionName, QAction* action);

    public slots:
      /// Open the widget that allows to change the different shortcut
      void open();

    private:
      void writeShortcutsFile();
      void readShortcutsFile();

      typedef std::pair<QString, QAction*> Binding;
      typedef std::list<Binding> BindingList;
      typedef std::map<QString, BindingList> MapBindings;

      class ShortcutButton : public QPushButton {
      public:
	ShortcutButton(QAction* action, QWidget* parent = 0);
	~ShortcutButton();

      protected:
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);

	bool hasFocus_;
        QList<int> modifiers_;
	QAction* action_;
      };

      MapBindings widgetsBindings_;
      std::map<QString, QKeySequence> saved_;
    };
  }
}

#endif /* GEPETTO_GUI_SHORTCUT_FACTORY_HH_ */
