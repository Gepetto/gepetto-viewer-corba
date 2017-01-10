#ifndef GEPETTO_GUI_SELECTION_HANDLER_H__
# define GEPETTO_GUI_SELECTION_HANDLER_H__

#include <QWidget>
#include <QComboBox>
#include <QVector3D>
#include <QKeyEvent>

#include <gepetto/gui/fwd.hh>

namespace gepetto {
  namespace gui {
    /// Base class to define selection mode.
    /// \note The class has no pure virtual method in order to be used in python.
    class SelectionMode : public QObject
    {
      Q_OBJECT
    public:
      SelectionMode(WindowsManagerPtr_t wsm) 
	: wsm_(wsm)
      {}
      ~SelectionMode() {}

      virtual void reset() { currentSelected_ = ""; }

      QString currentBody () { return currentSelected_; }

    signals:
      void selectedBodies(QStringList selectedBodies);

    public slots:
      /// Slot called when a body is selected.
      /// \param name body's name
      /// \param position click position
      virtual void onSelect(SelectionEvent* event) = 0;

      virtual QString getName() { return "None"; }

    protected:
      QString currentSelected_;
      WindowsManagerPtr_t wsm_;
    };

    /// Class that allows to select one body.
    class UniqueSelection : public SelectionMode
    {
      Q_OBJECT
    public:
      UniqueSelection(WindowsManagerPtr_t wsm);
      ~UniqueSelection();

    public slots:
      virtual void onSelect(SelectionEvent* event);
      virtual QString getName() { return "Unique"; }
    };

    class MultiSelection : public SelectionMode
    {
      Q_OBJECT
    public:
      MultiSelection(WindowsManagerPtr_t wsm);
      ~MultiSelection();

      virtual void reset();

    public slots:
      virtual void onSelect(SelectionEvent* event);
      virtual QString getName() { return "Multi"; }

    protected:
      QStringList selectedBodies_;
    };

    class SelectionHandler : public QComboBox
    {
      Q_OBJECT
    public:
      SelectionHandler(WindowsManagerPtr_t wsm, QWidget* parent = 0);
      ~SelectionHandler();

      void setParentOSG(OSGWidget* parent);

      SelectionMode* mode ();

    public slots:
      /// Add a mode to the list of available mode.
      /// \param mode mode to add
      void addMode(SelectionMode *mode);

    private slots:
      void getBodies(QStringList selectedBodies);
      void changeMode(int index);

    private:
      void initWidget();

      std::vector<SelectionMode *> modes_;
      OSGWidget* osg_;
      int index_;
      WindowsManagerPtr_t wsm_;
      QStringList selected_;
    };
  }
}

#endif /* GEPETTO_GUI_SELECTION_HANDLER_H__ */
