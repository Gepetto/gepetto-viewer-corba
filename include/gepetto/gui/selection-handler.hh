#ifndef GEPETTO_GUI_SELECTION_HANDLER_H__
# define GEPETTO_GUI_SELECTION_HANDLER_H__

#include <QWidget>
#include <QComboBox>
#include <QVector3D>

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

    signals:
      void selectedBodies(QStringList selectedBodies);

    public slots:
      /// Slot called when a body is selected.
      /// \param name body's name
      /// \param position click position
      virtual void onSelect(QString name, QVector3D position) { Q_UNUSED(name)emit selectedBodies(QStringList());}

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
      virtual void onSelect(QString name, QVector3D position);
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
      virtual void onSelect(QString name, QVector3D position);
      virtual QString getName() { return "Multi"; }

    protected:
      QStringList selectedBodies_;
    };

    class SelectionHandler : public QWidget
    {
      Q_OBJECT
    public:
      SelectionHandler(WindowsManagerPtr_t wsm, OSGWidget* parent);
      ~SelectionHandler();

    public slots:
      /// Add a mode to the list of available mode.
      /// \param mode mode to add
      void addMode(SelectionMode *mode);

    private slots:
      void getBodies(QStringList selectedBodies);
      void changeMode();

    private:
      void initWidget();

      std::vector<SelectionMode *> modes_;
      OSGWidget* osg_;
      WindowsManagerPtr_t wsm_;
      int index_;
      QStringList selected_;
    };
  }
}

#endif /* GEPETTO_GUI_SELECTION_HANDLER_H__ */
