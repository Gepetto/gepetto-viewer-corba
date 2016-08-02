#include <QHBoxLayout>
#include <QLabel>
#include <QVector3D>
#include <QAction>

#include <gepetto/gui/windows-manager.hh>
#include "gepetto/gui/osgwidget.hh"
#include "gepetto/gui/selection-handler.hh"

namespace gepetto {
  namespace gui {
    SelectionHandler::SelectionHandler(WindowsManagerPtr_t wsm, OSGWidget *parent)
      : QWidget(parent),
	osg_(parent),
	index_(0),
	wsm_(wsm)
    {
      QHBoxLayout* layout = new QHBoxLayout(this);
      setMaximumHeight(0);

      QAction* changeMode = new QAction(this);
      changeMode->setShortcut(Qt::Key_U);
      connect(changeMode, SIGNAL(triggered()), SLOT(changeMode()));
      addAction(changeMode);
    }

    SelectionHandler::~SelectionHandler()
    {
    }

    void SelectionHandler::changeMode()
    {
      foreach(QString name, selected_) {
	wsm_->setHighlight(name.toStdString().c_str(), 0);	
      }
      modes_[index_]->reset();
      disconnect(osg_, SIGNAL(clicked(QString, QVector3D)),
		 modes_[index_], SLOT(onSelect(QString, QVector3D)));
      disconnect(modes_[index_], SIGNAL(selectedBodies(QStringList)),
		 this, SLOT(getBodies(QStringList)));
      index_ = (index_ + 1) % modes_.size();
      connect(osg_, SIGNAL(clicked(QString, QVector3D)),
	      modes_[index_], SLOT(onSelect(QString, QVector3D)));
      connect(modes_[index_], SIGNAL(selectedBodies(QStringList)),
	      SLOT(getBodies(QStringList)));
    }

    void SelectionHandler::addMode(SelectionMode* mode)
    {
      modes_.push_back(mode);
    }

    void SelectionHandler::getBodies(QStringList selectedBodies)
    {
      selected_ = selectedBodies;
    }

    UniqueSelection::UniqueSelection(WindowsManagerPtr_t wsm)
      : SelectionMode(wsm)
    {
    }

    UniqueSelection::~UniqueSelection()
    {
    }

    void UniqueSelection::onSelect(QString name, QVector3D position)
    {
      if (currentSelected_ == name) return;
      if (currentSelected_ != "") wsm_->setHighlight(currentSelected_.toStdString().c_str(), 0);
      currentSelected_ = name;
      wsm_->setHighlight(name.toStdString().c_str(), 8);
      emit selectedBodies(QStringList() << name);
    }

    MultiSelection::MultiSelection(WindowsManagerPtr_t wsm)
      : SelectionMode(wsm)
    {
    }

    MultiSelection::~MultiSelection()
    {
    }

    void MultiSelection::reset()
    {
      SelectionMode::reset();
      selectedBodies_ = QStringList();
    }

    void MultiSelection::onSelect(QString name, QVector3D position)
    {
      if (currentSelected_ == name) return;
      if (currentSelected_ != "") wsm_->setHighlight(currentSelected_.toStdString().c_str(), 3);
      currentSelected_ = name;
      wsm_->setHighlight(name.toStdString().c_str(), 8);
      selectedBodies_ << name;
      emit selectedBodies(selectedBodies_);
    }
  }
}
